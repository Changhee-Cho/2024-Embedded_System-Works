// 삼색 LED
#define RED 27
#define GREEN 28
#define BLUE 29

// 능동부저(Active Buzzer)
#define BUZ_A_PIN 30

// 수동부저(Passive Buzzer)
#define BUZ_P_PIN 21

// 스위치(Push Switch — PULL UP 구조)
#define PUSHSWL 0 // 왼쪽
#define PUSHSWR 7 // 오른쪽

// 초음파센서(Ultrasonic)
#define TRIG 4
#define ECHO 5

// DC Motor
#define DC_A 6
#define DC_B 31

// Step Motor
#define BLUE 22
#define PINK 23
#define YELL 24
#define ORAG 25

// Servo Motor
# define SERVO 26

// LCD
#define BASE_ADDR 100
#define RS_PIN BASE_ADDR // 100
#define RW_PIN BASE_ADDR+1 // 101
#define EN_PIN BASE_ADDR+2 // 102
#define BL_PIN BASE_ADDR+3 // 103
#define D4_PIN BASE_ADDR+4 // 104
#define D5_PIN BASE_ADDR+5 // 105
#define D6_PIN BASE_ADDR+6 // 106
#define D7_PIN BASE_ADDR+7 // 107

// 4-FND
#define CLK_PIN 9
#define DIO_PIN 8

// IR_PIN
#define IR_PIN 18

2-1. RGB LED가 1초마다 빨-초-파 순으로 변하고 2초 꺼지기를 반복

// gcc 2-1_RGBLED.c -o 2-1 –lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define RED 27
#define GREEN 28
#define BLUE 29

int main(void){
    if(wiringPiSetup()==-1){
        printf("Setup Error!\n");
        return -1;
    }

    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);

    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);

    while(1)
    {
        // only RED ON
        digitalWrite(RED, HIGH);
        printf("RED LED ON\n");
        delay(1000);

        // only GREEN on
        digitalWrite(RED, LOW);
        digitalWrite(GREEN, HIGH);
        printf("GREEN LED ON\n");
        delay(1000);

        // only BLUE on
        digitalWrite(GREEN, LOW);
        digitalWrite(BLUE, HIGH);
        printf("BLUE LED ON\n");
        delay(1000);

        // all LED off
        digitalWrite(RED, LOW);
        digitalWrite(GREEN, LOW);
        digitalWrite(BLUE, LOW);
        printf("RGB LED OFF\n");
        delay(2000);
    }
    return 0;
}
2-2. 1초마다 능동부저가 켜지고 꺼지기를 반복

// gcc 2-2_ActiveBuzzer.c -o 2-2 –lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define BUZ_A_PIN 30

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Error!\n");
        return -1;
    }

    pinMode(BUZ_A_PIN, OUTPUT);

    while(1){
        // BUZ activation
        digitalWrite(BUZ_A_PIN, HIGH);
        delay(1000);
        
        // BUZ deactivation
        digitalWrite(BUZ_A_PIN, LOW);
        delay(1000);
    }
}

























2-3. 왼쪽 스위치가 눌리면 왼쪽 스위치 눌림, 오른쪽 스위치가 눌리면 오른쪽 스위치 눌림을 출력

// gcc 2-3_PushSwitch.c -o 2-3 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define PUSHSWL 0
#define PUSHSWR 7

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Error\n");
        return -1;
    }

    pinMode(PUSHSWL, INPUT);
    pinMode(PUSHSWR, INPUT);
    
    while(1){
        if(digitalRead(PUSHSWL)==LOW){
            printf("Left Switch Pushed!\n");
            delay(300);
        }
        else if(digitalRead(PUSHSWR)==LOW){
            printf("Right Switch Pushed!\n");
            delay(300);
        }
    }
    return 0;
}
과제 1
Switch를 누를 때마다 LED의 상태가 변하는 프로그램
• 프로그램이 실행되면 LED는 소등 상태이다.
• 소등 상태에서 Switch를 누르면 녹색 LED가 점등된다.
• 녹색 LED가 켜진 상태에서 Switch를 누르면 빨간색 LED가 점등된다.
• 빨간색 LED가 켜진 상태에서 Switch를 누르면 파란색 LED가 점등된다.
• 파란색 LED가 켜진 상태에서 Switch를 누르면 LED가 소등된다.

// gcc 2-homework1.c -o 2-homework1 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define RED 27
#define GREEN 28
#define BLUE 29

#define LEFT 0

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Error!\n");
        return -1;
    }

    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);

    int status = 0;
    // 0:off, 1: GREEN on, 2: RED on, 3: BLUE on

    while(1){
        if(digitalRead(LEFT)==LOW){
            status++;
            if(status==4)
                status = 0;
            switch(status){
                case 0:
                    digitalWrite(RED, LOW);
                    digitalWrite(GREEN, LOW);
                    digitalWrite(BLUE, LOW);
                    break;
                case 1:
                    digitalWrite(GREEN, HIGH);
                    break;
                case 2:
                    digitalWrite(GREEN, LOW);
                    digitalWrite(RED, HIGH);
                    break;
                case 3:
                    digitalWrite(RED, LOW);
                    digitalWrite(BLUE, HIGH);
                    break;
            }
            delay(300);
        }
    }
    return 0;
}
과제 2
Switch를 누르면 부저가 울리는 프로그램
• 프로그램을 실행하면 능동 부저가 울리지 않는 상태이다.
• 왼쪽버튼을 누르면 부저가 딜레이(delay)없이 울린다.
• 부저가 울릴 때 오른쪽 버튼을 누르면 부저가 꺼진다.

