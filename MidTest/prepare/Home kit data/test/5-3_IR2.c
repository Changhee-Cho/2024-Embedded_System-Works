// gcc 5-3_IR1.c -o 5-3 irpigpio.c -lpthread -lwiringPi -lpigpio
// sudo 로 실행시켜야 함

#include "irpigpio.h"
#include <stdio.h>
#include <wiringPi.h>

#define IR 18
#define LED 27
#define BUTTON_0 951308750
#define BUTTON_1 271208262

int deviceSetup(){
    if(gpioInitialise()<0 || wiringPiSetup()<0){
        return -1;
    }
    gpioSetMode(IR, PI_INPUT);
    gpioSetWatchdog(IR, 5);
    gpioSetAlertFunc(IR, alert);
}

int main(){
    long value;
    if(deviceSetup()<0)
        return -1;
    pinMode(LED, OUTPUT);
    while(1){
        value = readIR();
        switch(value){
            case BUTTON_0:
                printf("Button 0 Pressed, LED ON\n");
                digitalWrite(LED, HIGH);
                break;
            case BUTTON_1:
                printf("Button 1 Pressed, LED Off\n");
                digitalWrite(LED, LOW);
                break;
        }
    }
    gpioTerminate();
}