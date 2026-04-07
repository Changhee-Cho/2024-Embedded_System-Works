#include "irpigpio.h"
#include <stdio.h>
#include <wiringPi.h>

#define IR 18
#define LED 27

#define btn0 951308750
#define btn1 271208262

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
        return 1;
    pinMode(LED, OUTPUT);
    value = readIR();
    while(1){
        value = readIR();
        switch(value){
            case btn0:
                printf("Button 0 pressed, LED On\n");
                digitalWrite(LED, HIGH);
                break;
            case btn1:
                printf("Button 1 pressed, LED Off\n");
                digitalWrite(LED, LOW);
                break;
        }
    }
    gpioTerminate();
    return 0;
}