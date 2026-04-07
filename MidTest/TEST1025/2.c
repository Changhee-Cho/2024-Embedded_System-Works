#include <stdio.h>
#include <wiringPi.h>
#include <pcf8574.h>
#include <lcd.h>
#include <errno.h>
#include <string.h>

#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define BASE_ADDR 100
#define RS_PIN BASE_ADDR // 100
#define RW_PIN BASE_ADDR+1 // 101
#define EN_PIN BASE_ADDR+2 // 102
#define BL_PIN BASE_ADDR+3 // 103
#define D4_PIN BASE_ADDR+4 // 104
#define D5_PIN BASE_ADDR+5 // 105
#define D6_PIN BASE_ADDR+6 // 106
#define D7_PIN BASE_ADDR+7 // 107

char getch();
int main(){
	wiringPiSetup();
    pcf8574Setup(RS_PIN, 0x27);
    pinMode(RW_PIN, OUTPUT);
    digitalWrite(RW_PIN, LOW);
    pinMode(BL_PIN, OUTPUT);
    digitalWrite(BL_PIN, HIGH);
    int fd = lcdInit(2, 16 , 4, RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN, 0, 0, 0, 0);
    char a[50];
    char b[50];

			getchar();
			printf("first string: ");
			scanf("%s", &a);
			printf("second string: ");
			scanf("%s", &b);   
			printf("%s\n", a);
			printf("%s\n", b);
	char key;
    while(1){
			if(key=getch()){
				switch(key){
					case 'q':
						lcdClear(fd);
						lcdPosition(fd, 0,0);
						lcdPuts(fd, a);
						lcdPosition(fd, 0,1);
						lcdPuts(fd, b);
						break;
					case 'e':
						lcdClear(fd);
						break;
					case 'r':
						printf("first string: ");
						scanf("%s", &a);
						printf("second string: ");
						scanf("%s", &b);   
						printf("%s\n", a);
						printf("%s\n", b);
						 while(1){
			if(key=getch()){
				switch(key){
					case 'q':
						lcdClear(fd);
						lcdPosition(fd, 0,0);
						lcdPuts(fd, a);
						lcdPosition(fd, 0,1);
						lcdPuts(fd, b);
						break;
					case 'e':
						lcdClear(fd);
						break;
					case 'r':
						getchar();
						printf("first string: ");
						scanf("%s", &a);
						printf("second string: ");
						scanf("%s", &b);   
						printf("%s\n", a);
						printf("%s\n", b);
						break;
					}
				}
	}
						break;
					}
				}
	}
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
