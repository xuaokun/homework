#include <func.h>
void cleanup(void *p){
    free(p);
    printf("free success!\n");
}
void *pthFunc(void *p){
    p=malloc(20);
    pthread_cleanup_push(cleanup,p);
    read(STDIN_FILENO,p,sizeof(p));
    pthread_cleanup_pop(1);
    return NULL;
}
int main()
{
    pthread_t pid;
    int ret=pthread_create(&pid,NULL,pthFunc,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    ret=pthread_cancel(pid);
    THREAD_ERROR_CHECK(ret,"pthread_cancel");
    long threadRet;
    ret=pthread_join(pid,(void**)&threadRet);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    printf("I am main thread:%ld\n",threadRet);
    return 0;
}

