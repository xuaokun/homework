#include <func.h>
void handle(int signum,siginfo_t *pinfo,void *p){
    printf("sig%d is coming,send pid:%d,send uid:%d\n",signum,pinfo->si_pid,pinfo->si_uid);
    sleep(3);
}
int main()
{
    struct sigaction act;
    act.sa_flags=SA_SIGINFO|SA_NODEFER;
    act.sa_sigaction=handle;
    int ret=sigemptyset(&act.sa_mask);
    ERROR_CHECK(ret,-1,"sigemptyset");
    sigaddset(&act.sa_mask,SIGQUIT);
    sigaction(SIGINT,&act,NULL);
    while(1);
    return 0;
}

