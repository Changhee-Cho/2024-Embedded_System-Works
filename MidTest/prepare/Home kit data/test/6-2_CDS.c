#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>
#include <string.h>

#define CHAN_CONFIG_SINGLE 8
#define SPI_SPEED 1000000
#define SPI_CE_CHANNEL 1
#define SPI_ADC_LIGHT_CHANNEL 0
#define CS_MCP3208 11

int spiSetup(void);
int readAnalogData(int adcChannel);

int main(){
    wiringPiSetup();
    spiSetup();
    pinMode(CS_MCP3208, OUTPUT);
    int adcValue = 0;
    while(1){
        adcValue=readAnalogData(SPI_ADC_LIGHT_CHANNEL);
        printf("Light Value = %d\n", adcValue);
        delay(500);
    }
    return 0;
}

int spiSetup(void){
    if(wiringPiSPISetup(SPI_CE_CHANNEL, SPI_SPEED)==-1){
        fprintf(stderr, "wiringPiSPISetup Failed! Error: %s\n", strerror(errno));
        return 1;
    }
}

int readAnalogData(int adcChannel){
    int adcValue=0;
    unsigned char buffer[3] = {0};
    if(adcChannel<0||adcChannel>7)
        return -1;
    buffer[0]=0x01;
    buffer[1]=(CHAN_CONFIG_SINGLE+adcChannel)<<4;
    digitalWrite(CS_MCP3208, LOW);
    wiringPiSPIDataRW(SPI_CE_CHANNEL, buffer, 3);
    adcValue=(((buffer[1]&3)<<8)+buffer[2]);
    digitalWrite(CS_MCP3208,HIGH);
    return adcValue;
}