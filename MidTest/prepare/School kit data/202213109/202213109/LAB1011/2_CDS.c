#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>
#include <string.h>
#define CHAN_CONFIG_SINGLE 8 //- 싱글 채널 설정 값
#define SPI_SPEED 1000000 //- 1MHz
#define SPI_CE_CHANNEL 1 //- SPI와 연결된 CE 채널
#define SPI_ADC_LIGHT_CHANNEL 0 //- Light 연결 채널
#define CS_MCP3208 11 //- MCP3208 연결 핀
int spiSetup(void);
int readAnalogData(int adcChannel);
int main(void){
//- wiringPi 초기화
if( wiringPiSetup() == -1){
printf("Setup Fail ~!!");
return -1;
}
//- SPI통신 초기화
spiSetup();
//- ADC IC MCP3008 초기화
pinMode(CS_MCP3208, OUTPUT);
int adcValue = 0;
//- 센서 읽어서 출력
while(1){
//- 센서 값 읽기
adcValue = readAnalogData(SPI_ADC_LIGHT_CHANNEL);
printf(" Light Value = %dLux\n", adcValue);
delay(500);
}
return 0;
}

int spiSetup(void){
if( wiringPiSPISetup (SPI_CE_CHANNEL, SPI_SPEED) == -1 ){
fprintf (stderr, "wiringPiSPISetup Failed ! ERROR : %s\n",
strerror (errno));
return 1;
}
}
//- 채널 1번에서 측정된 ADC센서 읽어오기
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