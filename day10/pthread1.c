#include <func.h>
void *threadFunc(void *p){
    printf("I am child pthread:%d\n",*(int *)p);
    pthread_exit((void *)2);
}
int main()
{
    pthread_t pid;
    int val=1;
    int ret=pthread_create(&pid,NULL,threadFunc,&val);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    long v;
    ret= pthread_join(pid,(void **)&v);
    THREAD_ERROR_CHECK(ret,"pthread_join");
     printf("I am main thread,%ld\n",v);
    return 0;
}

