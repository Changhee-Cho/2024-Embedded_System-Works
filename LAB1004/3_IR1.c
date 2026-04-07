#include "irpigpio.h"
#include <stdio.h>
#include <wiringPi.h>

#define IR 18
#define LED 27

int deviceSetup(){
    if(gpioInitialise()<0 || wiringPiSetup() < 0){
        return -1;
    }
    gpioSetMode(IR, PI_INPUT);
    gpioSetWatchdog(IR, 5);
    gpioSetAlertFunc(IR, alert);
}

int main(){
    long value;
    if(deviceSetup()<0){
        return 1;
    }
    pinMode(LED, OUTPUT);
    while(1){
        value=readIR();
        printf("IR value %u\n", value);
    }
    gpioTerminate();
}