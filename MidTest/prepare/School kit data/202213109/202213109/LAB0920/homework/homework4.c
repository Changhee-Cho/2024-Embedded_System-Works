#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

#define DC_A 6
#define DC_B 31
#define RED 27
#define GREEN 28
#define BLUE 29

char getch();
int main(){
    char key;
    if(wiringPiSetup()==-1){
        printf("ERROR");
        return -1;
    }

    pinMode(DC_A, OUTPUT);
    pinMode(DC_B, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    while(1){
            if(key=getch()){
            switch(key){
              case '+':
                digitalWrite(DC_A,HIGH);
                digitalWrite(DC_B, LOW);
                digitalWrite(RED, LOW);
                digitalWrite(GREEN, LOW);
                digitalWrite(BLUE, HIGH);
                break;
              case '-':
                digitalWrite(DC_A, LOW);
                digitalWrite(DC_B, HIGH);
                digitalWrite(RED, LOW);
                digitalWrite(GREEN, HIGH);
                digitalWrite(BLUE, LOW);
                break;
               case '0':
                digitalWrite(DC_A, LOW);
                digitalWrite(DC_B, LOW);
                digitalWrite(RED, HIGH);
                digitalWrite(GREEN, LOW);
                digitalWrite(BLUE, LOW);
                 break;
        }
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