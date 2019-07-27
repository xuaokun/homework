#include "factory.h"
void *threadHandle(void *pInfo){
    ppthread_pool_info_t pIn=(ppthread_pool_info_t)pInfo;
    pQueue_t pQ=&pIn->que;
    pFdNode_t pGet;
    int getSuccessFlag=-1;
    while(1){
        pthread_mutex_lock(&pQ->mutex);
        if(!pQ->queSize){
            #ifdef DEBUG
            printf("我睡觉了\n");
            #endif 
            pthread_cond_wait(&pIn->cond,&pQ->mutex);
        }
        #ifdef DEBUG
        printf("我要工作了\n");
        #endif 
        getSuccessFlag=queGet(pQ,&pGet);
        printf("flag:%d\n",getSuccessFlag);
        pthread_mutex_unlock(&pQ->mutex);
        #ifdef DEBUG
        printf("fd:%d\n",pGet->newFd);
        #endif 
        if(!getSuccessFlag){
            sendOrRecv(pGet->newFd,pGet->cmd);
            #ifdef DEBUG
            printf("我完成工作了\n");
            #endif 
            epollInAdd(pIn->epfd,pGet->newFd);
            #ifdef DEBUG
            printf("add fd:%d\n",pGet->newFd);
            #endif
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
int sendOrRecv(int fd,char *cmd){
    chdir("../../cloudFile");
    char buf[20]={0};
    char fileName[20]={0};
    if(memcpy(buf,cmd,4),strcmp(buf,"gets")==0){
        strcpy(fileName,cmd+5);
        #ifdef DEBUG
        printf("send file:%s\n",fileName);
        #endif
        transFile(fd,fileName);
    }
    else{
        #ifdef DEBUG
        printf("recv file\n");
        #endif
        recvFile(fd);
    }
    return 0;
}