#include <stdio.h>
#include <wiringPi.h>


#define TRIG 4
#define ECHO 5
#define BUZ 30

int main(){
    if(wiringPiSetup()==-1){
        printf("ERROR");
        return -1;
    }
    
    int start=0, end=0;
    float distance=0, duration=0;

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(BUZ, OUTPUT);
    
    char enter;
    float dis[10];

    printf("엔터시 측정");
    getchar();
        for(int i=0; i<10; i++){
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
            distance=(340*duration)*100;

            dis[i]=distance;
            printf("%d회차 거리값: %fcm\n", i+1, dis[i]);
            delay(500);
        }
        
        float min = 500000000;
        float max = 0;

        for(int i=0; i<10; i++){
            if (min>dis[i])
                min=dis[i];
            if (max<dis[i])
                max=dis[i];
        }
        printf("최단 거리: %fcm\n", min);
        printf("최장 거리: %fcm\n", max);
        if(min<=20){
            for(int j=0; j<2; j++){
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
    return 0;
}