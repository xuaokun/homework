#include "factory.h"
void *threadHandle(void *pInfo){
    ppthread_pool_info_t pIn=(ppthread_pool_info_t)pInfo;
    pQueue_t pQ=&pIn->que;
    pFdNode_t pGet;
    int getSuccessFlag;
    while(1){
        pthread_mutex_lock(&pQ->mutex);
        if(!pQ->queSize){
            pthread_cond_wait(&pIn->cond,&pQ->mutex);
        }
        getSuccessFlag=queGet(pQ,&pGet);
        pthread_mutex_unlock(&pQ->mutex);
        if(!getSuccessFlag){
            handleClient(pGet->newFd);
            free(pGet);
            pGet=NULL;
        }
    }
}
 int factoryInit(ppthread_pool_info_t pInfo,int num,int cap){
    queInit(&pInfo->que,cap);
    pInfo->pthreadNum=num;
    pthread_cond_init(&pInfo->cond,NULL);
    pInfo->pthID=(pthread_t*)calloc(num,sizeof(pthread_t));
    return 0;
}
int factoryStart(ppthread_pool_info_t pInfo){
    for(int i=0;i<pInfo->pthreadNum;i++){
        pthread_create(pInfo->pthID+i,NULL,threadHandle,pInfo);
    }
    return 0;
}