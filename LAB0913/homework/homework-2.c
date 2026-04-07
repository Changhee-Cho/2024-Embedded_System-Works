#include <stdio.h>
#include <wiringPi.h>

#define BUZ 30
#define SWITCH 0
#define SWITCH2 7

int main(void){
    if(wiringPiSetup()==-1){
        printf("Setup Error");
        return -1;
    }

    pinMode(BUZ, OUTPUT);
    pinMode(SWITCH, INPUT);
    pinMode(SWITCH2, INPUT);
    while(1){

    digitalWrite(BUZ, LOW);
    if(digitalRead(SWITCH)==LOW){
        digitalWrite(BUZ, HIGH);
        delay(300);
    }
    else if(digitalRead(SWITCH2)==LOW){
        digitalWrite(BUZ, LOW);
        delay(300);
    }
    }
}