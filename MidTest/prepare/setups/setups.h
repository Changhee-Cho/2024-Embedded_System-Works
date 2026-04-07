#include <stdio.h>
#include <wiringPi.h>
#include <pcf8574.h>
#include <lcd.h>
#include "irpigpio.h"
#include <wiringPiSPI.h>
#include <errno.h>
#include <string.h>
#include "TM1637.h"

/*
    //LCD
    int LCDINIT();
    //FND
    int FNDINIT();
    /IR
    int IRSETUP();
    // SPI
    int SPISETUP(void);
    int readAnalogData(int adcChannel);
*/

//FND
#define CLK_PIN 9
#define DIO_PIN 8
//IR
#define IR 18
//SPI-Potentiometer
#define CHAN_CONFIG_SINGLE 8
#define SPI_SPEED 1000000
#define SPI_CE_CHANNEL 1
#define SPI_ADC_POTEN_CHANNEL 1
#define CS_MCP3208 11


// For LCD
#define BASE_ADDR 100
#define RS_PIN BASE_ADDR // 100
#define RW_PIN BASE_ADDR+1 // 101
#define EN_PIN BASE_ADDR+2 // 102
#define BL_PIN BASE_ADDR+3 // 103
#define D4_PIN BASE_ADDR+4 // 104
#define D5_PIN BASE_ADDR+5 // 105
#define D6_PIN BASE_ADDR+6 // 106
#define D7_PIN BASE_ADDR+7 // 107

int LCDINIT(){
    pcf8574Setup(RS_PIN, 0x27);
    pinMode(RW_PIN, OUTPUT);
    digitalWrite(RW_PIN, LOW);
    pinMode(BL_PIN, OUTPUT);
    digitalWrite(BL_PIN, HIGH);
    int fd = lcdInit(2, 16 , 4, RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN, 0, 0, 0, 0);
    return fd;

    // In Main Code
    /*
    int fd = LCDINIT();
    lcdClear(fd);
    lcdPosition(fd, 2, 0); --fd, x, y
    lcdPuts(fd, "Hello World!");
    */

   // gcc a.c -o a -lwiringPi -lwiringPiDev
}

// For 4FND
/*
int FNDINIT(){
    pinMode(CLK_PIN, INPUT);
    pinMode(DIO_PIN, INPUT);
    delay(200);
    TM1637_init(CLK_PIN, DIO_PIN);
    TM1637_set(BRIGHTEST, 0x40, 0xc0);
    */
    // In Main Code
    /*
    FNDINIT();
    TM1637_display(0, ListDisp[0]);
    TM1637_display(1, ListDisp[0]);
    TM1637_display(2, ListDisp[0]);
    TM1637_display(3, ListDisp[0]);
    */
/*
    // gcc a.c -o a TM1637.c -lwiringPi -fcommon
    return 0;
}
*/


// For IR
int IRSETUP(){
    gpioInitialise();
    gpioSetMode(IR, PI_INPUT);
    gpioSetWatchdog(IR, 5);
    gpioSetAlertFunc(IR, alert);
    return 0;

    // In Main Code
    /*
    IRSETUP();
    long value;
    value=readIR();
    */
   // gcc 5-3_IR1.c -o 5-3 irpigpio.c -lpthread -lwiringPi -lpigpio
}

// FOR SPI setup
int spiSetup(void){
    if(wiringPiSPISetup(SPI_CE_CHANNEL, SPI_SPEED) == -1){
        fprintf(stderr, "wiringPiSPISetup Failed! Error: %s\n", strerror(errno));
        return 1;
    }
}
int readAnalogData(int adcChannel){
    int adcValue = 0;
    unsigned char buffer[3] = {0};
    if(adcChannel<0||adcChannel>7)
        return -1;
    buffer[0] = 0x01;
    buffer[1] = (CHAN_CONFIG_SINGLE+adcChannel) << 4;
    digitalWrite(CS_MCP3208, LOW);
    wiringPiSPIDataRW(SPI_CE_CHANNEL, buffer, 3);
    adcValue=((buffer[1]&3)<<8)+buffer[2];
    digitalWrite(CS_MCP3208, HIGH);
    return adcValue;
}

// For SPI Setup
int SPISETUP(void){
    spiSetup();
    pinMode(CS_MCP3208, OUTPUT);
    return 0;
    // In Main Code
    /*
    SPISETUP();
    adcValue = readAnalogData(POTENTIAL: 1, CDS: 0);
    */
}
