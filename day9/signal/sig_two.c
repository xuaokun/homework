#include <func.h>
void sigFunc(int signum){
    printf("before sleep:sig%d is coming.\n",signum);
    sleep(3);
    printf("after sleep:sig%d is coming.\n",signum);
}
int main()
{
    if(signal(SIGINT,sigFunc)==SIG_ERR){
        perror("signal");
        return -1;
    }
    signal(SIGQUIT,sigFunc);
    while(1);
    return 0;
}

