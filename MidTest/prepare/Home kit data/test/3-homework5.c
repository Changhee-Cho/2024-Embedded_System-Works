// gcc 3-homework5.c -o 3-homework5 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define BLUE 22
#define PINK 23
#define YELL 24
#define ORAG 25

#define RED 27
#define BLED 29

char getch();

void setStep(int b, int p, int y, int o);

int main(){
    wiringPiSetup();
    pinMode(BLUE, OUTPUT);
    pinMode(PINK, OUTPUT);
    pinMode(YELL, OUTPUT);
    pinMode(ORAG, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(BLED, OUTPUT);

    digitalWrite(RED, LOW);
    digitalWrite(BLED, LOW);
   
    while(1){
        float angle;
        scanf("%f", &angle);
        // if angle > 0 natural number
        // CW
        if(angle>0){
            digitalWrite(RED, HIGH);
            angle = angle * 1.422222222222222222222222;
            for(int i=0; i<angle; i++){
                setStep(HIGH, HIGH, LOW, LOW);
                delay(2);
                setStep(LOW, HIGH, HIGH, LOW);
                delay(2);
                setStep(LOW, LOW, HIGH, HIGH);
                delay(2);
                setStep(HIGH, LOW, LOW, HIGH);
                delay(2);
            }
        }
        // angle < 0 minus number
        else{
            digitalWrite(BLED, HIGH);
            angle = angle * 1.422222222222222222222222 * -1;
            for(int i=0; i<angle; i++){
                setStep(HIGH, LOW, LOW, HIGH);
                delay(2);
                setStep(LOW, LOW, HIGH, HIGH);
                delay(2);
                setStep(LOW, HIGH, HIGH, LOW);
                delay(2);
                setStep(HIGH, HIGH, LOW, LOW);
                delay(2);
            }
        }
        digitalWrite(RED, LOW);
        digitalWrite(BLED, LOW);
    }
        return 0;
}

void setStep(int b, int p, int y, int o){
    digitalWrite(BLUE, b);
    digitalWrite(PINK, p);
    digitalWrite(YELL, y);
    digitalWrite(ORAG, o);
}