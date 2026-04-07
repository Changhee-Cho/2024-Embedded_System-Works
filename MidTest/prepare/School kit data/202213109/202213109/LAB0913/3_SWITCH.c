#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define PUSHL 0
#define PUSHR 7

int main(){
    if(wiringPiSetup()==-1){
        printf("ERROR");
        return -1;
    }
    pinMode(PUSHL, INPUT);
    pinMode(PUSHR, INPUT);

    while(1){
        if(digitalRead(PUSHL)==LOW){
            printf("LEFT SWITCH PUSHED\n");
            delay(300);
        }
        else if(digitalRead(PUSHR)==LOW){
            printf("RIGHT SWITCH PUSHED\n");
            delay(300);
        }
    }
}