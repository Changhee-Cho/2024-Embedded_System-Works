// gcc 4-3_SERVO.c -o 4-3 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define SERVO 26

int main(){
    wiringPiSetup();
    pinMode(SERVO, SOFT_PWM_OUTPUT);
    softPwmCreate(SERVO, 0, 200);
    while(1){
        // 0 angle
        softPwmWrite(SERVO, 15);
        delay(500);
        
        // -90 angle
        softPwmWrite(SERVO, 24);
        delay(500);

        // 90 angle
        softPwmWrite(SERVO, 5);
        delay(500);
    }
    softPwmStop(SERVO);
    delay(500);
    return 0;
}