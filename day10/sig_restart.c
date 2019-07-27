#include <func.h>
void handle(int num,siginfo_t *p,void *p1){
    printf("signum:%d\n",num);
    sleep(3);
    printf("after sleep signum:%d\n",num);
}
int main()
{
    struct sigaction act;
    bzero(&act,sizeof(act));
    act.sa_sigaction=handle;
    act.sa_flags=SA_SIGINFO|SA_RESTART;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGQUIT);
    sigaction(SIGINT,&act,NULL);
    char buf[128]={0};
    int ret=read(STDIN_FILENO,buf,sizeof(buf));
    printf("ret=%d,buf=%s\n",ret,buf);
    while(1);
    return 0;
}

