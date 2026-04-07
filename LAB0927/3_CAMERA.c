#include <stdio.h>
//#include <string.h>
#include <stdlib.h>

int main(){
    while(1)
    {
        getchar();
        char cmd[] = "sudo libcamera-jpeg --width 800 --height 800 -t 1 -o test.jpeg";
        printf("%s \n", cmd);
        system(cmd);
        printf("Take a picture\n");
    }
    return 0;
}