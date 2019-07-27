#include <func.h>

int main()
{
    printf("父进程ID:%d\n",getpid());
    pid_t ret=fork();
    if(ret<0){
        printf("creat fail!\n");
    }
    else if(ret==0){
        printf("子进程ID:%d\n",getpid());
        while(1);
    }
    else{
        while(1);
    }
    return 0;
}

