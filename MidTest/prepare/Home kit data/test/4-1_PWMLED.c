//gcc 4-1_PWMLED.c -o4-1 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#define RED 27

int main(){
    wiringPiSetup();
    pinMode(RED, SOFT_PWM_OUTPUT);
    softPwmCreate(RED, 0, 255);
    while(1){
        for(int i=0; i<=255; i+=5){
            softPwmWrite(RED, i);
            printf("RED LED %d\n", i);
            delay(50);
        }
    }
    return 0;
}