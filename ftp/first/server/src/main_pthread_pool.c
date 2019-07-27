#include "factory.h"

int main(){
    pthread_pool_info_t pthreadPollInfo;
    factoryInit(&pthreadPollInfo,PTHREADNUM,PTHREADCAPACITY);
    factoryStart(&pthreadPollInfo);
 #ifdef DEBUG   
    printf("pthread all start!\n"); 
 #endif
    int socketFd,confFd=0;
    char IP[16]={0};
    char port[5]={0};
    fetchIPInfo(confFd,IP,port);
#ifdef DEBUG
    printf("IP:%s,port:%s\n",IP,port);
#endif
    tcpInit(&socketFd,IP,port);
    int newFd;
    pQueue_t pQue=&pthreadPollInfo.que;
    struct epoll_event evs[1]; 
    int epfd=epoll_create(1);
    epollInAdd(epfd,socketFd);
    int readyCount,i;
    while(1){
        readyCount=epoll_wait(epfd,evs,2,0);
        for(i=0;i<readyCount;i++){
            if(evs[i].data.fd==socketFd){
                 newFd=accept(socketFd,NULL,NULL);//建立一个新连接生成描述符
 #ifdef DEBUG
                 printf("client comes\n");
 #endif                
                pFdNode_t pNew=(pFdNode_t)calloc(1,sizeof(pFdNode_t));
                pNew->newFd=newFd;
                pthread_mutex_lock(&pQue->mutex);
                queInsert(pQue,pNew);//互斥进行队列中描述符的插入
                pthread_mutex_unlock(&pQue->mutex);
                pthread_cond_signal(&pthreadPollInfo.cond);//每次新增一个描述符就唤醒一个线程
            }
        }
    }
} 