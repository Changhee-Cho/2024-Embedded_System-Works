#include <stdio.h>
#include <wiringPi.h>
#include "TM1637.h"

#define CLK_PIN 9
#define DIO_PIN 8

int main(){
    int8_t NumTab[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int8_t ListDisp[4];
    unsigned char i=0, count =0;
    delay(150);
    if(wiringPiSetup()==-1){
        printf("Setup Error\n");
        return -1;
    }
    pinMode(CLK_PIN, INPUT);
    pinMode(DIO_PIN, INPUT);
    delay(200);
    TM1637_init(CLK_PIN, DIO_PIN);
    TM1637_set(BRIGHTEST, 0x40, 0xc0);

    while(1){
        unsigned char BitSelect = 0;
        i = count;
        count++;
        if(count == sizeof(NumTab))
            count = 0;
        for(BitSelect = 0; BitSelect<4;BitSelect++){
            ListDisp[BitSelect]=NumTab[i];
            i++;
            if(i==sizeof(NumTab))
                i = 0;
        }
        TM1637_display(0, ListDisp[0]);
        TM1637_display(1, ListDisp[1]);
        TM1637_display(2, ListDisp[2]);
        TM1637_display(3, ListDisp[3]);
        delay(500);
    }
    return 0;
}