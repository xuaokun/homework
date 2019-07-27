#include <func.h>
void handle(int signum,siginfo_t *pinfo,void *p){
    printf("sig%d is coming.\n",signum);
    sleep(3);
    printf("after sleep:sig%d is coming.\n",signum);
}
int main()
{
    struct sigaction act;
    act.sa_flags=SA_SIGINFO|SA_NODEFER;
    act.sa_sigaction=handle;
    sigaction(SIGINT,&act,NULL);
    while(1);
    return 0;
}

