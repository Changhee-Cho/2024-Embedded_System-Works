#include <wiringPi.h>
#include <stdio.h>

#define BUZ 30

int main(void){
    if(wiringPiSetup()==-1){
        printf("Setup Error");
        return -1;
    }

pinMode(BUZ, OUTPUT);

while(1){
    digitalWrite(BUZ, HIGH);
    delay(1000);

    digitalWrite(BUZ, LOW);
    delay(1000);
}
}