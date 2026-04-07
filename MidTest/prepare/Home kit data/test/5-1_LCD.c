// gcc 5-1_LCD.c -o 5-1 -lwiringPi -lwiringPiDev

#include <stdio.h>
#include <wiringPi.h>
#include <pcf8574.h>
#include <lcd.h>

#define BASE_ADDR 100
#define RS_PIN BASE_ADDR // 100
#define RW_PIN BASE_ADDR+1 // 101
#define EN_PIN BASE_ADDR+2 // 102
#define BL_PIN BASE_ADDR+3 // 103
#define D4_PIN BASE_ADDR+4 // 104
#define D5_PIN BASE_ADDR+5 // 105
#define D6_PIN BASE_ADDR+6 // 106
#define D7_PIN BASE_ADDR+7 // 107

int main(){
    wiringPiSetup();
    pcf8574Setup(RS_PIN, 0x27);
    pinMode(RW_PIN, OUTPUT);
    digitalWrite(RW_PIN, LOW);
    pinMode(BL_PIN, OUTPUT);
    digitalWrite(BL_PIN, HIGH);
    int fd = lcdInit(2, 16 , 4, RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN, 0, 0, 0, 0);
    
    while(1){
        lcdClear(fd);
        delay(500);
        lcdPosition(fd, 2, 0);
        lcdPuts(fd, "Hello World!");
        lcdPosition(fd, 2, 1);
        lcdPuts(fd, "* Good Luck *");
        delay(2000);
    }
    lcdClear(fd);
    delay(500);
    return 0;
}