// gcc 2-homework2.c -o 2-homework2 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define BUZ 30
#define LEFT 0
#define RIGHT 7

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Error\n");
        return -1;
    }
    
    pinMode(BUZ, OUTPUT);
    pinMode(LEFT, INPUT);
    pinMode(RIGHT, INPUT);
    
    digitalWrite(BUZ, LOW);

    while(1){
        if(digitalRead(LEFT)==LOW){
            digitalWrite(BUZ, HIGH);
            delay(300);
        }
        else if(digitalRead(RIGHT)==LOW){
            digitalWrite(BUZ, LOW);
            delay(300);
        }
    }
    return 0;
}













3-1. 초음파센서로 거리측정 값 터미널 출력 무한반복

// gcc 3-1_Ultrasonic.c -o 3-1 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define TRIG 4
#define ECHO 5

int main(){
    int start = 0, end = 0;
    float distance = 0, duration = 0;

    if(wiringPiSetup()==-1){
        printf("Setup Error\n");
        return -1;
    }

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    while(1){
        digitalWrite(TRIG, LOW);
        delay(500);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);

        while(digitalRead(ECHO)==LOW)
            start = micros();
        while(digitalRead(ECHO)==HIGH)
            end = micros();
        
        duration = (float)(end - start);
        duration = duration / 1000000 / 2;
        distance = (duration * 340) * 100;

        printf("Distance: %3.0f cm \n", distance);
    }
    return 0;
}
3-2. DC 모터를 1초 시계방향, 1초 반시계방향, 1초 정지를 무한 반복

// gcc 3-2_DCMotor.c -o 3-2 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define DC_A 6
#define DC_B 31

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Error\n");
        return -1;
    }
    pinMode(DC_A, OUTPUT);
    pinMode(DC_B, OUTPUT);

    while(1){
        // CW
        digitalWrite(DC_A, LOW);
        digitalWrite(DC_B, HIGH);
        printf("Motor run CW\n");
        delay(1000);

        // CCW
        digitalWrite(DC_A, HIGH);
        digitalWrite(DC_B, LOW);
        printf("Motor run CCW\n");
        delay(1000);

        // Stop
        digitalWrite(DC_A, LOW);
        digitalWrite(DC_B, LOW);
        printf("Motor run STOP\n");
        delay(1000);
    }
    return 0;
}












3-3. Step Motor를 시계방향 360도, 반시계 360도 회전한 후 500ms 정지하기를 반복(360도:512번 setStep 반복)

// gcc 3-3_StepMotor.c -o 3-3 –l wiringPi

#include <stdio.h>
#include <wiringPi.h>
#define BLUE 22
#define PINK 23
#define YELL 24
#define ORAG 25

void setStep(int b, int p, int y, int o){
    digitalWrite(BLUE, b);
    digitalWrite(PINK, p);
    digitalWrite(YELL, y);
    digitalWrite(ORAG, o);
}

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Error\n");
        return -1;
    }
    pinMode(BLUE, OUTPUT);
    pinMode(PINK, OUTPUT);
    pinMode(YELL, OUTPUT);
    pinMode(ORAG, OUTPUT);

    while(1){
        printf("CW\n");
        for(int i=0; i<512; i++){
            setStep(HIGH, HIGH, LOW, LOW);
            delay(2);
            setStep(LOW, HIGH, HIGH, LOW);
            delay(2);
            setStep(LOW, LOW, HIGH, HIGH);
            delay(2);
            setStep(HIGH, LOW, LOW, HIGH);
            delay(2);
        }

        printf("CCW\n");
        for(int i=0; i<512; i++){
            setStep(HIGH, LOW, LOW, HIGH);
            delay(2);
            setStep(LOW, LOW, HIGH, HIGH);
            delay(2);
            setStep(LOW, HIGH, HIGH, LOW);
            delay(2);
            setStep(HIGH, HIGH, LOW, LOW);
            delay(2);
        }

        printf("STOP\n");
        setStep(HIGH, HIGH, HIGH, HIGH);
        delay(500);
    }
    return 0;
}
과제 3
초음파 센서를 이용하여 부저를 동작시키는 프로그램을 만드시오.
• 초음파 센서를 동작 시키고 10회 동안 거리를 측정한다.
• 측정 시작은 사용자가 Enter 키를 누르면 수행된다.
• 측정한 거리 중 가장 가까운 값과 가장 먼 값을 터미널 창에 출력한다.
• 가장 가까운 거리가 20cm 이하면 부저를 {0.25(on)-0.25(off)-0.25(on)-0.25(off)} x 2 주기로 울린다.
• 가장 가까운 거리가 30cm 이하면 부저를 0.5(on)-0.5(off)-0.5(on)-0.5(off) 주기로 울린다.
• 가장 가까운 거리가 40cm 이하면 부저를 2초 동안 울린다.


// gcc 3-homework1.c -o 3-homework3 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define TRIG 4
#define ECHO 5
#define BUZ 30

