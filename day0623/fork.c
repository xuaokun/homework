#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t pid=fork();
    if(0==pid){
        while(1);
    }
    else{
        exit(10);
    }
}

