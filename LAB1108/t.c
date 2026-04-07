#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
int main(){
    wiringPiSetup();
    softToneCreate(21);
    softToneWrite(21, 399);
    delay(500);
    softToneStop(2);
}