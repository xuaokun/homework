#include <func.h>

void *childPthread(void *p){
    time_t now;
    time(&now);
    char*pT=ctime(&now);
    printf("I am childthread,the time is %s\n",pT);
    sleep(1);
    printf("I am childthread,the time is %s\n",pT);
    pthread_exit(NULL);
}
int main()
{
    pthread_t pthid;
    int ret=pthread_create(&pthid,NULL,childPthread,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    THREAD_ERROR_CHECK(ret,"pthread_mutex_init");
    sleep(1);
     time_t now;
    time(&now);
    char *p=ctime(&now);
    printf("I am main thread p=%s\n",p);
    ret=pthread_join(pthid,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    return 0;
}

