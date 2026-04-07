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