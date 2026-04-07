// gcc 3-homework1.c -o 3-homework3 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define TRIG 4
#define ECHO 5
#define BUZ 30

int main(){
    wiringPiSetup();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(BUZ, OUTPUT);

    float value[10] = { 0 };
    while(1){
        getchar();
        for(int i=0; i<10; i++){
            int start = 0, end = 0;
            float duration = 0, distance = 0;

            digitalWrite(TRIG, LOW);
            delay(500);
            digitalWrite(TRIG, HIGH);
            delayMicroseconds(10);
            digitalWrite(TRIG, LOW);

            while(digitalRead(ECHO)==LOW)
                start = micros();
            while(digitalRead(ECHO)==HIGH)
                end = micros();
            duration = (float)(end-start);
            duration = duration / 1000000 / 2;
            distance = (340 * duration) * 100;
            value[i]=distance;
            printf("%d번째: %3.0f cm\n", i+1, value[i]);
        }
        float min = value[0];
        float max = value[0];
        for(int i=0; i<10; i++){
            if(min>value[i])
                min = value[i];
            if(max<value[i])
                max = value[i];
        }
        printf("Shortest: %3.0f cm\n", min);
        printf("Longest: %3.0f cm\n", max);

        if(min<=20){
            for(int i=0; i<2; i++){
                digitalWrite(BUZ, HIGH);
                delay(250);
                digitalWrite(BUZ, LOW);
                delay(250);
                digitalWrite(BUZ, HIGH);
                delay(250);
                digitalWrite(BUZ, LOW);
                delay(250);
            }
        }
        else if(min<=30){
            digitalWrite(BUZ, HIGH);
            delay(500);
            digitalWrite(BUZ, LOW);
            delay(500);
            digitalWrite(BUZ, HIGH);
            delay(500);
            digitalWrite(BUZ, LOW);
            delay(500);
        }
        else if(min<=40){
            digitalWrite(BUZ, HIGH);
            delay(2000);
            digitalWrite(BUZ, LOW);
        }
    }
    return 0;
}