#include <func.h>
typedef struct{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int tickets;
}data_t,*pData_t;
void *th1(void*p){
  pData_t pNew=(pData_t)p;
  while(1){
      pthread_mutex_lock(&pNew->mutex);
      if(pNew->tickets>0){
          pNew->tickets--;
          if(0==pNew->tickets){
              pthread_cond_signal(&pNew->cond);
          }
          printf("I am thread1,the left tickets=%d\n",pNew->tickets);
          pthread_mutex_unlock(&pNew->mutex);
          sleep(2);
      }
      else{
          printf("I am thread1,the left tickets=%d\n",pNew->tickets);
          pthread_mutex_unlock(&pNew->mutex);
          break;
      }
  }
}

void *th2(void*p){
  pData_t pNew=(pData_t)p;
  while(1){
      pthread_mutex_lock(&pNew->mutex);
      if(pNew->tickets>0){
          pNew->tickets--;
          if(0==pNew->tickets){
              pthread_cond_signal(&pNew->cond);
          }
          printf("I am thread2,the left tickets=%d\n",pNew->tickets);
          pthread_mutex_unlock(&pNew->mutex);
          sleep(2);
      }
      else{
          printf("I am thread2,the left tickets=%d\n",pNew->tickets);
          pthread_mutex_unlock(&pNew->mutex);
              break;
      }
  }
}
void *th3(void*p){
    pData_t pNew=(pData_t)p;
    pthread_mutex_lock(&pNew->mutex);
    if(pNew->tickets>0){
        pthread_cond_wait(&pNew->cond,&pNew->mutex);
    }
    pNew->tickets=20;
    pthread_mutex_unlock(&pNew->mutex);
}
int main()
{
    pData_t pd;
    pd->tickets=20;
    pthread_t id1,id2,id3;
    pthread_mutex_init(&pd->mutex,NULL);
    pthread_cond_init(&pd->cond,NULL);
    int ret=pthread_create(&id1,NULL,th1,pd);
    THREAD_ERROR_CHECK(ret,"pthread_creat");
    ret=pthread_create(&id2,NULL,th2,pd);
    THREAD_ERROR_CHECK(ret,"pthread_creat");
    ret=pthread_create(&id3,NULL,th3,pd);
    THREAD_ERROR_CHECK(ret,"pthread_creat");
    ret=pthread_join(id1,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    ret=pthread_join(id2,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    return 0;
}

