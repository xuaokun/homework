#include <func.h>

int main()
{
    pid_t pid=fork();
    if(pid==0){
        exit(10);
    }
    else
    {
        int status;
        pid_t pid=wait(&status);//等待子进程退出，否则出现僵尸进程
        if(WIFEXITED(status)){
            printf("Child process(pid:%d) exited with code %d\n",pid,WEXITSTATUS(status));
        }
        sleep(10);
    }
}

