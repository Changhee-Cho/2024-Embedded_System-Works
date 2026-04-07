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

//- 함수 선언 ---------------------------------------------------------------------
void hw_init();
void parserCmd();
void InitBT(); 
void CaptureImage();
void sendSensorData();
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

unsigned int melody[8] = {262, 294, 330, 349, 392, 440, 494, 523};
int star[] = {0,0,4,4,5,5,4,3,3,2,2,1,1,0,4,4,3,3,2,2,1,4,4,3,3,2,2,1,0,0,4,4,5,5,4,3,3,2,2,1,1,0};
int rabbit[] ={4,2,2,4,2,0,1,2,1,0,2,4,7,4,7,4,7,4,2,4,1,3,2,1,0};

int lcd_bl =1;
int fd;
int start = 0, end=0;
float distance = 0, duration=0;

int idx=0;
int melon =0;
/*
PI_THREAD(t2){
    while(1){
        print("%d\n",melon);
        if(melon==1){
            softToneWrite(BUZ_P_PIN, melody[rabbit[idx]]);
            delay(500);
            softToneWrite(BUZ_P_PIN, 0);
            delay(500);
            idx++;
            if(idx==(sizeof(rabbit)/sizeof(rabbit[1]))){
                idx=0;
            }
        }
    }
}
*/
/*
PI_THREAD(t3){
    while(1){
            if(melon==2){
            softToneWrite(BUZ_P_PIN, melody[star[idx]]);
            delay(500);
            softToneWrite(BUZ_P_PIN, 0);
            delay(500);
            idx++;
            if(idx==(sizeof(star)/sizeof(star[1]))){
                idx=0;
            }
    }
    }

}
*/
int main(){
        //- I2C 통신기반 LCD 슬레이브 설정
    pcf8574Setup(RS_PIN, 0x27); //- P0핀 - LCD의 RS 연결, 주소 지정
    //- P1핀 - LCD의 RW 연결, 동작모드 설정
    pinMode (RW_PIN, OUTPUT);
    digitalWrite (RW_PIN, LOW);
    //- P3핀 - LCD의 Backlight 연결
    pinMode (BL_PIN, OUTPUT);
    digitalWrite (BL_PIN, HIGH); //- Backlight ON
    //- LCD 제어 핸들
    fd = lcdInit (2, 16, 4, RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN,
    D7_PIN, 0,0,0,0 );
    lcdClear(fd);
    unsigned int nNextTime, nBuzzTime, nSensorTime;
    InitBT();                                  
    hw_init();  
    pinMode(BUZ_P_PIN, SOFT_TONE_OUTPUT);
    softToneCreate(BUZ_P_PIN);
    piThreadCreate(t2);
    piThreadCreate(t3);
                               
    nNextTime = millis();                     
    nBuzzTime = millis();
    nSensorTime = millis();   

    while(1){
        if(millis() > nNextTime + 250){
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
    if(millis() > nSensorTime + 1000)
		{
			potential = readAnalogData(SPI_ADC_POTEN_CHANNEL);
			illum = readAnalogData(SPI_ADC_LIGHT_CHANNEL);
            
			
			nSensorTime = millis();	
            digitalWrite(TRIG, LOW);
            delay(500);
            digitalWrite(TRIG, HIGH);
            delayMicroseconds(10);
            digitalWrite(TRIG, LOW);
            while(digitalRead(ECHO) == LOW)
            start = micros();
            //- 초음파 반사되어 돌아온 시간 측정
            while(digitalRead(ECHO) == HIGH)
            end = micros();
            //- 두 시간 차는 왕복으로 나누기 2해서 거리 구하기
            duration = (float)(end - start);
            duration = duration/1000000/2;
            distance=(340 * duration) * 100;

            printf("Send Sensor Data\n");
			sendSensorData();
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
    }else if(!strncmp(&rx_buf[1], "RED", 3)) { 
        index += 3;   
        if(rx_buf[index] == ',')
        {
            int bOn = 0;
            index++;                
            if(rx_buf[index] == '0')
            {
		 printf("Red LED off\n");
                bOn = 0;
            }else if(rx_buf[index] == '1'){
		 printf("Red LED on\n");
                bOn = 1;
            }

            index++;
            if(rx_buf[index]== '#')
                LedOn(LED_RED, bOn);
       }

    }else if(!strncmp(&rx_buf[1], "GED", 3)) {
        index += 3;
        if(rx_buf[index] == ',')
        {
            int bOn = 0;
            index++;                
            if(rx_buf[index] == '0')
            {
		 printf("Green LED off\n");
                bOn = 0;
            }else if(rx_buf[index] == '1'){
		 printf("Green LED on\n");
                bOn = 1;
            }

            index++;
            if(rx_buf[index]== '#')
                LedOn(LED_GREEN, bOn);
        }
    }else if(!strncmp(&rx_buf[1], "BED", 3)){
        index += 3;
        if(rx_buf[index] == ',')
        {
            int bOn = 0;
            index++;                
            if(rx_buf[index] == '0')
            {
		 printf("BLUE LED off\n");
                bOn = 0;
            }

			else if(rx_buf[index] == '1'){
		 printf("BLUE LED on\n");
                bOn = 1;
            }
            index++;
            if(rx_buf[index]== '#')
                LedOn(LED_BLUE, bOn);
        }
    }else if(!strncmp(&rx_buf[1], "WED", 3)){
        index += 3;
        if(rx_buf[index] == ',')
        {
            int bOn = 1;
            index++;                
            if(rx_buf[index] == '0')
            {
		 printf("White LED off\n");
                bOn = 0;
            }else if(rx_buf[index] == '1'){
		 printf("White LED on\n");
                bOn = 1;
            }

            index++;
            if(rx_buf[index]== '#'){
                LedOn(LED_BLUE, bOn);
                LedOn(LED_GREEN, bOn);
                LedOn(LED_RED, bOn);
            }
        }
    }else if(!strncmp(&rx_buf[index],"SHT",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            printf("Capture Image\n");
            CaptureImage();
            printf("Send Image\n");
            sendImageData();
        }
    }
    //메시지 LCD 출력
    else if(!strncmp(&rx_buf[index],"STM",3)){
        //@SHT#
        index += 3;
        if(rx_buf[index] == ',')
        {
            int bOn = 0;
            index++;
            int tempidx = 0;
            while(rx_buf[index]!='#'){
                lcdPutchar(fd, rx_buf[index]);
                index++;
            }
            if(rx_buf[index]== '#')
                lcdHome(fd);
        }
        
    }
    // LCD 백라이트 ON
    else if(!strncmp(&rx_buf[index],"LON",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            digitalWrite(BL_PIN, HIGH);
        }
    }
    // LCD 백라이트 OFF
    else if(!strncmp(&rx_buf[index],"LOF",3)){
        //@SHT#
        digitalWrite(BL_PIN, LOW);
    }
    // LCD 클리어
    else if(!strncmp(&rx_buf[index],"LCR",3)){
        //@SHT#
        lcdClear(fd);
    }
    //소리 도부터
    else if(!strncmp(&rx_buf[index],"4DO",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            softToneWrite(BUZ_P_PIN,melody[0]);
            delay(500);
            softToneWrite(BUZ_P_PIN, 0);
        }
    }
    //레
    else if(!strncmp(&rx_buf[index],"4RE",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            softToneWrite(BUZ_P_PIN,melody[1]);
            delay(500);
            softToneWrite(BUZ_P_PIN, 0);
        }
    }
    //미
    else if(!strncmp(&rx_buf[index],"4MI",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            softToneWrite(BUZ_P_PIN,melody[2]);
            delay(500);
            softToneWrite(BUZ_P_PIN, 0);
        }
    }
    //파
    else if(!strncmp(&rx_buf[index],"4FA",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            softToneWrite(BUZ_P_PIN,melody[3]);
            delay(500);
            softToneWrite(BUZ_P_PIN, 0);
        }
    }
    //솔
    else if(!strncmp(&rx_buf[index],"4SO",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            softToneWrite(BUZ_P_PIN,melody[4]);
            delay(500);
            softToneWrite(BUZ_P_PIN, 0);
        }
    }
    //라
    else if(!strncmp(&rx_buf[index],"4LA",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            softToneWrite(BUZ_P_PIN,melody[5]);
            delay(500);
            softToneWrite(BUZ_P_PIN, 0);
        }
    }
    //시
    else if(!strncmp(&rx_buf[index],"4TI",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            softToneWrite(BUZ_P_PIN,melody[6]);
            delay(500);
            softToneWrite(BUZ_P_PIN, 0);
            printf("시 눌림\n");
        }
    }
    //끝
    else if(!strncmp(&rx_buf[index],"4ED",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            idx=0;
            melon=0;
        }
    }
    //산토끼
    else if(!strncmp(&rx_buf[index],"4RB",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            
            idx=0;
            melon=1;
        }
    }
    //작은별
    else if(!strncmp(&rx_buf[index],"4SA",3)){
        //@SHT#
        if(rx_buf[4]=='#'){
            idx=0;
            melon=2;
        }
    }
    else{
	printf("%s\n", rx_buf);
        return;
    }
}

//- 블루투스 통신 초기화 -------------------------------------------------------
void InitBT() 
{
	g_hBT = serialOpen("/dev/rfcomm0", 115200);
	printf("Bluetooth init\n");
}

//- 온도, 습도 전송 --------------------------------------------------------------
void sendSensorData()
{
	serialPrintf(g_hBT, "@POT,%d#@CDS,%d#@ULT,%f#", potential, illum, distance);
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
	
	fp_read = fopen("/home/pi/202213109/LAB1101/test.jpeg", "rb");
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