int main(){
    wiringPiSetup();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(BUZ, OUTPUT);

    float value[10] = { 0 };
    while(1){
        getchar();
        for(int i=0; i<10; i++){
            int start = 0, end = 0;
            float duration = 0, distance = 0;

            digitalWrite(TRIG, LOW);
            delay(500);
            digitalWrite(TRIG, HIGH);
            delayMicroseconds(10);
            digitalWrite(TRIG, LOW);

            while(digitalRead(ECHO)==LOW)
                start = micros();
            while(digitalRead(ECHO)==HIGH)
                end = micros();
            duration = (float)(end-start);
            duration = duration / 1000000 / 2;
            distance = (340 * duration) * 100;
            value[i]=distance;
            printf("%d번째: %3.0f cm\n", i+1, value[i]);
        }
        float min = value[0];
        float max = value[0];
        for(int i=0; i<10; i++){
            if(min>value[i])
                min = value[i];
            if(max<value[i])
                max = value[i];
        }
        printf("Shortest: %3.0f cm\n", min);
        printf("Longest: %3.0f cm\n", max);

        if(min<=20){
            for(int i=0; i<2; i++){
                digitalWrite(BUZ, HIGH);
                delay(250);
                digitalWrite(BUZ, LOW);
                delay(250);
                digitalWrite(BUZ, HIGH);
                delay(250);
                digitalWrite(BUZ, LOW);
                delay(250);
            }
        }
        else if(min<=30){
            digitalWrite(BUZ, HIGH);
            delay(500);
            digitalWrite(BUZ, LOW);
            delay(500);
            digitalWrite(BUZ, HIGH);
            delay(500);
            digitalWrite(BUZ, LOW);
            delay(500);
        }
        else if(min<=40){
            digitalWrite(BUZ, HIGH);
            delay(2000);
            digitalWrite(BUZ, LOW);
        }
    }
    return 0;
}
과제 4
키보드 입력을 통해 DC 모터를 제어하는 프로그램을 만드시오.
• 프로그램을 시작하면 정지상태이다.
• ‘+’ 키를 입력하면 시계방향으로 회전하고, ‘-’ 를 입력하면 반시계 방향으로 회전한다.
• 이 때, getch() 함수를 사용하여 enter를 누르지 않아도 동작하도록 하시오.
• ‘0’ 을 입력하면 다시 정지상태가 된다.
• 정지상태는 빨간색 LED, 시계방향은 파란색, 반시계방향은 초록색을 점등한다.

// gcc 3-homework4.c –o3-homework4 -lwiringPi
// 같은 디렉토리 안에 getch.h 파일 있음

#include <stdio.h>
#include <wiringPi.h>
#include "getch.h"

#define DC_A 6
#define DC_B 31
#define RED 27
#define GREEN 28
#define BLUE 29

char getch();

int main(){
    wiringPiSetup();
    pinMode(DC_A, OUTPUT);
    pinMode(DC_B, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);

    digitalWrite(DC_A, LOW);
    digitalWrite(DC_B, LOW);
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);


    char key;
    while(1){
        if(key=getch()){
            digitalWrite(RED, LOW);
            digitalWrite(GREEN, LOW);
            digitalWrite(BLUE, LOW);
            switch(key){
                case '+':
                    // CW
                    digitalWrite(DC_A, LOW);
                    digitalWrite(DC_B, HIGH);
                    digitalWrite(BLUE, HIGH);
                    break;
                case '-':
                    // CCW
                    digitalWrite(DC_A, HIGH);
                    digitalWrite(DC_B, LOW);
                    digitalWrite(GREEN, HIGH);
                    break;
                case '0':
                    // STOP
                    digitalWrite(DC_A, LOW);
                    digitalWrite(DC_B, LOW);
                    digitalWrite(RED, HIGH);
                    break;
            }
        }
    }
    return 0;
}
과제 5
키보드 입력을 통해 스탭 모터를 제어하는 프로그램을 만드시오.
• 스탭 모터는 사용자가 입력한 각도 만큼 회전한다.
• 사용자 입력을 통해 양수를 입력하면 시계방향으로 해당 각도만큼 이동한다.
• 음수를 입력하면 반시계방향으로 해당 각도만큼 이동한다.
• 시계 방향으로 회전하면 빨간색 LED를 점등하고, 반시계방향은 파란색 LED를 점등한다.
• 정지 상태에서는 LED가 소등되어 있다.

// gcc 3-homework5.c -o 3-homework5 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>

#define BLUE 22
#define PINK 23
#define YELL 24
#define ORAG 25

#define RED 27
#define BLED 29

char getch();

void setStep(int b, int p, int y, int o);

