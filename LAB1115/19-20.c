#include <stdio.h>
#include <unistd.h>   
#include <stdlib.h>   
#include <string.h> 
#include <errno.h>
#include <wiringPi.h> 
#include <wiringSerial.h>
#include <wiringPiI2C.h>
#include <wiringPiSPI.h>
#include <pcf8574.h>
#include <lcd.h>
#include <softTone.h>
#include <softPwm.h>

// SPI ADC
#define CHAN_CONFIG_SINGLE 8 
#define SPI_SPEED 1000000 
#define SPI_CE_CHANNEL 1 
#define SPI_ADC_LIGHT_CHANNEL 0 
#define SPI_ADC_POTEN_CHANNEL 1
#define CS_MCP3208 11 
#define BUZ_PIN		  30
#define TRIG 4
#define ECHO 5

#define BUZ_P_PIN 21

#define BASE_ADDR 100
#define RS_PIN BASE_ADDR //- 100
#define RW_PIN BASE_ADDR+1 //- 101
#define EN_PIN BASE_ADDR+2 //- 102
#define BL_PIN BASE_ADDR+3 //- 103
#define D4_PIN BASE_ADDR+4 //- 104
#define D5_PIN BASE_ADDR+5 //- 105
#define D6_PIN BASE_ADDR+6 //- 106
#define D7_PIN BASE_ADDR+7 //- 107


#define DC1_A 1
#define DC1_B 4
#define DC2_A 5
#define DC2_B 6

#define FRONT_TRIG 28
#define FRONT_ECHO 29
#define RIGHT_TRIG 26
#define RIGHT_ECHO 27
#define LEFT_TRIG 10
#define LEFT_ECHO 11

//- 함수 선언 ---------------------------------------------------------------------
void hw_init();
void parserCmd();
void InitBT(); 
void CaptureImage();
void sendSensorData(int dp, float value);
void sendImageData();
void LedOn(int type, int bOn);
void BuzzerOn(int bOn);
int readAnalogData(int adcChannel);



//- 전역 변수 및 상수 -------------------------------------------------------------
enum { LED_RED, LED_GREEN, LED_BLUE};
static unsigned char LED[] = {27, 28, 29};
int potential, illum;
int bBuzzerRun =0;
char rx_buf[BUFSIZ] = "";
int  g_hBT;

int speed = 40;
int drive = 0;
int status = 0;

float distance_front = 0, distance_left=0, distance_right=0;

unsigned int nNextTime, nBuzzTime, nSensorTime;

