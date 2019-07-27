#include <func.h>
#define N 20000000
pthread_mutex_t mutex;
int val=0;
void *childPthread(void *p){
    for(int i=0;i<N;i++){
        pthread_mutex_lock(&mutex);
        val+=1;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
int main()
{
    pthread_t pthid;
    int ret=pthread_create(&pthid,NULL,childPthread,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    ret=pthread_mutex_init(&mutex,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_mutex_init");
    for(int i=0;i<N;i++){
        pthread_mutex_lock(&mutex);
        val+=1;
        pthread_mutex_unlock(&mutex);
    }
    ret=pthread_join(pthid,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    printf("I am main thread,%d\n",val);
    return 0;
}

