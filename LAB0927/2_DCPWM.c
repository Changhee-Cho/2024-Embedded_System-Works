#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>

#define DC_A 6
#define DC_B 31

int main(){
    wiringPiSetup();

    pinMode(DC_A, SOFT_PWM_OUTPUT);
    pinMode(DC_B, SOFT_PWM_OUTPUT);

    softPwmCreate(DC_A, 0, 100);
    softPwmCreate(DC_B, 0, 100);

    int duty = 10;
    int updown = 10;

    while(1){
        if(duty>90)
            updown = -10;
        if (duty<20)
            updown = 10;
        duty += updown;
        printf("Motor duty= %d\n", duty);

        softPwmWrite(DC_A, duty);
        softPwmWrite(DC_B, 0);
        printf("Motor run CCW\n");
        delay(1000);

        softPwmWrite(DC_A, 0);
        softPwmWrite(DC_B, 0);
        printf("Motor Stop\n");
        delay(1000);

        softPwmWrite(DC_A, 0);
        softPwmWrite(DC_B, duty);
        printf("Motor run CW\n");
        delay(1000);

        softPwmWrite(DC_A, 0);
        softPwmWrite(DC_B, 0);
        printf("Motor Stop\n");
        delay(1000);
    }
    return 0;
}