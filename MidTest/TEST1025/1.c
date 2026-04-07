#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>
#include <string.h>
#include <softPwm.h>

#define DC_A 6
#define DC_B 31

#define CHAN_CONFIG_SINGLE 8
#define SPI_SPEED 1000000
#define SPI_CE_CHANNEL 1
#define SPI_ADC_POTEN_CHANNEL 1
#define CS_MCP3208 11

#define BUZ 30

int spiSetup();
int readAnalogData(int adcChannel);

int main(){
    wiringPiSetup();
    spiSetup();
    pinMode(DC_A, SOFT_PWM_OUTPUT);
    pinMode(DC_B, SOFT_PWM_OUTPUT);
    pinMode(BUZ, OUTPUT);
    pinMode(CS_MCP3208, OUTPUT);
    softPwmCreate(DC_A, 0, 100);
    softPwmCreate(DC_B, 0, 100);
    int adcValue=0;
    int status768 =0;
    int status512 = 0;
    while(1){
        adcValue=readAnalogData(SPI_ADC_POTEN_CHANNEL);
        int speed = adcValue / 100 * 10;
        softPwmWrite(DC_A, speed);
        if(adcValue>=768 && status768!=1 &&status512!=1){
            status768 = 1;
            status512=1;
            digitalWrite(BUZ, HIGH);
            delay(200);
            digitalWrite(BUZ, LOW);
            delay(200);
            digitalWrite(BUZ, HIGH);
            delay(200);
            digitalWrite(BUZ, LOW);
            delay(200);
        }
        else if(adcValue>=512 && status512!=1){
            status512 =1;
            status768=0;
            digitalWrite(BUZ, HIGH);
            delay(500);
            digitalWrite(BUZ, LOW);
            delay(500);
            digitalWrite(BUZ, HIGH);
            delay(500);
            digitalWrite(BUZ, LOW);
            delay(500);
        }
        else{
            status512 = 0;
            status768=0;
            
        }
    }
}

int spiSetup(void){
    if(wiringPiSPISetup(SPI_CE_CHANNEL, SPI_SPEED) == -1){
        fprintf(stderr, "wiringPiSPISetup Failed! Error: %s\n", strerror(errno));
        return 1;
    }
}
int readAnalogData(int adcChannel){
    int adcValue = 0;
    unsigned char buffer[3] = {0};
    if(adcChannel<0||adcChannel>7)
        return -1;
    buffer[0] = 0x01;
    buffer[1] = (CHAN_CONFIG_SINGLE+adcChannel) << 4;
    digitalWrite(CS_MCP3208, LOW);
    wiringPiSPIDataRW(SPI_CE_CHANNEL, buffer, 3);
    adcValue=((buffer[1]&3)<<8)+buffer[2];
    digitalWrite(CS_MCP3208, HIGH);
    return adcValue;
}
