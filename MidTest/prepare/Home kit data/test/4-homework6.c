//gcc 4-homework6.c -o4-homework6 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "getch.h"


#define SERVO 26
#define LED 27

char getch();

int main(){
    wiringPiSetup();
    pinMode(SERVO, SOFT_PWM_OUTPUT);
    pinMode(LED, SOFT_PWM_OUTPUT);
    softPwmCreate(SERVO, 0, 200);
    softPwmCreate(LED, 0, 18);
    
    // default 15
    softPwmWrite(SERVO, 15);
    softPwmWrite(LED, 9);
    char key;
    int angle = 15;
    int light = 9;

    while(1){
        if(key=getch()){
            switch(key){
                case 'q':
                    if(angle<24){
                        // CCW
                        angle++;
                        light++;
                        softPwmWrite(SERVO, angle);
                        softPwmWrite(LED, light);
                    }
                    break;
                case 'e':
                    if(angle>6){
                        angle--;
                        light--;
                        softPwmWrite(SERVO, angle);
                        softPwmWrite(LED, light);
                    }
                    break;
            }
        }
    }
    return 0;
}