// gcc 3-3_StepMotor.c -o 3-3 -l wiringPi

#include <stdio.h>
#include <wiringPi.h>

#define BLUE 22
#define PINK 23
#define YELL 24
#define ORAG 25

void setStep(int b, int p, int y, int o){
    digitalWrite(BLUE, b);
    digitalWrite(PINK, p);
    digitalWrite(YELL, y);
    digitalWrite(ORAG, o);
}

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Error\n");
        return -1;
    }
    pinMode(BLUE, OUTPUT);
    pinMode(PINK, OUTPUT);
    pinMode(YELL, OUTPUT);
    pinMode(ORAG, OUTPUT);

    while(1){
        printf("CW\n");
        for(int i=0; i<512; i++){
            setStep(HIGH, HIGH, LOW, LOW);
            delay(2);
            setStep(LOW, HIGH, HIGH, LOW);
            delay(2);
            setStep(LOW, LOW, HIGH, HIGH);
            delay(2);
            setStep(HIGH, LOW, LOW, HIGH);
            delay(2);
        }
        printf("CCW\n");
        for(int i=0; i<512; i++){
            setStep(HIGH, LOW, LOW, HIGH);
            delay(2);
            setStep(LOW, LOW, HIGH, HIGH);
            delay(2);
            setStep(LOW, HIGH, HIGH, LOW);
            delay(2);
            setStep(HIGH, HIGH, LOW, LOW);
            delay(2);
        }

        printf("STOP\n");
        setStep(HIGH, HIGH, HIGH, HIGH);
        delay(500);
    }
    return 0;
}