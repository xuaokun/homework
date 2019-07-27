#include <func.h>

int main()
{
    sigset_t procmask;
    sigemptyset(&procmask);
    sigaddset(&procmask,SIGINT);
    sigprocmask(SIG_BLOCK,&procmask,NULL);
    printf("you can't capture the sig2 next 5s\n");
    sleep(5);
    sigset_t pending;
    sigpending(&pending);
    if(sigismember(&pending,SIGINT)){
        printf("sig2 is pending.\n");
    }
    else{
        printf("sig2 is not pending.\n");
    }
    sigprocmask(SIG_UNBLOCK,&procmask,NULL);
    return 0;
}

