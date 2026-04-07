
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <softTone.h>
#include <unistd.h>
#include <termios.h>

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
char getch();

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Fail\n");
        return -1;
    }
    char key;
    softToneCreate(BUZ);
    spiSetup();
    pinMode(CS_MCP3208, OUTPUT);
    int adcValue = 0;

    getchar();
    int tone = 0;
    while(1){
        //변환을 해 줘야 하는데, 0일때 200 주파수, 오른쪽 끝 700 주파수;
        adcValue =readAnalogData(SPI_ADC_POTEN_CHANNEL);
        tone = adcValue/2+200;
        printf("Potentionmeter value = %d, tone = %d\n", adcValue, tone);
        
        if(key=getch()){
            switch(key){
                case 'q':
                    softToneWrite(BUZ, tone);
                    digitalWrite(LED, HIGH);
                    delay(200);
                    softToneWrite(BUZ, 0);
                    digitalWrite(LED, LOW);
                    break;
                case 'w':
                    softToneWrite(BUZ, tone);
                    digitalWrite(LED, HIGH);
                    break;
                case 'r':
                    softToneWrite(BUZ, 0);
                    digitalWrite(LED, LOW);
                    break;
            }
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

char getch(){
    char c;
    struct termios oldattr, newattr;
    tcgetattr(STDIN_FILENO, &oldattr); // now terminal config
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO); // turn off CANONICAL and ECHO
    newattr.c_cc[VMIN] = 1; // least letter count 1
    newattr.c_cc[VTIME] = 0; // least read wating time 0
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // input terminal config
    c = getchar(); // read keyboard
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // recover original coonfig
    return c;
}