// gcc 3-homework4.c -o3-homework4 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>
#include "getch.h"

#define DC_A 6
#define DC_B 31
#define RED 27
#define GREEN 28
#define BLUE 29

char getch();

int main(){
    wiringPiSetup();
    pinMode(DC_A, OUTPUT);
    pinMode(DC_B, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);

    digitalWrite(DC_A, LOW);
    digitalWrite(DC_B, LOW);
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);


    char key;
    while(1){
        if(key=getch()){
            digitalWrite(RED, LOW);
            digitalWrite(GREEN, LOW);
            digitalWrite(BLUE, LOW);
            switch(key){
                case '+':
                    // CW
                    digitalWrite(DC_A, LOW);
                    digitalWrite(DC_B, HIGH);
                    digitalWrite(BLUE, HIGH);
                    break;
                case '-':
                    // CCW
                    digitalWrite(DC_A, HIGH);
                    digitalWrite(DC_B, LOW);
                    digitalWrite(GREEN, HIGH);
                    break;
                case '0':
                    // STOP
                    digitalWrite(DC_A, LOW);
                    digitalWrite(DC_B, LOW);
                    digitalWrite(RED, HIGH);
                    break;
            }
        }
    }
    return 0;
}