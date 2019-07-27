#include <func.h>
long  times=0;
void sigFunc(int num){
    time_t now;
    time(&now);
    printf("the current time:%s,caculate times=%ld\n",ctime(&now),times);
    times=0;
}
int main()
{
    signal(SIGALRM,sigFunc);
    struct itimerval t;
    bzero(&t,sizeof(t));
    t.it_value.tv_sec=1;
    t.it_interval.tv_sec=1;
   int ret= setitimer(ITIMER_REAL,&t,NULL);
    ERROR_CHECK(ret,-1,"setitimer");
    double d,f;
    while(1){
        f=123.456;
        d=789.123*f;
        times++;
    }
    return 0;
}

