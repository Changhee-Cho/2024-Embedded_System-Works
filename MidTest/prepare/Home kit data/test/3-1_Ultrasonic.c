// gcc 3-1_Ultrasonic.c -o 3-1 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define TRIG 4
#define ECHO 5

int main(){
    int start = 0, end = 0;
    float distance = 0, duration = 0;

    if(wiringPiSetup()==-1){
        printf("Setup Error\n");
        return -1;
    }

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    while(1){
        digitalWrite(TRIG, LOW);
        delay(500);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);

        while(digitalRead(ECHO)==LOW)
            start = micros();
        while(digitalRead(ECHO)==HIGH)
            end = micros();
        
        duration = (float)(end - start);
        duration = duration / 1000000 / 2;
        distance = (duration * 340) * 100;

        printf("Distance: %3.0f cm \n", distance);
    }
    return 0;
}