// gcc 2-3_PushSwitch.c -o 2-3 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define PUSHSWL 0
#define PUSHSWR 7

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Error\n");
        return -1;
    }

    pinMode(PUSHSWL, INPUT);
    pinMode(PUSHSWR, INPUT);
    
    while(1){
        if(digitalRead(PUSHSWL)==LOW){
            printf("Left Switch Pushed!\n");
            delay(300);
        }
        else if(digitalRead(PUSHSWR)==LOW){
            printf("Right Switch Pushed!\n");
            delay(300);
        }
    }
    return 0;
}