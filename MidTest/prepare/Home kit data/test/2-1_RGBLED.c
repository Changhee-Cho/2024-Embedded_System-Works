// gcc 2-1_RGBLED.c -o 2-1 -lwiringPi
#include <stdio.h>
#include <wiringPi.h>

#define RED 27
#define GREEN 28
#define BLUE 29

int main(void){
    if(wiringPiSetup()==-1){
        printf("Setup Error!\n");
        return -1;
    }

    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);

    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);

    while(1)
    {
        // only RED ON
        digitalWrite(RED, HIGH);
        printf("RED LED ON\n");
        delay(1000);

        // only GREEN on
        digitalWrite(RED, LOW);
        digitalWrite(GREEN, HIGH);
        printf("GREEN LED ON\n");
        delay(1000);

        // only BLUE on
        digitalWrite(GREEN, LOW);
        digitalWrite(BLUE, HIGH);
        printf("BLUE LED ON\n");
        delay(1000);

        // all LED off
        digitalWrite(RED, LOW);
        digitalWrite(GREEN, LOW);
        digitalWrite(BLUE, LOW);
        printf("RGB LED OFF\n");
        delay(2000);
    }
    return 0;
}