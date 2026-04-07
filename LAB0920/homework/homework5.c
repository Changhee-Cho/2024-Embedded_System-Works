#include <stdio.h>
#include <wiringPi.h>

#define BLUE_PIN 22 //WPI
#define PINK_PIN 23 //WPI
#define YELL_PIN 24 //WPI
#define ORAG_PIN 25 //WPI
#define RED 27
#define GREEN 28
#define BLUE 29

void setStep(int _blue, int _pink, int _yell, int _orag){
    digitalWrite(BLUE_PIN, _blue);
    digitalWrite(PINK_PIN, _pink);
    digitalWrite(YELL_PIN, _yell);
    digitalWrite(ORAG_PIN, _orag);
}

int main(){
    if(wiringPiSetup()==-1){
        printf("ERROR");
        return -1;
    }
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(PINK_PIN, OUTPUT);
    pinMode(YELL_PIN, OUTPUT);
    pinMode(ORAG_PIN, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    int a = 0;

    while(1){
        printf("원하는 각도를 입력: ");
        scanf("%d",&a);
        if(a>0){
            // 시계방향
            digitalWrite(RED, HIGH);
            digitalWrite(GREEN, LOW);
            digitalWrite(BLUE, LOW);
            for(int i=0; i<(a*1.43); i++){
                setStep(HIGH, HIGH, LOW, LOW); delay(2);
                setStep(LOW, HIGH, HIGH, LOW); delay(2);
                setStep(LOW, LOW, HIGH, HIGH); delay(2);
                setStep(HIGH, LOW, LOW, HIGH); delay(2);
            }
        }
        else if(a<0){
            // 반시계방향
            digitalWrite(RED, LOW);
            digitalWrite(GREEN, LOW);
            digitalWrite(BLUE, HIGH);
            for(int i=0; i<(-1*a*1.43); i++){
                setStep(HIGH, LOW, LOW, HIGH); delay(2);
                setStep(LOW, LOW, HIGH, HIGH); delay(2);
                setStep(LOW, HIGH, HIGH, LOW); delay(2);
                setStep(HIGH, HIGH, LOW, LOW); delay(2);
            }

        }
            // 정지상태
            setStep(HIGH,HIGH,HIGH,HIGH);
            digitalWrite(RED, LOW);
            digitalWrite(GREEN, LOW);
            digitalWrite(BLUE, LOW);
    }
    return 0;
}