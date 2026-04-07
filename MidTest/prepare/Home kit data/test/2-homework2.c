// gcc 2-homework2.c -o 2-homework2 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define BUZ 30
#define LEFT 0
#define RIGHT 7

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Error\n");
        return -1;
    }
    
    pinMode(BUZ, OUTPUT);
    pinMode(LEFT, INPUT);
    pinMode(RIGHT, INPUT);
    
    digitalWrite(BUZ, LOW);

    while(1){
        if(digitalRead(LEFT)==LOW){
            digitalWrite(BUZ, HIGH);
            delay(300);
        }
        else if(digitalRead(RIGHT)==LOW){
            digitalWrite(BUZ, LOW);
            delay(300);
        }
    }
    return 0;
}