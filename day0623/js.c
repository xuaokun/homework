#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    pid_t pid=fork();
    if(0==pid){
        exit(10);
    }
    else{
        wait(NULL);
        sleep(10);
    }
    return 0;
}

