#include <func.h>
typedef struct{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}data_t,*pData_t;
void cleanup(void *p){
    pData_t pNew=(pData_t)p;
    printf("I am cleanup1\n");
    pthread_mutex_unlock(&pNew->mutex);
}
void *childPthread1(void *p){
    pData_t pNew=(pData_t) p;
    printf("I am childpthread1\n");
    pthread_mutex_lock(&pNew->mutex);
    pthread_cleanup_push(cleanup,p);
    pthread_cond_wait(&pNew->cond,&pNew->mutex);
    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}

void *childPthread2(void *p){
    pData_t pNew=(pData_t)p;
    printf("I am childpthread2\n");
    pthread_mutex_lock(&pNew->mutex);
    pthread_cleanup_push(cleanup,p);
    pthread_cond_wait(&pNew->cond,&pNew->mutex);
    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}

int main(){
    pthread_t pthid1,pthid2;
    pData_t pd;
    pthread_mutex_init(&pd->mutex,NULL);
    pthread_cond_init(&pd->cond,NULL);
    int ret=pthread_create(&pthid1,NULL,childPthread1,pd);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    ret=pthread_create(&pthid2,NULL,childPthread2,pd);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    sleep(1);
    pthread_cancel(pthid1);
    pthread_cancel(pthid2);
    ret=pthread_join(pthid1,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    ret=pthread_join(pthid2,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    printf("I am main thread.\n");
    return 0;
}