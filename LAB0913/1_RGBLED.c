#include <stdio.h>
#include <wiringPi.h>

#define R_LED 27
#define G_LED 28
#define B_LED 29

int main(void) {
    if(wiringPiSetup() == -1){
        printf("Setup Fail ~!!");
        return -1;
    }

    pinMode(R_LED, OUTPUT);
    pinMode(G_LED, OUTPUT);
    pinMode(B_LED, OUTPUT);

    while(1){
        digitalWrite(R_LED, HIGH);
        printf("RED LED ON=n");
        delay(1000);

        digitalWrite(R_LED, LOW);
        digitalWrite(G_LED, HIGH);
        printf("GREEN LED ON\n");
        delay(1000);

        digitalWrite(G_LED, LOW);
        digitalWrite(B_LED, HIGH);
        printf("BLUE LED ON\n");
        delay(1000);

        digitalWrite(R_LED, LOW);
        digitalWrite(G_LED, LOW);
        digitalWrite(B_LED, LOW);
        printf("RGB LED OFF\n");

        delay(2000);

    }
}