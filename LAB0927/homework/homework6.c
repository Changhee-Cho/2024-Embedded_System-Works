#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

char getch();

#define SERVO 26
#define LED 27

int main(void){
    if(wiringPiSetup()==-1){
        printf("Setup Error!\n");
        return -1;
    }
    pinMode(SERVO, SOFT_PWM_OUTPUT);
    softPwmCreate(SERVO, 0, 200);
    softPwmCreate(LED, 0, 18);
    

    int status = 0; // 반시계방향 1, 시계 방향 2
    int angle = 0; int light = 0;

     // 초기 설정 // 15도 중간밝기(18)
    softPwmWrite(SERVO, 15);
    angle = 15;
    softPwmWrite(LED, 9);
    light = 9;
    
    char key;

    while(1){
        if(key = getch()){
            switch(key){
                case 'q':
                    status = 1;
                    break;
                case 'e':
                    status = 2;
                    break;
            }
        }
        if(status == 1){
            if(angle<24){
                angle+=1;
                light+=1;
            }
        }
        else if(status == 2){
            if(angle>6){
                angle-=1;
                light-=1;
            }
        }
        softPwmWrite(SERVO, angle);
        softPwmWrite(LED, light);
        delay(500);
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