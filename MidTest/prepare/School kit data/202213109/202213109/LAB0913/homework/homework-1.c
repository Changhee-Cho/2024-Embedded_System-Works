#include <stdio.h>
#include <wiringPi.h>

#define RED 27
#define GREEN 28
#define BLUE 29
#define SWITCH 0


int main(void){
    if(wiringPiSetup()==-1){
        printf("Setup Error");
        return -1;
    }
    int R_CON = 0;
    int G_CON = 0;
    int B_CON = 0;
    
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(SWITCH, INPUT);


    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    while(1){
        if(R_CON==0&&G_CON==0&&B_CON==0&&digitalRead(SWITCH)==LOW){
            digitalWrite(GREEN, HIGH);
            G_CON=1;
            delay(300);
        }
        else if(G_CON==1&&digitalRead(SWITCH)==LOW){
            digitalWrite(GREEN, LOW);
            digitalWrite(RED, HIGH);
            R_CON=1;
            G_CON=0;
            delay(300);
        }
        else if(R_CON==1&&digitalRead(SWITCH)==LOW){
            digitalWrite(RED, LOW);
            digitalWrite(BLUE, HIGH);
            R_CON=0;
            B_CON=1;
            delay(300);
        }
        else if(B_CON==1&&digitalRead(SWITCH)==LOW){
            digitalWrite(BLUE, LOW);
            delay(300);
        }
    }
}