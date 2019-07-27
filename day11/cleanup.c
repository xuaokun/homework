#include <func.h>
void cleanup1(void *p){
    printf("I am cleanup1\n");
}
void cleanup2(void *p){
    printf("I am cleanup2\n");
}
void *childPthread(void *p){
    printf("I am childpthread\n");
    pthread_cleanup_push(cleanup1,NULL);
    pthread_cleanup_push(cleanup2,NULL);
    sleep(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}
int main()
{
    pthread_t pthid;
    int ret=pthread_create(&pthid,NULL,childPthread,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    ret=pthread_join(pthid,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    return 0;
}

