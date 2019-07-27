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
            sendOrRecv(pGet->newFd,pGet->cmd);
            epollInAdd(pIn->epfd,pGet->newFd);
            // struct epoll_event event;
            // event.events=EPOLLIN;
            // event.data.fd=pGet->newFd;
            // #ifdef DEBUG
            // printf("epfd:%d\n",pIn->epfd);
            // #endif
            // epoll_ctl(pIn->epfd,EPOLL_CTL_ADD,pGet->newFd,&event);
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