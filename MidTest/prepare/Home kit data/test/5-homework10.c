//  gcc 5-homework10.c -o homework10 -lwiringPi -lwiringPiDev irpigpio.c -lpthread -lpigpio
#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
#include <pcf8574.h>
#include <lcd.h>
#include "irpigpio.h"

#define BASE_ADDR 100
#define RS_PIN BASE_ADDR
#define RW_PIN BASE_ADDR+1
#define EN_PIN BASE_ADDR+2
#define BL_PIN BASE_ADDR+3
#define D4_PIN BASE_ADDR+4
#define D5_PIN BASE_ADDR+5
#define D6_PIN BASE_ADDR+6
#define D7_PIN BASE_ADDR+7

#define IR 18
#define BUZ 21

#define BUTTON_1 951308750
#define BUTTON_2 271208262
unsigned int melody[8] = {262, 294, 330, 349, 392, 440, 494, 523};

int main(){
    wiringPiSetup();

    pinMode(BUZ, SOFT_TONE_OUTPUT);

    
    if(gpioInitialise()==0)
        return -1;
    gpioSetMode(IR, PI_INPUT);
    gpioSetWatchdog(IR, 5);
    gpioSetAlertFunc(IR, alert);

    pcf8574Setup(RS_PIN, 0x27);
    pinMode (RW_PIN, OUTPUT);
    digitalWrite (RW_PIN, LOW);
    pinMode (BL_PIN, OUTPUT);
    digitalWrite (BL_PIN, HIGH);
    int fd = lcdInit (2, 16, 4, RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN,
    D7_PIN, 0,0,0,0 );



    long value;
    int status = 0; int logic = 0;
    while(1){
        value = readIR();
        if(value==BUTTON_1)
            status = 1;
        else if(value == BUTTON_2)
            status = 2;
        if(status > 0){
            lcdClear(fd);
            delay(500);
            lcdPosition(fd, 0, 0);
            if(status == 1 && logic!=1){
                lcdPuts(fd, "Open the door");
                logic=1;
                
                softToneWrite(BUZ, melody[0]);
                delay(500);
                softToneWrite(BUZ, melody[2]);
                delay(500);
                softToneWrite(BUZ, melody[4]);
                delay(500);
            }
            else if(status == 2 && logic !=2){
                lcdPuts(fd, "Close the door");
                logic=2;

                
                softToneWrite(BUZ, melody[4]);
                delay(500);
                softToneWrite(BUZ, melody[2]);
                delay(500);
                softToneWrite(BUZ, melody[0]);
                delay(500);
            }
            softToneWrite(BUZ, 0);
            delay(1000);
            lcdClear(fd);
        }

    }

    return 0;
}