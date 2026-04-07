// gcc 4-4_PassiveBuzzer.c -o 4-4 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>

#define BUZ_P_PIN 21
unsigned int melody[8] = {262, 294, 330, 349, 392, 440, 494, 523};

int main(){
    int idx = 0;
    wiringPiSetup();

    pinMode(BUZ_P_PIN, SOFT_TONE_OUTPUT);
    softToneCreate(BUZ_P_PIN);
    while(1){
        for(int i=0; i<8; i++){
            softToneWrite(BUZ_P_PIN, melody[i]);
            delay(1000);
        }
    }
    softToneStop(BUZ_P_PIN);
    return 0;
}