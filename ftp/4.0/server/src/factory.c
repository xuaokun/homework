#include "../include/factory.h"

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
            sendOrRecv(pGet);
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
int connectInfoInsert(pConInfoQueue_t pConInfo,pConnectInfo_t pNew){
    if(NULL==pConInfo->pHead){
        pConInfo->pHead=pConInfo->pTail=pNew;
    }
    else{
        pConInfo->pTail->pNext=pNew;
        pConInfo->pTail=pNew;
    }
    pConInfo->queSize++;
    return 0;
}
int timeQueueDel(pConInfoQueue_t pConInfo,pConnectInfo_t pNew){
     pConnectInfo_t pTmp=pConInfo->pHead,pPre=pConInfo->pHead;
    if(pConInfo->queSize==0){
        return -1;
    }
    if(pTmp==pNew){
        pConInfo->pHead=pConInfo->pHead->pNext;
        if(NULL==pConInfo->pHead){
            pConInfo->pTail=NULL;
        }
        pConInfo->queSize--;
        return 0;
    }
    while(pTmp!=NULL){
        if(pTmp!=pNew){
            pPre=pTmp;
            pTmp=pTmp->pNext;
        }
        else{
            pPre->pNext=pTmp->pNext;
            break;
        }
    }
    if(NULL==pTmp){
        return -1;
    }
    pConInfo->queSize--;
    return 0;
}
int connectInfoDelete(pConInfoQueue_t pConInfo,pConnectInfo_t pNew){
    pConnectInfo_t pTmp=pConInfo->pHead,pPre=pConInfo->pHead;
    if(pConInfo->queSize==0){
        return -1;
    }
    if(pTmp==pNew){
        pConInfo->pHead=pConInfo->pHead->pNext;
        if(NULL==pConInfo->pHead){
            pConInfo->pTail=NULL;
        }
        free(pNew);
        pNew=NULL;
        pConInfo->queSize--;
        return 0;
    }
    while(pTmp!=NULL){
        if(pTmp!=pNew){
            pPre=pTmp;
            pTmp=pTmp->pNext;
        }
        else{
            pPre->pNext=pTmp->pNext;
            free(pNew);
            pNew=NULL;
            break;
        }
    }
    if(NULL==pTmp){
        return -1;
    }
    pConInfo->queSize--;
    return 0;
}
int sendOrRecv(pFdNode_t pNode){
    chdir("../../cloudFile");
    char buf[20]={0};
    if(memcpy(buf,pNode->cmd,4),strcmp(buf,"gets")==0){
        #ifdef DEBUG
        printf("send file:%s\n",pNode->fileMD);
        #endif
        transFile(pNode);
    }
    else{
        #ifdef DEBUG
        printf("recv file\n");
        #endif
        recvFile(pNode);
    }
    return 0;
}