PI_THREAD(front){
    while(1){
        int start = 0, end =0;
        float distance = 0, duration = 0;

        digitalWrite(FRONT_TRIG, LOW);
        delay(500);
        digitalWrite(FRONT_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(FRONT_TRIG, LOW);

        while(digitalRead(FRONT_ECHO)==LOW)
            start=micros();
        
        while(digitalRead(FRONT_ECHO)==HIGH)
            end=micros();

        duration=(float)(end-start);
        duration=duration/1000000/2;
        distance=(340*duration)*100;
        distance_front=distance;

    }
}
PI_THREAD(left){
    while(1){
        int start = 0, end =0;
        float distance = 0, duration = 0;

        digitalWrite(LEFT_TRIG, LOW);
        delay(500);
        digitalWrite(LEFT_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(LEFT_TRIG, LOW);

        while(digitalRead(LEFT_ECHO)==LOW)
            start=micros();
        
        while(digitalRead(LEFT_ECHO)==HIGH)
            end=micros();

        duration=(float)(end-start);
        duration=duration/1000000/2;
        distance=(340*duration)*100;
        distance_left=distance;
    }
}
PI_THREAD(right){
    while(1){
        int start = 0, end =0;
        float distance = 0, duration = 0;

        digitalWrite(RIGHT_TRIG, LOW);
        delay(500);
        digitalWrite(RIGHT_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(RIGHT_TRIG, LOW);

        while(digitalRead(RIGHT_ECHO)==LOW)
            start=micros();
        
        while(digitalRead(RIGHT_ECHO)==HIGH)
            end=micros();

        duration=(float)(end-start);
        duration=duration/1000000/2;
        distance=(340*duration)*100;
        distance_right=distance;
    }
}

int main(){
    wiringPiSetup();
;
    InitBT();                                  
    hw_init();
    piThreadCreate(front);
    piThreadCreate(left);
    piThreadCreate(right); 

    pinMode(DC1_A, SOFT_PWM_OUTPUT);
    pinMode(DC1_B, SOFT_PWM_OUTPUT);
    pinMode(DC2_A, SOFT_PWM_OUTPUT);
    pinMode(DC2_B, SOFT_PWM_OUTPUT);
    
    softPwmCreate(DC1_A, 0, 100);
    softPwmCreate(DC1_B, 0, 100);
    softPwmCreate(DC1_A, 0, 100);
    softPwmCreate(DC2_B, 0, 100);

    pinMode(FRONT_TRIG, OUTPUT);
    pinMode(FRONT_ECHO, INPUT);
    pinMode(RIGHT_TRIG, OUTPUT);
    pinMode(RIGHT_ECHO, INPUT);
    pinMode(LEFT_TRIG, OUTPUT);
    pinMode(LEFT_ECHO, INPUT);

    while(1){
        if(millis() > nNextTime + 500){
		int i=0;
		memset(rx_buf, 0, sizeof(rx_buf));
		while(serialDataAvail(g_hBT) > 0){
				rx_buf[i] = serialGetchar(g_hBT);
				if(rx_buf[i] == '#'){
					if(rx_buf[0] == '@') parserCmd();
					break;
				}
				i++;	
		}
              nNextTime = millis();
        }
    }
    printf ("\n") ;
    return 0 ;
}
void hw_init(void){
    wiringPiSetup();
    if( wiringPiSPISetup (SPI_CE_CHANNEL, SPI_SPEED) == -1 ){
        fprintf (stderr, "wiringPiSPISetup Failed ! ERROR : %s\n", strerror (errno));
        return;
    }
	int i;
	for(i=0; i<sizeof(LED); i++){
		pinMode(LED[i], OUTPUT);           
		digitalWrite(LED[i], LOW);
	}

	pinMode(BUZ_PIN,OUTPUT);   
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);               
}
void parserCmd()
{
    int index = 1;
    printf("%s",rx_buf);
    if(!strncmp(&rx_buf[1], "BUZ", 3)) {   
        index += 3;
        if(rx_buf[index] == ',')
        {
            int bOn;
            index++;                
            if(rx_buf[index] == '0')
            {
		 printf("Buzzer off\n");
                bOn = 0;
            }else if(rx_buf[index] == '1'){
		 printf("Buzzer on\n");
                bOn = 1;
            }
            index++;
            if(rx_buf[index]== '#'){
				BuzzerOn(bOn);
                bBuzzerRun = bOn;
            }
        }
    }
    else if(!strncmp(&rx_buf[index],"6SC",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            sendSensorData(1, speed);
        }
    }
    else if(!strncmp(&rx_buf[index],"6UL",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            sendSensorData(2,distance_left);
        }
    }
    else if(!strncmp(&rx_buf[index],"6UF",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            sendSensorData(4, distance_front);
        }
    }
    else if(!strncmp(&rx_buf[index],"6UR",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            sendSensorData(3, distance_right);
        }
    }
    else if(!strncmp(&rx_buf[index],"6SU",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            if(speed<100){
                speed += 10;
                switch(status){
                    case 0:
                        softPwmWrite(DC1_A, 0);
                        softPwmWrite(DC1_B, 0);
                        softPwmWrite(DC2_A, 0);
                        softPwmWrite(DC2_B, 0);
                        drive = 0;
                        status = 0;
                        break;
                    //정지
                    case 1:
                        drive=1;
                        status = 1;
                        softPwmWrite(DC1_A, speed);
                        softPwmWrite(DC1_B, 0);
                        softPwmWrite(DC2_A, speed);
                        softPwmWrite(DC2_B, 0);
                        break;
                        //전진
                    case 2:
                        drive=1;
                        status=2;
                        softPwmWrite(DC1_A, 0);
                        softPwmWrite(DC1_B, speed);
                        softPwmWrite(DC2_A, 0);
                        softPwmWrite(DC2_B, speed);
                        break;
                        //후진
                    case 3:
                        //좌회전
                        status=3;
                        drive=1;
                        softPwmWrite(DC1_A, 0);
                        softPwmWrite(DC1_B, speed);
                        softPwmWrite(DC2_A, speed);
                        softPwmWrite(DC2_B, 0);
                        break;
                    case 4:
                        //우회전
                        status=4;
                        drive=1;
                        softPwmWrite(DC1_A, speed);
                        softPwmWrite(DC1_B, 0);
                        softPwmWrite(DC2_A, 0);
                        softPwmWrite(DC2_B, speed);
                        break;
                }
            }
        }
    }
    else if(!strncmp(&rx_buf[index],"6SD",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            if(speed>0){
                speed -= 10;
                switch(status){
                    case 0:
                        softPwmWrite(DC1_A, 0);
                        softPwmWrite(DC1_B, 0);
                        softPwmWrite(DC2_A, 0);
                        softPwmWrite(DC2_B, 0);
                        drive = 0;
                        status = 0;
                        break;
                    //정지
                    case 1:
                        drive=1;
                        status = 1;
                        softPwmWrite(DC1_A, speed);
                        softPwmWrite(DC1_B, 0);
                        softPwmWrite(DC2_A, speed);
                        softPwmWrite(DC2_B, 0);
                        break;
                        //전진
                    case 2:
                        drive=1;
                        status=2;
                        softPwmWrite(DC1_A, 0);
                        softPwmWrite(DC1_B, speed);
                        softPwmWrite(DC2_A, 0);
                        softPwmWrite(DC2_B, speed);
                        break;
                        //후진
                    case 3:
                        //좌회전
                        status=3;
                        drive=1;
                        softPwmWrite(DC1_A, 0);
                        softPwmWrite(DC1_B, speed);
                        softPwmWrite(DC2_A, speed);
                        softPwmWrite(DC2_B, 0);
                        break;
                    case 4:
                        //우회전
                        status=4;
                        drive=1;
                        softPwmWrite(DC1_A, speed);
                        softPwmWrite(DC1_B, 0);
                        softPwmWrite(DC2_A, 0);
                        softPwmWrite(DC2_B, speed);
                        break;
                }
            }
        }
    }
    else if(!strncmp(&rx_buf[index],"6TP",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            drive=1;
            status = 1;
            softPwmWrite(DC1_A, speed);
            softPwmWrite(DC1_B, 0);
            softPwmWrite(DC2_A, speed);
            softPwmWrite(DC2_B, 0);
        }
    }
    else if(!strncmp(&rx_buf[index],"6LR",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            status=3;
            drive=1;
            softPwmWrite(DC1_A, 0);
            softPwmWrite(DC1_B, speed);
            softPwmWrite(DC2_A, speed);
            softPwmWrite(DC2_B, 0);
        }
    }
    else if(!strncmp(&rx_buf[index],"6RR",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            status=4;
            drive=1;
            softPwmWrite(DC1_A, speed);
            softPwmWrite(DC1_B, 0);
            softPwmWrite(DC2_A, 0);
            softPwmWrite(DC2_B, speed);
        }
    }
    else if(!strncmp(&rx_buf[index],"6BT",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            drive=1;
            status=2;
            softPwmWrite(DC1_A, 0);
            softPwmWrite(DC1_B, speed);
            softPwmWrite(DC2_A, 0);
            softPwmWrite(DC2_B, speed);
        }
    }
    else if(!strncmp(&rx_buf[index],"6ST",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            softPwmWrite(DC1_A, 0);
            softPwmWrite(DC1_B, 0);
            softPwmWrite(DC2_A, 0);
            softPwmWrite(DC2_B, 0);
            drive = 0;
            status = 0;
        }
    }
    // 과제19 후방주차
    else if(!strncmp(&rx_buf[index],"7PK",3)){
        //@SHT#
        //전진
        if(rx_buf[4]=='#'){
            int status = 0;
            // 버튼 누르면 전진
            softPwmWrite(DC1_A, speed);
            softPwmWrite(DC1_B, 0);
            softPwmWrite(DC2_A, speed);
            softPwmWrite(DC2_B, 0);

            while(1){
                printf("%d\n", status);
                if((status!=1&&status!=3)&& distance_left<40){
                    // 왼쪽이 8cm 이하면 좌회전하라
                    status = 2;
                    // 근데 만약 우측 거리가 40cm 이하라면 직진하여라
                    softPwmWrite(DC1_A, 0);
                    softPwmWrite(DC1_B, speed);
                    softPwmWrite(DC2_A, speed);
                    softPwmWrite(DC2_B, 0);
                    delay(800);
                    status = 1;
                    
                }
                if(status==1 && distance_front < 15){
                    status=3;
                    softPwmWrite(DC1_A, 0);
                    softPwmWrite(DC1_B, speed);
                    softPwmWrite(DC2_A, 0);
                    softPwmWrite(DC2_B, speed);
                    delay(3500);
                    softPwmWrite(DC1_A, 0);
                    softPwmWrite(DC1_B, 0);
                    softPwmWrite(DC2_A, 0);
                    softPwmWrite(DC2_B, 0);
                    break;
                }
                if(status==3&& distance_front>30){
                    status=4;
                    break;
                }


                // 자동화 코드
                if(status == 1){
                    // 전진
                    softPwmWrite(DC1_A, speed);
                    softPwmWrite(DC1_B, 0);
                    softPwmWrite(DC2_A, speed);
                    softPwmWrite(DC2_B, 0);
                }
                else if(status == 2){
                    // 좌회전
                    softPwmWrite(DC1_A, 0);
                    softPwmWrite(DC1_B, speed);
                    softPwmWrite(DC2_A, speed);
                    softPwmWrite(DC2_B, 0);
                }
                else if(status == 3){
                    // 후진
                    softPwmWrite(DC1_A, 0);
                    softPwmWrite(DC1_B, speed);
                    softPwmWrite(DC2_A, 0);
                    softPwmWrite(DC2_B, speed);
                }
                else if(status == 4){
                    //정지
                    softPwmWrite(DC1_A, 0);
                    softPwmWrite(DC1_B, 0);
                    softPwmWrite(DC2_A, 0);
                    softPwmWrite(DC2_B, 0);
                    break;
                }
            }
        }
    }
    // 과제 20 트랙주행
        else if(!strncmp(&rx_buf[index],"7DV",3)){
        //@SHT#
        //전진
        if(rx_buf[4]=='#'){
            //우회전하는데 3초가 필요함
            softPwmWrite(DC1_A, speed);
            softPwmWrite(DC1_B, 0);
            softPwmWrite(DC2_A, speed);
            softPwmWrite(DC2_B, 0);

            delay(2000);

            softPwmWrite(DC1_A, speed);
            softPwmWrite(DC1_B, 0);
            softPwmWrite(DC2_A, 0);
            softPwmWrite(DC2_B, speed);

            delay(2000);

            softPwmWrite(DC1_A, speed);
            softPwmWrite(DC1_B, 0);
            softPwmWrite(DC2_A, speed);
            softPwmWrite(DC2_B, 0);

            delay(3400);

            softPwmWrite(DC1_A, speed);
            softPwmWrite(DC1_B, 0);
            softPwmWrite(DC2_A, 0);
            softPwmWrite(DC2_B, speed);

            delay(2000);

            softPwmWrite(DC1_A, speed);
            softPwmWrite(DC1_B, 0);
            softPwmWrite(DC2_A, speed);
            softPwmWrite(DC2_B, 0);

            delay(2000);

            softPwmWrite(DC1_A, 0);
            softPwmWrite(DC1_B, 0);
            softPwmWrite(DC2_A, 0);
            softPwmWrite(DC2_B, 0);
            
            
            system("sudo libcamera-jpeg -t 1 --width 600 --height 400 -o test.jpeg");
            return;
            }



            /*
            status = 0;
            softPwmWrite(DC1_A, speed);
            softPwmWrite(DC1_B, 0);
            softPwmWrite(DC2_A, speed);
            softPwmWrite(DC2_B, 0);
            while(1){
                
                // 자동화 코드
                if(status == 1){
                    // 전진
                    softPwmWrite(DC1_A, speed);
                    softPwmWrite(DC1_B, 0);
                    softPwmWrite(DC2_A, speed);
                    softPwmWrite(DC2_B, 0);
                }
                else if(status == 2){
                    // 우회전
                    softPwmWrite(DC1_A, speed);
                    softPwmWrite(DC1_B, 0);
                    softPwmWrite(DC2_A, 0);
                    softPwmWrite(DC2_B, speed);
                }
                else if(status == 3){
                    // 후진
                    softPwmWrite(DC1_A, 0);
                    softPwmWrite(DC1_B, speed);
                    softPwmWrite(DC2_A, 0);
                    softPwmWrite(DC2_B, speed);
                }
                else if(status == 4){
                    //정지
                    softPwmWrite(DC1_A, 0);
                    softPwmWrite(DC1_B, 0);
                    softPwmWrite(DC2_A, 0);
                    softPwmWrite(DC2_B, 0);
                    break;
                }
                
            }*/
        }
    }

//- 블루투스 통신 초기화 -------------------------------------------------------
void InitBT() 
{
	g_hBT = serialOpen("/dev/rfcomm0", 115200);
	printf("Bluetooth init\n");
}

//- 온도, 습도 전송 --------------------------------------------------------------
void sendSensorData(int dp, float value)
{
    
}

//- LED On/Off 제어 -------------------------------------------------------------
void LedOn(int type, int bOn)
{
	digitalWrite(LED[type], bOn);
}
//- Buzzer On/Off 제어 ----------------------------------------------------------
void BuzzerOn(int bOn)
{
	if(bOn)
		digitalWrite(BUZ_PIN, HIGH);
	else
		digitalWrite(BUZ_PIN, LOW);
}
void CaptureImage(){
    system("sudo libcamera-jpeg -t 1 --width 600 --height 400 -o test.jpeg");
    return;
}
void sendImageData()
{
	char tx_buf[16];
	FILE *fp_read;
	long file_size;
	char *memblock;
	
	fp_read = fopen("/home/pi/202213109/LAB1115/test.jpeg", "rb");
	if(fp_read != NULL){
		fseek(fp_read, 0, SEEK_END);
		file_size = ftell(fp_read);
		memblock = (char*)malloc(file_size);
		fseek(fp_read, 0, SEEK_SET);
		fread(memblock, 1, file_size, fp_read);
		fclose(fp_read);
		
		serialPrintf(g_hBT, "@IMG,");		
		write(g_hBT, memblock, file_size);
		serialPrintf(g_hBT, "####");
		free(memblock);
	}else{
		printf("File read error!\n");
		return;
	}
}
int readAnalogData(int adcChannel){
    int adcValue = 0;
    unsigned char buffer[3] = {0};
    //- MCP3008의 8개 채널 중 하나인지 체크
    if(adcChannel<0 || adcChannel>7) return -1;
    //- SPI 통신 버퍼 설정
    buffer[0] = 0x01;
    buffer[1] = (CHAN_CONFIG_SINGLE+adcChannel) << 4;
    //- SPI 통신할 칩 활성화
    digitalWrite(CS_MCP3208, LOW);
    //- 채널 1에서 측정된 ADC 센서 읽어 오기
    wiringPiSPIDataRW(SPI_CE_CHANNEL, buffer, 3);
    //- 읽어온 값에서 10비트 데이터 추출하기
    adcValue = ( (buffer[1] & 3 ) << 8 ) + buffer[2];
    //- SPI 통신할 칩 비활성화
    digitalWrite(CS_MCP3208, HIGH);
    return adcValue;
}