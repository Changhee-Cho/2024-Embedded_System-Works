#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
#include "getch.h"

#define BUZ_P_PIN 21
unsigned int melody[8] = {262, 294, 330, 349, 392, 440, 494, 523};

char getch();
int main(){
    char key;
    int num = 0;
    wiringPiSetup();
    softToneCreate(BUZ_P_PIN);
    while(1){
        if(key=getch()){
            switch(key){
                case 'a':
                    num = 0;
                    break;
                case 's':
                    num = 1;
                    break;
                case 'd':
                    num = 2;
                    break;
                case 'f':
                    num = 3;
                    break;
                case 'g':
                    num = 4;
                    break;
                case 'h':
                    num = 5;
                    break;
                case 'j':
                    num = 6;
                    break;
                case 'k':
                    num = 7;
                    break;
            }
            softToneWrite(BUZ_P_PIN, melody[num]);
            delay(125);
            softToneWrite(BUZ_P_PIN, 0);
        }
    }
    softToneStop(BUZ_P_PIN);
    return 0;
}