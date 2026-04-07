#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <softPwm.h>

#define DC1_A 1
#define DC1_B 4
#define DC2_A 5
#define DC2_B 6

#define FRONT_TRIG 28
#define FRONT_ECHO 29
#define RIGHT_TRIG 26
#define RIGHT_ECHO 27
#define LEFT_TRIG 10
#define LEFT_ECHO 11

char getch();

PI_THREAD(front){
    while(1){
        int start = 0, end =0;
        float distance = 0, duration = 0;

        digitalWrite(FRONT_TRIG, LOW);
        delay(500);
        digitalWrite(FRONT_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(FRONT_TRIG, LOW);

        while(digitalRead(FRONT_ECHO)==LOW)
            start=micros();
        
        while(digitalRead(FRONT_ECHO)==HIGH)
            end=micros();

        duration=(float)(end-start);
        duration=duration/1000000/2;
        distance=(340*duration)*100;

        printf("정면: %3.0fcm\n",distance);
        delay(2000);
    }
}
PI_THREAD(left){
    while(1){
        int start = 0, end =0;
        float distance = 0, duration = 0;

        digitalWrite(LEFT_TRIG, LOW);
        delay(500);
        digitalWrite(LEFT_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(LEFT_TRIG, LOW);

        while(digitalRead(LEFT_ECHO)==LOW)
            start=micros();
        
        while(digitalRead(LEFT_ECHO)==HIGH)
            end=micros();

        duration=(float)(end-start);
        duration=duration/1000000/2;
        distance=(340*duration)*100;

        printf("좌측: %3.0fcm\n",distance);
        delay(2000);
    }
}
PI_THREAD(right){
    while(1){
        int start = 0, end =0;
        float distance = 0, duration = 0;

        digitalWrite(RIGHT_TRIG, LOW);
        delay(500);
        digitalWrite(RIGHT_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(RIGHT_TRIG, LOW);

        while(digitalRead(RIGHT_ECHO)==LOW)
            start=micros();
        
        while(digitalRead(RIGHT_ECHO)==HIGH)
            end=micros();

        duration=(float)(end-start);
        duration=duration/1000000/2;
        distance=(340*duration)*100;

        printf("우측: %3.0fcm\n",distance);
        delay(2000);
    }
}

int main(){
    wiringPiSetup();
    piThreadCreate(front);
    piThreadCreate(left);
    piThreadCreate(right);
    pinMode(DC1_A, SOFT_PWM_OUTPUT);
    pinMode(DC1_B, SOFT_PWM_OUTPUT);
    pinMode(DC2_A, SOFT_PWM_OUTPUT);
    pinMode(DC2_B, SOFT_PWM_OUTPUT);

    pinMode(FRONT_TRIG, OUTPUT);
    pinMode(FRONT_ECHO, INPUT);
    pinMode(RIGHT_TRIG, OUTPUT);
    pinMode(RIGHT_ECHO, INPUT);
    pinMode(LEFT_TRIG, OUTPUT);
    pinMode(LEFT_ECHO, INPUT);
    softPwmCreate(DC1_A, 0, 100);
    softPwmCreate(DC1_B, 0, 100);
    softPwmCreate(DC1_A, 0, 100);
    softPwmCreate(DC2_B, 0, 100);

    int speed = 50;
    int drive = 0;
    int status = 0;
    // 0: 정지 1: 전진 2: 후진 3: 좌회전 4: 우회전
    
    char key;
    while(1){
        if(key=getch()){
            switch(key){
                case 'q':
                    softPwmWrite(DC1_A, 0);
                    softPwmWrite(DC1_B, 0);
                    softPwmWrite(DC2_A, 0);
                    softPwmWrite(DC2_B, 0);
                    drive = 0;
                    status = 0;
                    break;
                case 'e':
                    if(speed>0){
                        speed -= 10;
                        switch(status){
                            case 0:
                                softPwmWrite(DC1_A, 0);
                                softPwmWrite(DC1_B, 0);
                                softPwmWrite(DC2_A, 0);
                                softPwmWrite(DC2_B, 0);
                                drive = 0;
                                status = 0;
                                break;
                                //정지
                            case 1:
                                drive=1;
                                status = 1;
                                softPwmWrite(DC1_A, speed);
                                softPwmWrite(DC1_B, 0);
                                softPwmWrite(DC2_A, speed);
                                softPwmWrite(DC2_B, 0);
                                break;
                                //전진
                            case 2:
                                drive=1;
                                status=2;
                                softPwmWrite(DC1_A, 0);
                                softPwmWrite(DC1_B, speed);
                                softPwmWrite(DC2_A, 0);
                                softPwmWrite(DC2_B, speed);
                                break;
                                //후진
                            case 3:
                                //좌회전
                                status=3;
                                drive=1;
                                softPwmWrite(DC1_A, 0);
                                softPwmWrite(DC1_B, speed);
                                softPwmWrite(DC2_A, speed);
                                softPwmWrite(DC2_B, 0);
                                break;
                            case 4:
                                //우회전
                                status=4;
                                drive=1;
                                softPwmWrite(DC1_A, speed);
                                softPwmWrite(DC1_B, 0);
                                softPwmWrite(DC2_A, 0);
                                softPwmWrite(DC2_B, speed);
                                break;
                        }
                    }
                    break;
                case 'r':
                    if(speed<100){
                        speed += 10;
                        switch(status){
                            case 0:
                                softPwmWrite(DC1_A, 0);
                                softPwmWrite(DC1_B, 0);
                                softPwmWrite(DC2_A, 0);
                                softPwmWrite(DC2_B, 0);
                                drive = 0;
                                status = 0;
                                break;
                                //정지
                            case 1:
                                drive=1;
                                status = 1;
                                softPwmWrite(DC1_A, speed);
                                softPwmWrite(DC1_B, 0);
                                softPwmWrite(DC2_A, speed);
                                softPwmWrite(DC2_B, 0);
                                break;
                                //전진
                            case 2:
                                drive=1;
                                status=2;
                                softPwmWrite(DC1_A, 0);
                                softPwmWrite(DC1_B, speed);
                                softPwmWrite(DC2_A, 0);
                                softPwmWrite(DC2_B, speed);
                                break;
                                //후진
                            case 3:
                                //좌회전
                                status=3;
                                drive=1;
                                softPwmWrite(DC1_A, 0);
                                softPwmWrite(DC1_B, speed);
                                softPwmWrite(DC2_A, speed);
                                softPwmWrite(DC2_B, 0);
                                break;
                            case 4:
                                //우회전
                                status=4;
                                drive=1;
                                softPwmWrite(DC1_A, speed);
                                softPwmWrite(DC1_B, 0);
                                softPwmWrite(DC2_A, 0);
                                softPwmWrite(DC2_B, speed);
                                break;
                        }
                    }
                    break;
                case 'w':
                    drive=1;
                    status = 1;
                    softPwmWrite(DC1_A, speed);
                    softPwmWrite(DC1_B, 0);
                    softPwmWrite(DC2_A, speed);
                    softPwmWrite(DC2_B, 0);
                    break;
                case 's':
                    drive=1;
                    status=2;
                    softPwmWrite(DC1_A, 0);
                    softPwmWrite(DC1_B, speed);
                    softPwmWrite(DC2_A, 0);
                    softPwmWrite(DC2_B, speed);
                    break;    
                case 'a':
                    //좌회전
                    status=3;
                    drive=1;
                    softPwmWrite(DC1_A, 0);
                    softPwmWrite(DC1_B, speed);
                    softPwmWrite(DC2_A, speed);
                    softPwmWrite(DC2_B, 0);
                    break;
                case 'd':
                    //우회전
                    status=4;
                    drive=1;
                    softPwmWrite(DC1_A, speed);
                    softPwmWrite(DC1_B, 0);
                    softPwmWrite(DC2_A, 0);
                    softPwmWrite(DC2_B, speed);
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