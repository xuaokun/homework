#include <func.h>
#define N 20000000
typedef struct{
    pthread_mutex_t mutex;
    int val;
}data_t;

void cleanup1(void *p){
    printf("I am cleanup1\n");
}
void cleanup2(void *p){
    printf("I am cleanup2\n");
}
void *childPthread(void *p){
    data_t *pval=(data_t *)p;
    for(int i=0;i<N;i++){
        pthread_mutex_lock(&pval->mutex);
        pval->val+=1;
        pthread_mutex_unlock(&pval->mutex);
    }
    pthread_exit(NULL);
}
int main()
{
    pthread_t pthid;
    data_t t;
    t.val=0;
    int ret=pthread_create(&pthid,NULL,childPthread,&t);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    ret=pthread_mutex_init(&t.mutex,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_mutex_init");
    for(int i=0;i<N;i++){
        pthread_mutex_lock(&t.mutex);
        t.val+=1;
        pthread_mutex_unlock(&t.mutex);
    }
    ret=pthread_join(pthid,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    printf("I am main thread,%d\n",t.val);
    return 0;
}

