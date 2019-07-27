#include <func.h>
void handle(int signum,siginfo_t *pinfo,void *p){
    printf("sig%d is coming.\n",signum);
    sleep(3);
    sigset_t pending;
    sigpending(&pending);
    if(sigismember(&pending,SIGQUIT)){
        printf("SIGQUIT is pending.\n");
    }
    else{
        printf("SIGUIT is not pending.\n");
    }
    printf("after sleep:sig%d is coming.\n",signum);
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

