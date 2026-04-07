#include <stdio.h>
#include <wiringPi.h>
#include <string.h>
#include <stdlib.h>

#define TRIG 4
#define ECHO 5
#define BUZ 30

int main(){
    if(wiringPiSetup()==-1){
        printf("ERROR\n");
        return -1;
    }
    pinMode(BUZ, OUTPUT);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    int start=0, end=0;
    float distance = 0, duration = 0;
    getchar();
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
        duration = (float)(end-start);
        duration = duration/1000000/2;
        distance = (340 * duration) * 100;
        printf("Distance: %.3fcm\n", distance);

        if(distance<=20){
            for(int i=0; i<3; i++){
                digitalWrite(BUZ, HIGH);
                delay(500);
                digitalWrite(BUZ, LOW);
                delay(500);
            }

        digitalWrite(TRIG, LOW);
        delay(500);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
        while(digitalRead(ECHO)==LOW)
            start=micros();
        while(digitalRead(ECHO)==HIGH)
            end=micros();
        duration = (float)(end-start);
        duration = duration/1000000/2;
        distance = (340 * duration) * 100;
        printf("Distance: %.3fcm\n", distance);

        if(distance<=20){
            char cmd[] = "sudo libcamera-jpeg --width 800 --height 800 -t 1 -o test.jpeg";
            printf("%s \n", cmd);
            system(cmd);
            printf("Take a picture \n");
        }
        while(distance>20){
            digitalWrite(TRIG, LOW);
        delay(500);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
        while(digitalRead(ECHO)==LOW)
            start=micros();
        while(digitalRead(ECHO)==HIGH)
            end=micros();
        duration = (float)(end-start);
        duration = duration/1000000/2;
        distance = (340 * duration) * 100;
        printf("Distance: %.3fcm\n", distance);
        }
    }
}
return 0;
}