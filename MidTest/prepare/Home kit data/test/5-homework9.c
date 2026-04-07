// gcc homework9.c –o 9 TM1637.c -lwiringPi -fcommon

#include <stdio.h>
#include <wiringPi.h>
#include "TM1637.h"
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define CLK 9
#define DIO 8
#define LED 27

char getch();

int main(void){
    wiringPiSetup();
    pinMode(CLK, INPUT);
    pinMode(DIO, INPUT);
    pinMode(LED, OUTPUT);
    delay(200);
    TM1637_init(CLK, DIO);
    TM1637_set(BRIGHTEST, 0x40, 0xc0);

    int8_t NumTab[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int8_t ListDisp[4];

    char key;
    int sum;
    int status = 0; // 0은 출력x 1은 출력
    while(1){
        if(key=getch()){
            
            int a;
            int b;

            switch(key){
                case 'q':
                    status = 0;
                    printf("첫번째 양의 정수: ");
                    scanf("%d", &a);
                    printf("두번째 양의 정수: ");
                    scanf("%d", &b);
                    if(a<0&&b<0){
                        status = 0;
                        break;
                    }
                    if(a+b>0){
                        sum=a+b;
                        ListDisp[0]=sum/1000;
                        ListDisp[1]=sum/100%10;
                        ListDisp[2]=sum/10%10;
                        ListDisp[3]=sum%10;
                        status = 1;
                    }
                    else{
                        status = 0;
                        break;
                    }
                    break;
                case 'e':
                    if(status==1){
                        while(sum>0){
                            sum--;
                            ListDisp[0]=sum/1000;
                            ListDisp[1]=sum/100%10;
                            ListDisp[2]=sum/10%10;
                            ListDisp[3]=sum%10;
                            TM1637_display(0,ListDisp[0]);
                            TM1637_display(1,ListDisp[1]);
                            TM1637_display(2,ListDisp[2]);
                            TM1637_display(3,ListDisp[3]);
                            delay(1000);
                            if(sum==0){
                                digitalWrite(LED, HIGH);
                                delay(1000);
                                digitalWrite(LED, LOW);
                            }
                        }
                        status=0;
                    }
                    break;
            }
        }
        // fnd 출력
        if(status == 1){
            TM1637_display(0,ListDisp[0]);
            TM1637_display(1,ListDisp[1]);
            TM1637_display(2,ListDisp[2]);
            TM1637_display(3,ListDisp[3]);
        }
    }
    return 0;
}

char getch(){
    char c;
    struct termios oldattr, newattr;
    tcgetattr(STDIN_FILENO, &oldattr); // now terminal config
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO); // turn off CANONICAL and ECHO
    newattr.c_cc[VMIN] = 1; // least letter count 1
    newattr.c_cc[VTIME] = 0; // least read wating time 0
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // input terminal config
    c = getchar(); // read keyboard
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // recover original coonfig
    return c;
}