int main(){
    wiringPiSetup();
    pinMode(BLUE, OUTPUT);
    pinMode(PINK, OUTPUT);
    pinMode(YELL, OUTPUT);
    pinMode(ORAG, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(BLED, OUTPUT);

    digitalWrite(RED, LOW);
    digitalWrite(BLED, LOW);
   
    while(1){
        float angle;
        scanf("%f", &angle);
        // if angle > 0 natural number
        // CW
        if(angle>0){
            digitalWrite(RED, HIGH);
            angle = angle * 1.422222222222222222222222;
            for(int i=0; i<angle; i++){
                setStep(HIGH, HIGH, LOW, LOW);
                delay(2);
                setStep(LOW, HIGH, HIGH, LOW);
                delay(2);
                setStep(LOW, LOW, HIGH, HIGH);
                delay(2);
                setStep(HIGH, LOW, LOW, HIGH);
                delay(2);
            }
        }
        // angle < 0 minus number
        else{
            digitalWrite(BLED, HIGH);
            angle = angle * 1.422222222222222222222222 * -1;
            for(int i=0; i<angle; i++){
                setStep(HIGH, LOW, LOW, HIGH);
                delay(2);
                setStep(LOW, LOW, HIGH, HIGH);
                delay(2);
                setStep(LOW, HIGH, HIGH, LOW);
                delay(2);
                setStep(HIGH, HIGH, LOW, LOW);
                delay(2);
            }
        }
        digitalWrite(RED, LOW);
        digitalWrite(BLED, LOW);
    }
        return 0;
}

void setStep(int b, int p, int y, int o){
    digitalWrite(BLUE, b);
    digitalWrite(PINK, p);
    digitalWrite(YELL, y);
    digitalWrite(ORAG, o);
}
4-1. PWM으로 0~255 사이의 값을 출력하는 것을 반복(5씩 증가)

//gcc 4-1_PWMLED.c -o4-1 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#define RED 27

int main(){
    wiringPiSetup();
    pinMode(RED, SOFT_PWM_OUTPUT);
    softPwmCreate(RED, 0, 255);
    while(1){
        for(int i=0; i<=255; i+=5){
            softPwmWrite(RED, i);
            printf("RED LED %d\n", i);
            delay(50);
        }
    }
    return 0;
}





























4-2. 10~100 Duty 사이에서 10씩 증감하며 DC모터 회전반복

// gcc 4-2_PWMDC.c –o4-2 -lwiringPi
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define DC_A 6
#define DC_B 31

int main(){
    wiringPiSetup();
    pinMode(DC_A, SOFT_PWM_OUTPUT);
    pinMode(DC_B, SOFT_PWM_OUTPUT);

    softPwmCreate(DC_A, 0, 100);
    softPwmCreate(DC_B, 0, 100);

    int duty = 10;
    int updown = 10;

    while(1){
        if(duty>90)
            updown = -10;
        if(duty<20)
            updown = 10;
        duty += updown;
        printf("Motor duty = %d\n", duty);
        
        softPwmWrite(DC_A, duty);
        softPwmWrite(DC_B, 0);
        printf("Motor run CCW\n");
        delay(1000);

        softPwmWrite(DC_A, 0);
        softPwmWrite(DC_B, duty);
        printf("Motor run CW\n");
        delay(1000);

        softPwmWrite(DC_A, 0);
        softPwmWrite(DC_B, 0);
        printf("Motor run STOP\n");
        delay(1000);
    }
    return 0;
}
4-3. 0.5초씩 서보모터를 0도 –90도 90도 반복

// gcc 4-3_SERVO.c -o 4-3 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define SERVO 26

int main(){
    wiringPiSetup();
    pinMode(SERVO, SOFT_PWM_OUTPUT);
    softPwmCreate(SERVO, 0, 200);
    while(1){
        // 0 angle
        softPwmWrite(SERVO, 15);
        delay(500);
        
        // -90 angle
        softPwmWrite(SERVO, 24);
        delay(500);

        // 90 angle
        softPwmWrite(SERVO, 5);
        delay(500);
    }
    softPwmStop(SERVO);
    delay(500);
    return 0;
}





















4-4. 수동 부저를 이용해 낮은 도~ 높은 도까지 1초마다 증가하는 것을 반복

// gcc 4-4_PassiveBuzzer.c -o 4-4 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>

#define BUZ_P_PIN 21
unsigned int melody[8] = {262, 294, 330, 349, 392, 440, 494, 523};

int main(){
    int idx = 0;
    wiringPiSetup();

    pinMode(BUZ_P_PIN, SOFT_TONE_OUTPUT);
    softToneCreate(BUZ_P_PIN);
    while(1){
        for(int i=0; i<8; i++){
            softToneWrite(BUZ_P_PIN, melody[i]);
            delay(1000);
        }
    }
    softToneStop(BUZ_P_PIN);
    return 0;
}
-------------------------------------------------
4-5. 카메라를 이용해 엔터를 칠 때마다 사진 촬영

// gcc 4-5_Camera.c –o 4-5

#include <stdio.h>
#include <stdlib.h>

int main(){
    while(1){
        getchar();
        char cmd[] = "sudo libcamera-jpeg --width 800 --height 800 -t 1 -o test.jpeg";
        system(cmd);
        printf("Take a picture!\n");
    }
}
과제 6
키보드 입력을 통해 서보 모터를 제어하는 프로그램을 만드시오.
• 서보모터는 초기에 오른쪽을 가리키고 있다. (15 에 해당하는 각도)
• 키보드의 q 를 누르면 서보 모터가 반시계 방향 1만큼 회전한다. (최대 24)
• 키보드의 e 를 누르면 서보 모터가 시계 방향 1만큼 회전한다. (최소 6)
• 이 때 각 버튼은 enter를 누르지 않고, 누르는 즉시 이동한다.
• 함수는 다음 장의 char getch() 함수를 사용한다.
• 이와 동시에, 서보 모터의 각도가 가장 작을 때는 LED가 꺼져 있고 상승할 수록 단계적으로 밝아진다.

//gcc 4-homework6.c -o4-homework6 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "getch.h"


#define SERVO 26
#define LED 27

char getch();

int main(){
    wiringPiSetup();
    pinMode(SERVO, SOFT_PWM_OUTPUT);
    pinMode(LED, SOFT_PWM_OUTPUT);
    softPwmCreate(SERVO, 0, 200);
    softPwmCreate(LED, 0, 18);
    
    // default 15
    softPwmWrite(SERVO, 15);
    softPwmWrite(LED, 9);
    char key;
    int angle = 15;
    int light = 9;

    while(1){
        if(key=getch()){
            switch(key){
                case 'q':
                    if(angle<24){
                        // CCW
                        angle++;
                        light++;
                        softPwmWrite(SERVO, angle);
                        softPwmWrite(LED, light);
                    }
                    break;
                case 'e':
                    if(angle>6){
                        angle--;
                        light--;
                        softPwmWrite(SERVO, angle);
                        softPwmWrite(LED, light);
                    }
                    break;
            }
        }
    }
    return 0;
}
과제 7
수동 부저를 사용하여 피아노 프로그램을 만드시오.
• a~k 의 키보드 버튼은 각각 ‘낮은 도’ ~ ‘높은 도’에 대응한다.
• getch() 함수를 사용하여 8개의 음을 연주하는 피아노를 만드시오.
• 키 한 번은 125 ms 의 딜레이를 갖는다.

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
과제 8
카메라를 이용하여 접근하는 물체를 촬영하시오.
• 초기에는 초음파 센서, 카메라, 부저가 동작하지 않는다.
• enter 키를 누르면 시스템이 시작된다. 시스템이 시작되면 초음파 센서를 통해 거리를 측정한다.
• 만약 물체가 20cm 이내로 접근할 경우, 부저를 3초 동안 짧은 주기로 울린다.
• 만약 부저가 울린 이후에도 물체가 20cm 이내에 있을 경우 사진을 촬영한다.
• 사진을 촬영하게 되면 “Take a Picture!” 문장을 출력한다.
• 부저가 울린 이후에 물체가 20cm에 있지 않다면 아무 일도 일어나지 않는다. 

#include <stdio.h>
#include <wiringPi.h>
#include <string.h>
#include <stdlib.h>

#define TRIG 4
#define ECHO 5
#define BUZ 30

int main(){
    if(wiringPiSetup()==-1){
        printf("ERROR\n");
        return -1;
    }
    pinMode(BUZ, OUTPUT);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    int start=0, end=0;
    float distance = 0, duration = 0;
    getchar();
    while(1){
        digitalWrite(TRIG, LOW);
        delay(500);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
        while(digitalRead(ECHO)==LOW)
            start=micros();
        while(digitalRead(ECHO)==HIGH)
            end=micros();
        duration = (float)(end-start);
        duration = duration/1000000/2;
        distance = (340 * duration) * 100;
        printf("Distance: %.3fcm\n", distance);

        if(distance<=20){
            for(int i=0; i<3; i++){
                digitalWrite(BUZ, HIGH);
                delay(500);
                digitalWrite(BUZ, LOW);
                delay(500);
            }

        digitalWrite(TRIG, LOW);
        delay(500);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
        while(digitalRead(ECHO)==LOW)
            start=micros();
        while(digitalRead(ECHO)==HIGH)
            end=micros();
        duration = (float)(end-start);
        duration = duration/1000000/2;
        distance = (340 * duration) * 100;
        printf("Distance: %.3fcm\n", distance);

        if(distance<=20){
            char cmd[] = "sudo libcamera-jpeg --width 800 --height 800 -t 1 -o test.jpeg";
            printf("%s \n", cmd);
            system(cmd);
            printf("Take a picture \n");
        }
        while(distance>20){
            digitalWrite(TRIG, LOW);
        delay(500);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
        while(digitalRead(ECHO)==LOW)
            start=micros();
        while(digitalRead(ECHO)==HIGH)
            end=micros();
        duration = (float)(end-start);
        duration = duration/1000000/2;
        distance = (340 * duration) * 100;
        printf("Distance: %.3fcm\n", distance);
        }
    }
}
return 0;
}
5-1. LCD 윗줄 “Hello World” 아랫줄 “*Good Luck*” 출력 2초마다 깜빡깜빡

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









5-2. 4FND에 0~F까지 순서대로 전부 출력 반복

// gcc 5-2_FND.c -o 5-2 TM1637.c -lwiringPi -fcommon

#include <stdio.h>
#include <wiringPi.h>
#include "TM1637.h"

#define CLK_PIN 9
#define DIO_PIN 8

int main(){
    int8_t NumTab[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int8_t ListDisp[4];
    unsigned char i=0, count = 0;
    delay(150);
    
    wiringPiSetup();

    pinMode(CLK_PIN, INPUT);
    pinMode(DIO_PIN, INPUT);
    delay(200);

    TM1637_init(CLK_PIN,DIO_PIN);
    TM1637_set(BRIGHTEST, 0x40, 0xc0);

    while(1){
        unsigned char BitSelect = 0;
        i = count;
        count++;
        if(count == sizeof(NumTab))
            count = 0;
        for(BitSelect = 0; BitSelect<4; BitSelect++){
            ListDisp[BitSelect] = NumTab[i];
            i++;
            if(i==sizeof(NumTab))
                i=0;
        }
        TM1637_display(0, ListDisp[0]);
        TM1637_display(1, ListDisp[1]);
        TM1637_display(2, ListDisp[2]);
        TM1637_display(3, ListDisp[3]);
        delay(500);
    }
    return 0;
}
5-3(1). IR센서 값 읽어들여서 화면에 출력

// gcc 5-3_IR1.c -o 5-3 irpigpio.c -lpthread -lwiringPi -lpigpio
// sudo 로 실행시켜야 함

#include "irpigpio.h"
#include <stdio.h>
#include <wiringPi.h>

#define IR 18
#define LED 27

int deviceSetup(){
    if(gpioInitialise()<0 || wiringPiSetup()<0){
        return -1;
    }
    gpioSetMode(IR, PI_INPUT);
    gpioSetWatchdog(IR, 5);
    gpioSetAlertFunc(IR, alert);
}

int main(){
    long value;
    if(deviceSetup()<0)
        return -1;
    pinMode(LED, OUTPUT);
    while(1){
        value=readIR();
        printf("IR value %u\n", value);
    }
    gpioTerminate();
}



















5-3(2). 리모컨 버튼에 따라 동작하기 0을 누르면 LED 켜고, 1을 누르면 LED off

// gcc 5-3_IR1.c -o 5-3 irpigpio.c -lpthread -lwiringPi -lpigpio
// sudo 로 실행시켜야 함

#include "irpigpio.h"
#include <stdio.h>
#include <wiringPi.h>

#define IR 18
#define LED 27
#define BUTTON_0 951308750
#define BUTTON_1 271208262

int deviceSetup(){
    if(gpioInitialise()<0 || wiringPiSetup()<0){
        return -1;
    }
    gpioSetMode(IR, PI_INPUT);
    gpioSetWatchdog(IR, 5);
    gpioSetAlertFunc(IR, alert);
}

int main(){
    long value;
    if(deviceSetup()<0)
        return -1;
    pinMode(LED, OUTPUT);
    while(1){
        value = readIR();
        switch(value){
            case BUTTON_0:
                printf("Button 0 Pressed, LED ON\n");
                digitalWrite(LED, HIGH);
                break;
            case BUTTON_1:
                printf("Button 1 Pressed, LED Off\n");
                digitalWrite(LED, LOW);
                break;
        }
    }
    gpioTerminate();
}
과제 9
FND에 프로그램의 실행결과를 출력한다.
• 프로그램을 실행하면 시스템은 2가지의 기능을 갖는다.
• 키보드의 q 를 누르면 두개의 양의 정수를 입력 받는 문구가 출력된다.
• scanf를 이용해 두 개의 양의 정수를 입력했을 때 두 수의 합을 출력한다.
• 값이 출력된 이후에 키보드의 e를 누르면 해당 숫자가 1씩 줄어들고, 0이 되면 LED가 1초 동안 켜진다.
• q를 이용해 양의 정수를 입력 받지 않은 상태에서 e를 누르면 아무일도 일어나지 않는다.

// gcc homework9.c –o 9 TM1637.c -lwiringPi -fcommon

#include <stdio.h>
#include <wiringPi.h>
#include "TM1637.h"
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include “getch.h”

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
과제 10
IR센서와 리모컨을 사용한 프로그램의 실행결과를 LCD에 출력한다.
• 초기의 LCD에는 아무것도 출력되지 않는다.
• 리모컨의 1번을 누르면 LCD에 ‘Open the door’ 가 출력되고 1초 뒤 사라진다.
• 리모컨의 2번을 누르면 LCD에 ‘Close the door’ 가 출력되고 1초 뒤 사라진다.
• 문이 열릴 때는 상승하는 음계가 수동 부저에서 출력된다. ex) 도 미 솔 ~
• 문이 닫힐 때는 하강하는 음계가 수동 부저에서 출력된다. ex) 솔 미 도 ~
• 문이 열린 상태에서 1번을 누르거나, 닫힌 상태에서 2번을 누르면 아무 일도 일어나지 않는다.
• 리모컨의 2번을 눌렀을 때, 신호 값이 불안정하다면 다른 버튼을 사용한다.

