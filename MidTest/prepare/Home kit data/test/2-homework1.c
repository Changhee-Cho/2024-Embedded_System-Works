// gcc 2-homework1.c -o 2-homework1 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define RED 27
#define GREEN 28
#define BLUE 29

#define LEFT 0

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Error!\n");
        return -1;
    }

    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);

    int status = 0;
    // 0:off, 1: GREEN on, 2: RED on, 3: BLUE on

    while(1){
        if(digitalRead(LEFT)==LOW){
            status++;
            if(status==4)
                status = 0;
            switch(status){
                case 0:
                    digitalWrite(RED, LOW);
                    digitalWrite(GREEN, LOW);
                    digitalWrite(BLUE, LOW);
                    break;
                case 1:
                    digitalWrite(GREEN, HIGH);
                    break;
                case 2:
                    digitalWrite(GREEN, LOW);
                    digitalWrite(RED, HIGH);
                    break;
                case 3:
                    digitalWrite(RED, LOW);
                    digitalWrite(BLUE, HIGH);
                    break;
            }
            delay(300);
        }
    }
    return 0;
}