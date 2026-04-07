#include <wiringPi.h>
#include <stdio.h>

#define DC_A 6
#define DC_B 31


int main(){
    if(wiringPiSetup()==-1){
    printf("ERROR");
    return -1;
}
pinMode(DC_A, OUTPUT);
pinMode(DC_B, OUTPUT);
while(1){
    // 반시계방향(CCW)
    digitalWrite(DC_A, HIGH);
    digitalWrite(DC_B, LOW);
    printf("반시계방향\n");
    delay(1000);

    // 시계방향(CW)
    digitalWrite(DC_A, LOW);
    digitalWrite(DC_B, HIGH);
    printf("시계방향\n");
    delay(1000);

    digitalWrite(DC_A, LOW);
    digitalWrite(DC_B, LOW);
    printf("정지\n");
    delay(1000);
}
return 0;
}