//  gcc 5-homework10.c -o homework10 -lwiringPi -lwiringPiDev irpigpio.c -lpthread -lpigpio


#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
#include <pcf8574.h>
#include <lcd.h>
#include "irpigpio.h"

#define BASE_ADDR 100
#define RS_PIN BASE_ADDR
#define RW_PIN BASE_ADDR+1
#define EN_PIN BASE_ADDR+2
#define BL_PIN BASE_ADDR+3
#define D4_PIN BASE_ADDR+4
#define D5_PIN BASE_ADDR+5
#define D6_PIN BASE_ADDR+6
#define D7_PIN BASE_ADDR+7

#define IR 18
#define BUZ 21
#define BUTTON_1 271208262
#define BUTTON_2 1297467390
unsigned int melody[8] = {262, 294, 330, 349, 392, 440, 494, 523};

int main(){
    wiringPiSetup();
    pinMode(BUZ, SOFT_TONE_OUTPUT);
    if(gpioInitialise()==0)
        return -1;
    gpioSetMode(IR, PI_INPUT);
    gpioSetWatchdog(IR, 5);
    gpioSetAlertFunc(IR, alert);

    pcf8574Setup(RS_PIN, 0x27);
    pinMode (RW_PIN, OUTPUT);
    digitalWrite (RW_PIN, LOW);
    pinMode (BL_PIN, OUTPUT);
    digitalWrite (BL_PIN, HIGH);
    int fd = lcdInit (2, 16, 4, RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN,
    D7_PIN, 0,0,0,0 );

    long value;
    int status = 0; int logic = 0;
    while(1){
        value = readIR();
        if(value==BUTTON_1)
            status = 1;
        else if(value == BUTTON_2)
            status = 2;
        if(status > 0){
            lcdClear(fd);
            delay(500);
            lcdPosition(fd, 0, 0);
            if(status == 1 && logic!=1){
                lcdPuts(fd, "Open the door");
                logic=1;
                
                softToneWrite(BUZ, melody[0]);
                delay(500);
                softToneWrite(BUZ, melody[2]);
                delay(500);
                softToneWrite(BUZ, melody[4]);
                delay(500);
            }
            else if(status == 2 && logic !=2){
                lcdPuts(fd, "Close the door");
                logic=2;
   
                softToneWrite(BUZ, melody[4]);
                delay(500);
                softToneWrite(BUZ, melody[2]);
                delay(500);
                softToneWrite(BUZ, melody[0]);
                delay(500);
            }
            softToneWrite(BUZ, 0);
            delay(1000);
            lcdClear(fd);
        }
    }
    return 0;
}
6-1. 가변저항 값 계속 읽어들이기

