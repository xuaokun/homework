#include <func.h>
void handle(int num,siginfo_t *p,void *p1){
    printf("signum:%d\n",num);
}
int main()
{
    struct sigaction act;
    bzero(&act,sizeof(act));
    act.sa_sigaction=handle;
    act.sa_flags=SA_SIGINFO;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGQUIT);
    sigaction(SIGINT,&act,NULL);
    while(1);
    return 0;
}

