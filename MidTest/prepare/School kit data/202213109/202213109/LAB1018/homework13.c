#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <time.h>
#include <wiringPiSPI.h>
#include <math.h>
#include <errno.h>
#include <string.h>

#define DB_HOST "localhost"
#define DB_USER "202213109"
#define DB_PASS "10015"
#define DB_NAME "EMB202213109"

#define ULTRASONIC_TRIG 4
#define ULTRASONIC_ECHO 5

#define CHAN_CONFIG_SINGLE 8 //- 싱글 채널 설정 값
#define SPI_SPEED 1000000 //- 1MHz
#define SPI_CE_CHANNEL 1 //- SPI와 연결된 CE 채널
#define SPI_ADC_LIGHT_CHANNEL 0 //- Light 연결 채널
#define CS_MCP3208 11 //- MCP3208 연결 핀
int spiSetup(void);
int readAnalogData(int adcChannel);
MYSQL *connector;
MYSQL_RES *result;
MYSQL_ROW row;

typedef struct _sensor {
    int ultrasonic;
    int illum;
}SENSOR;

int main(void){
    printf( "Raspberry Pi - MySQL DB Test \n" );
    if (wiringPiSetup()==-1) exit(1);
        
    connector = mysql_init(NULL);
    if(!mysql_real_connect(connector, DB_HOST, DB_USER, DB_PASS,DB_NAME, 3306, NULL, 0)){
        fprintf( stderr, "%s \n", mysql_error(connector));
        return 0;
    }else{
        printf("MySQL(raspiDB) opened! \n");
    }

    spiSetup();
    pinMode(CS_MCP3208, OUTPUT);
    int adcValue=0;

    pinMode(ULTRASONIC_TRIG, OUTPUT);
    pinMode(ULTRASONIC_ECHO, INPUT);
    int start_time=0, end_time=0;
    float distance=0, duration=0;
    mysql_query(connector, "delete from sensor");
    while(1){
        char query[1024];
        getchar();

        //- 초음파 센서에 동작 커맨드 신호 보내기

        digitalWrite(ULTRASONIC_TRIG, LOW);
        delay(500);
        digitalWrite(ULTRASONIC_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(ULTRASONIC_TRIG, LOW);
        //- 초음파 발사 시작 시간 측정
        while(digitalRead(ULTRASONIC_ECHO) == LOW)
        start_time = micros();
        //- 초음파 반사되어 돌아온 시간 측정
        while(digitalRead(ULTRASONIC_ECHO) == HIGH)
        end_time = micros();
        //- 두 시간 차는 왕복으로 나누기 2해서 거리 구하기
        duration = (float)(end_time - start_time);
        duration = duration/1000000/2;
        distance=(340 * duration) * 100;

        adcValue = readAnalogData(SPI_ADC_LIGHT_CHANNEL);

        SENSOR gData = {distance,adcValue};
        sprintf(query, "insert into sensor values (now(), %d, %d)", gData.ultrasonic, gData.illum);
        if(mysql_query(connector, query)){
            fprintf(stderr, "%s \n", mysql_error(connector));
            printf("Write DB error. \n");
        }else{
            printf("Insert OK!! \n");
        }
    delay(1000);
    }
    mysql_close(connector);
    return(0);
}

int spiSetup(void){
if( wiringPiSPISetup (SPI_CE_CHANNEL, SPI_SPEED) == -1 ){
fprintf (stderr, "wiringPiSPISetup Failed ! ERROR : %s\n",
strerror (errno));
return 1;
}
}
//- 채널 1번에서 측정된 ADC센서 읽어오기 ----------------------------
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