// gcc 6-1.c –o 6-1 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>
#include <string.h>

#define CHAN_CONFIG_SINGLE 8
#define SPI_SPEED 1000000
#define SPI_CE_CHANNEL 1
#define SPI_ADC_POTEN_CHANNEL 1
#define CS_MCP3208 11

int spiSetup(void);
int readAnalogData(int adcChannel);

int main(){
    wiringPiSetup();
    spiSetup();
    pinMode(CS_MCP3208, OUTPUT);
    int adcValue = 0;
    while(1){
        adcValue=readAnalogData(SPI_ADC_POTEN_CHANNEL);
        printf("Potentionmeter Value = %d\n", adcValue);
        delay(500);
    }
    return 0;
}

int spiSetup(void){
    if(wiringPiSPISetup(SPI_CE_CHANNEL, SPI_SPEED)==-1){
        fprintf(stderr, "wiringPiSPISetup Failed! Error: %s\n", strerror(errno));
        return 1;
    }
}

int readAnalogData(int adcChannel){
    int adcValue=0;
    unsigned char buffer[3] = {0};
    if(adcChannel<0||adcChannel>7)
        return -1;
    buffer[0]=0x01;
    buffer[1]=(CHAN_CONFIG_SINGLE+adcChannel)<<4;
    digitalWrite(CS_MCP3208, LOW);
    wiringPiSPIDataRW(SPI_CE_CHANNEL, buffer, 3);
    adcValue=(((buffer[1]&3)<<8)+buffer[2]);
    digitalWrite(CS_MCP3208,HIGH);
    return adcValue;
}
6-2. CDS 센서 값 읽어들이기
// 어두울수록 값이 커지고 밝을수록 값이 작아짐
// 가변저항 코드에서 SPI_ADC_LIGHT_CHANNEL 만 바뀜(CDS:0, 가변저항 : 1)

