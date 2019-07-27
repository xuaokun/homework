#include "factory.h"
void cleanup(void *pq){
    pQue_t pQue=(pQue_t)pq;
    pthread_mutex_unlock(&pQue->mutex);
}
void *threadFunc(void *p){
    pFactory_t pThreadInfo=(pFactory_t)p;
    pQue_t pQue=&pThreadInfo->que;
    pNode_t pGet;
    int getSuccessFlag;
    while(1){
        pthread_mutex_lock(&pQue->mutex);//多线程抢锁
        pthread_cleanup_push(cleanup,pQue);
        if(!pQue->queSize){
            pthread_cond_wait(&pThreadInfo->cond,&pQue->mutex);//若请求队列为空则阻塞
        }
        getSuccessFlag=queGet(pQue,&pGet);//存在一种可能：阻塞的线程正在重新加锁时另一线程已经进入拿到任务，因此此处用返回值判断是否拿到任务
        pthread_cleanup_pop(1);
        if(!getSuccessFlag){
            transFile(pGet->newfd);
            free(pGet);
            pGet=NULL; 
        }
    }
}
int  factoryInit(pFactory_t p,int pthreadNum,int pthreadCapacity){
    queInit(&p->que,pthreadCapacity);
    p->pthreadNum=pthreadNum;
    pthread_cond_init(&p->cond,NULL);//条件变量初始化
    p->pthID=(pthread_t *)calloc(pthreadNum,sizeof(pthread_t));//申请存储线程ID的空间  
    p->startflag=0;//标志设为未启动
    return 0;
}
int factoryStart(pFactory_t pF){
    if(!pF->startflag){
        for(int i=0;i<pF->pthreadNum;i++){
            pthread_create(pF->pthID+i,NULL,threadFunc,pF);
        }
        pF->startflag=1;
    }
    return 0;
}