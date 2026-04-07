#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define R_LED_PIN 27
int main(){
    if(wiringPiSetup()==-1){
        return -1;
    }
    pinMode(R_LED_PIN, SOFT_PWM_OUTPUT);

    softPwmCreate(R_LED_PIN, 0, 255);

    while(1){
        for(int i=0; i<=255; i+=5){
            softPwmWrite(R_LED_PIN, i);
            printf("RED LED ON %d\n", i);
            delay(50);
        }
    }
    return 0;
}