#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>

#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define BUZ 21

char getch();



int main(void){
    if(wiringPiSetup()==-1){
        printf("Setup Error!\n");
        return -1;
    }
    pinMode(BUZ, SOFT_TONE_OUTPUT);
    softToneCreate(BUZ);
    softToneWrite(BUZ, 0);
    
    
    char key;
    unsigned int melody[8] = {262, 294, 330, 349, 392, 440, 494, 523};
    int status;
    while(1){
        if(key = getch()){
            switch(key){
                // 도
                case 'a':
                    softToneWrite(BUZ, melody[0]);
                    delay(125);
                    break;
                // 레
                case 's':
                    softToneWrite(BUZ, melody[1]);
                    delay(125);
                    break;
                // 미
                case 'd':
                    softToneWrite(BUZ, melody[2]);
                    delay(125);
                    break;
                // 파
                case 'f':
                    softToneWrite(BUZ, melody[3]);
                    delay(125);
                    break;
                // 솔
                case 'g':
                    softToneWrite(BUZ, melody[4]);
                    delay(125);
                    break;
                // 라
                case 'h':
                    softToneWrite(BUZ, melody[5]);
                    delay(125);
                    break;
                // 시
                case 'j':
                    softToneWrite(BUZ, melody[6]);
                    delay(125);
                    break;
                // 도
                case 'k':
                    softToneWrite(BUZ, melody[7]);
                    delay(125);
                    break;
            }
            softToneWrite(BUZ, 0);
        }
    

    }

    softToneStop(BUZ);

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