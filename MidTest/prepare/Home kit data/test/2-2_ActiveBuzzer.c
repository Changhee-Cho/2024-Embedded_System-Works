// gcc 2-2_ActiveBuzzer.c -o 2-2 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define BUZ_A_PIN 30

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Error!\n");
        return -1;
    }

    pinMode(BUZ_A_PIN, OUTPUT);

    while(1){
        // BUZ activation
        digitalWrite(BUZ_A_PIN, HIGH);
        delay(1000);
        
        // BUZ deactivation
        digitalWrite(BUZ_A_PIN, LOW);
        delay(1000);
    }
}