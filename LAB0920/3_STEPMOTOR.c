#include <wiringPi.h>
#include <stdio.h>

#define BLUE_PIN 22
#define PINK_PIN 23
#define YELL_PIN 24
#define ORAG_PIN 25

void setStep(int blue, int pink, int yell, int orag){
    digitalWrite(BLUE_PIN, blue);
    digitalWrite(PINK_PIN, pink);
    digitalWrite(YELL_PIN, yell);
    digitalWrite(ORAG_PIN, orag);
}

int main(){
    if(wiringPiSetup()==-1){
        printf("ERROR\n");
        return -1;
    }
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(PINK_PIN, OUTPUT);
    pinMode(YELL_PIN, OUTPUT);
    pinMode(ORAG_PIN, OUTPUT);

    while(1){
        printf("CW\n");
        //시계반향
        for(int i=0; i<512; i++){
            setStep(HIGH, HIGH, LOW, LOW);  delay(2);
            setStep(LOW, HIGH, HIGH, LOW);  delay(2);
            setStep(LOW, LOW, HIGH, HIGH);  delay(2);
            setStep(HIGH, LOW, LOW, HIGH);  delay(2);
        }
        printf("CCW\n");
        for(int i=0; i<512; i++){
            setStep(HIGH, LOW, LOW, HIGH); delay(2);
            setStep(LOW, LOW, HIGH, HIGH); delay(2);
            setStep(LOW, HIGH, HIGH, LOW); delay(2);
            setStep(HIGH, HIGH, LOW, LOW); delay(2);
        }
        printf("----stop\n");
        setStep(HIGH, HIGH, HIGH, HIGH);
        delay(500);
    }

    return 0;
}