#include <wiringPi.h>
#include <stdio.h>

#define TRIG 4
#define ECHO 5

int main(){
    if(wiringPiSetup()==-1){
        printf("설정 에러\n");
        return -1;
    }

    int start =0, end =0;
    float distance=0, duration=0;

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    while(1){
        digitalWrite(TRIG, LOW);
        delay(500);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);

        while(digitalRead(ECHO)==LOW)
            start=micros();
        
        while(digitalRead(ECHO)==HIGH)
            end=micros();

        duration=(float)(end-start);
        duration=duration/1000000/2;
        distance=(340*duration)*100;

        printf("Distance: %3.0fcm\n",distance);
        return 0;
    }
    return 0;
}