// gcc 6-2.c –o 6-2 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>
#include <string.h>

#define CHAN_CONFIG_SINGLE 8
#define SPI_SPEED 1000000
#define SPI_CE_CHANNEL 1
#define SPI_ADC_LIGHT_CHANNEL 0
#define CS_MCP3208 11

int spiSetup(void);
int readAnalogData(int adcChannel);

int main(){
    wiringPiSetup();
    spiSetup();
    pinMode(CS_MCP3208, OUTPUT);
    int adcValue = 0;
    while(1){
        adcValue=readAnalogData(SPI_ADC_LIGHT_CHANNEL);
        printf("Light Value = %d\n", adcValue);
        delay(500);
    }
    return 0;
}

int spiSetup(void){
    if(wiringPiSPISetup(SPI_CE_CHANNEL, SPI_SPEED)==-1){
        fprintf(stderr, "wiringPiSPISetup Failed! Error: %s\n", strerror(errno));
        return 1;
    }
}

int readAnalogData(int adcChannel){
    int adcValue=0;
    unsigned char buffer[3] = {0};
    if(adcChannel<0||adcChannel>7)
        return -1;
    buffer[0]=0x01;
    buffer[1]=(CHAN_CONFIG_SINGLE+adcChannel)<<4;
    digitalWrite(CS_MCP3208, LOW);
    wiringPiSPIDataRW(SPI_CE_CHANNEL, buffer, 3);
    adcValue=(((buffer[1]&3)<<8)+buffer[2]);
    digitalWrite(CS_MCP3208,HIGH);
    return adcValue;
}
과제 11
가변저항과 조도센서를 이용한 금고를 만든다.
• 초기에는 아무 일도 일어나지 않는다.
• enter 키를 누르면 시스템이 시작되고, 10~1000 사이의 정수형 난수를 생성하여 키 값으로 저장한다.
• 가변 저항의 값이 화면상에 주기적으로 출력된다.
• 가변 저항의 값이 키 값의 ± 100 범위에 들어오면 빨간색 LED가 켜진다.
• 가변 저항의 값이 키 값의 ± 10 범위에 들어오면 수동부저로 상승 음계를 출력한다. 이 때 금고의 문이 열린다.
• 금고의 문이 열렸을 때, 조도가 정상이면 아무일도 일어나지 않는다.
• 조도가 낮으면(사람의 손으로 가렸을 때 도달하는 값) 카메라를 이용하여 사진을 촬영한다.
• 모든 작업이 수행되면 프로그램이 종료된다.

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <softTone.h>
#define CHAN_CONFIG_SINGLE 8
#define SPI_SPEED 1000000
#define SPI_CE_CHANNEL 1
#define SPI_ADC_POTEN_CHANNEL 1
#define SPI_ADC_LIGHT_CHANNEL 0
#define CS_MCP3208 11

#define BUZ 21
#define LED 27
unsigned int melody[8] = {262, 294, 330, 349, 392, 440, 494, 523};

