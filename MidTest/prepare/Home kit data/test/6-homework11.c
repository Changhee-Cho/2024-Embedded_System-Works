
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <softTone.h>
#define CHAN_CONFIG_SINGLE 8
#define SPI_SPEED 1000000
#define SPI_CE_CHANNEL 1
#define SPI_ADC_POTEN_CHANNEL 1
#define SPI_ADC_LIGHT_CHANNEL 0
#define CS_MCP3208 11

#define BUZ 21
#define LED 27
unsigned int melody[8] = {262, 294, 330, 349, 392, 440, 494, 523};

int spiSetup();
int readAnalogData(int adcChannel);

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Fail\n");
        return -1;
    }
    softToneCreate(BUZ);
    spiSetup();
    pinMode(CS_MCP3208, OUTPUT);
    int adcValue = 0;

    getchar();
    srand((unsigned int)time(NULL));
    int key = rand()%1001+10;
    printf("key: %d\n", key);
    int status = 0;
    int cds = 0;
    while(1){
        adcValue =readAnalogData(SPI_ADC_POTEN_CHANNEL);
        printf("Potentionmeter value = %d\n", adcValue);
        delay(500);
        if(adcValue<=key+100 && adcValue>= key-100){
            digitalWrite(LED, HIGH);
            if(adcValue<=key+10 && adcValue>= key-10&&status == 0){
                softToneWrite(BUZ, melody[0]);
                delay(300);
                softToneWrite(BUZ, melody[2]);
                delay(300);
                softToneWrite(BUZ, melody[4]);
                delay(300);
                softToneWrite(BUZ, 0);
                status = 1;
                cds = readAnalogData(SPI_ADC_LIGHT_CHANNEL);
                if(cds>250){
                    char cmd[]="sudo libcamera-jpeg --width 800 --height 800 -t 1 -o test.jpg";
                    system(cmd);
                    printf("Picture created\n");
                    return 0;
                }
                else
                    return 0;
            }
            else{
                softToneWrite(BUZ, 0);
            }
        }
        else{
            digitalWrite(LED, LOW);
            status=0;
        }
    }
}

int spiSetup(){
    if(wiringPiSPISetup(SPI_CE_CHANNEL, SPI_SPEED) == -1){
        fprintf(stderr, "wiringPiSetup Failed ! ERROR: %s\n", strerror(errno));
        return 1;
    }
}

int readAnalogData(int adcChannel){
    int adcValue = 0;
    unsigned char buffer[3] = {0};
    if(adcChannel<0 || adcChannel>7)
        return 1;
    buffer[0]=0x01;
    buffer[1]=(CHAN_CONFIG_SINGLE+adcChannel)<<4;
    digitalWrite(CS_MCP3208, LOW);
    wiringPiSPIDataRW(SPI_CE_CHANNEL, buffer, 3);
    adcValue = ((buffer[1]&3)<< 8)+buffer[2];
    digitalWrite(CS_MCP3208, HIGH);
    return adcValue;
}