// gcc 3-2_DCMotor.c -o 3-2 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define DC_A 6
#define DC_B 31

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Error\n");
        return -1;
    }
    pinMode(DC_A, OUTPUT);
    pinMode(DC_B, OUTPUT);

    while(1){
        // CW
        digitalWrite(DC_A, LOW);
        digitalWrite(DC_B, HIGH);
        printf("Motor run CW\n");
        delay(1000);

        // CCW
        digitalWrite(DC_A, HIGH);
        digitalWrite(DC_B, LOW);
        printf("Motor run CCW\n");
        delay(1000);

        // Stop
        digitalWrite(DC_A, LOW);
        digitalWrite(DC_B, LOW);
        printf("Motor run STOP\n");
        delay(1000);
    }
    return 0;
}