int spiSetup();
int readAnalogData(int adcChannel);

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Fail\n");
        return -1;
    }
    softToneCreate(BUZ);
    spiSetup();
    pinMode(CS_MCP3208, OUTPUT);
    int adcValue = 0;

    getchar();
    srand((unsigned int)time(NULL));
    int key = rand()%1001+10;
    printf("key: %d\n", key);
    int status = 0;
    int cds = 0;
    while(1){
        adcValue =readAnalogData(SPI_ADC_POTEN_CHANNEL);
        printf("Potentionmeter value = %d\n", adcValue);
        delay(500);
        if(adcValue<=key+100 && adcValue>= key-100){
            digitalWrite(LED, HIGH);
            if(adcValue<=key+10 && adcValue>= key-10&&status == 0){
                softToneWrite(BUZ, melody[0]);
                delay(300);
                softToneWrite(BUZ, melody[2]);
                delay(300);
                softToneWrite(BUZ, melody[4]);
                delay(300);
                softToneWrite(BUZ, 0);
                status = 1;
                cds = readAnalogData(SPI_ADC_LIGHT_CHANNEL);
                if(cds>250){
                    char cmd[]="sudo libcamera-jpeg --width 800 --height 800 -t 1 -o test.jpg";
                    system(cmd);
                    printf("Picture created\n");
                    return 0;
                }
                else
                    return 0;
            }
            else{
                softToneWrite(BUZ, 0);
            }
        }
        else{
            digitalWrite(LED, LOW);
            status=0;
        }
    }
}

int spiSetup(){
    if(wiringPiSPISetup(SPI_CE_CHANNEL, SPI_SPEED) == -1){
        fprintf(stderr, "wiringPiSetup Failed ! ERROR: %s\n", strerror(errno));
        return 1;
    }
}

int readAnalogData(int adcChannel){
    int adcValue = 0;
    unsigned char buffer[3] = {0};
    if(adcChannel<0 || adcChannel>7)
        return 1;
    buffer[0]=0x01;
    buffer[1]=(CHAN_CONFIG_SINGLE+adcChannel)<<4;
    digitalWrite(CS_MCP3208, LOW);
    wiringPiSPIDataRW(SPI_CE_CHANNEL, buffer, 3);
    adcValue = ((buffer[1]&3)<< 8)+buffer[2];
    digitalWrite(CS_MCP3208, HIGH);
    return adcValue;
}
과제 12
가변저항을 이용하여 전자 하모니카를 만드시오. (23-2 중간고사 기출문제)
• 초기에는 어떠한 동작을 수행하지 않고, enter를 누르면 시스템이 시작된다.
• 가변 저항의 값은 수동부저의 주파수를 나타낸다. 가변 저항이 왼쪽 끝에 있을 때 주파수는 대략 200이고, 오른쪽 끝에 있을
때는 주파수가 대략 700이다.
• 키보드 Q를 누르면 해당 주파수의 음을 수동 부저를 통해 0.2초 동안 출력한다.
• 키보드 W를 누르면 해당 주파수의 음을 멈추지 않고 출력한다.
• 부저가 울릴 때는 빨간색 LED가 켜진다.
• 키보드 R을 누르면 수동 부저가 정지하고 LED가 꺼진다.

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <softTone.h>
#include <unistd.h>
#include <termios.h>

#define CHAN_CONFIG_SINGLE 8
#define SPI_SPEED 1000000
#define SPI_CE_CHANNEL 1
#define SPI_ADC_POTEN_CHANNEL 1
#define SPI_ADC_LIGHT_CHANNEL 0
#define CS_MCP3208 11

#define BUZ 21
#define LED 27
unsigned int melody[8] = {262, 294, 330, 349, 392, 440, 494, 523};

int spiSetup();
int readAnalogData(int adcChannel);
char getch();

int main(){
    if(wiringPiSetup()==-1){
        printf("Setup Fail\n");
        return -1;
    }
    char key;
    softToneCreate(BUZ);
    spiSetup();
    pinMode(CS_MCP3208, OUTPUT);
    int adcValue = 0;

    getchar();
    int tone = 0;
    while(1){
        //변환을 해 줘야 하는데, 0일때 200 주파수, 오른쪽 끝 700 주파수;
        adcValue =readAnalogData(SPI_ADC_POTEN_CHANNEL);
        tone = adcValue/2+200;
        printf("Potentionmeter value = %d, tone = %d\n", adcValue, tone);
        
        if(key=getch()){
            switch(key){
                case 'q':
                    softToneWrite(BUZ, tone);
                    digitalWrite(LED, HIGH);
                    delay(200);
                    softToneWrite(BUZ, 0);
                    digitalWrite(LED, LOW);
                    break;
                case 'w':
                    softToneWrite(BUZ, tone);
                    digitalWrite(LED, HIGH);
                    break;
                case 'r':
                    softToneWrite(BUZ, 0);
                    digitalWrite(LED, LOW);
                    break;
            }
        }
    }
}

int spiSetup(){
    if(wiringPiSPISetup(SPI_CE_CHANNEL, SPI_SPEED) == -1){
        fprintf(stderr, "wiringPiSetup Failed ! ERROR: %s\n", strerror(errno));
        return 1;
    }
}

int readAnalogData(int adcChannel){
    int adcValue = 0;
    unsigned char buffer[3] = {0};
    if(adcChannel<0 || adcChannel>7)
        return 1;
    buffer[0]=0x01;
    buffer[1]=(CHAN_CONFIG_SINGLE+adcChannel)<<4;
    digitalWrite(CS_MCP3208, LOW);
    wiringPiSPIDataRW(SPI_CE_CHANNEL, buffer, 3);
    adcValue = ((buffer[1]&3)<< 8)+buffer[2];
    digitalWrite(CS_MCP3208, HIGH);
    return adcValue;
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