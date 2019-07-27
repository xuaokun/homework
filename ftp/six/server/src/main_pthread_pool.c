#include "factory.h"
int main(){
    pthread_pool_info_t pthreadPollInfo;
    factoryInit(&pthreadPollInfo,PTHREADNUM,PTHREADCAPACITY);
    factoryStart(&pthreadPollInfo);
 #ifdef DEBUG   
    printf("pthread all start!\n"); 
 #endif
    connectSql();
    int socketFd,confFd=0;
    char IP[16]={0};
    char port[5]={0};
    fetchIPInfo(confFd,IP,port);
#ifdef DEBUG
    printf("IP:%s,port:%s\n",IP,port);
#endif
    tcpInit(&socketFd,IP,port);
    int newFd;
    struct epoll_event evs[21]; 
    int epfd=epoll_create(1);
    pthreadPollInfo.epfd=epfd;
    epollInAdd(epfd,socketFd);
    int readyCount,i;
    char cmd[20]={0};
    train_t train;
    pConInfoQueue_t pConInfo=(pConInfoQueue_t)calloc(1,sizeof(conInfoQueue_t));
    bzero(pConInfo,sizeof(conInfoQueue_t));
    pConInfo->pHead=pConInfo->pTail=NULL;
    while(1){
        readyCount=epoll_wait(epfd,evs,21,-1);
#ifdef DEBUG
        printf("主线程监听到信号了\n");
 #endif
        for(i=0;i<readyCount;i++){
            if(evs[i].data.fd==socketFd){
                newFd=accept(socketFd,NULL,NULL);//建立一个新连接生成描述符
 #ifdef DEBUG
                printf("client comes\n");
 #endif         
                pConnectInfo_t pNew=(pConnectInfo_t)calloc(1,sizeof(connectInfo_t));
                pNew->fd=newFd;
                printf("开始登陆验证\n");
                int ret=handleLogin(pNew);
                if(0==ret){
                    epollInAdd(epfd,newFd);
                    
                    connectInfoInsert(pConInfo,pNew);
                    continue;
                }
                else{
                    connectInfoDelete(pConInfo,pNew);
                    close(newFd);
                    continue;
                }
            }
            else{
                newFd=evs[i].data.fd;
                pConnectInfo_t pCurrentCon=pConInfo->pHead;
                while(pCurrentCon){
                    if(pCurrentCon->fd==newFd){
                        break;
                    }
                    pCurrentCon=pCurrentCon->pNext;
                }
                bzero(&train,sizeof(train));
                recvCycle(newFd,&train.dataLen,4);
                #ifdef DEBUG
                printf("datalen:%d\n",train.dataLen);
                #endif
                recvCycle(newFd,train.buf,train.dataLen);
                bzero(cmd,sizeof(cmd));
                memcpy(cmd,train.buf,train.dataLen);
                generateLog(pCurrentCon->userName,cmd);
                #ifdef DEBUG
                printf("服务器开始处理命令:%s\n",cmd);
                #endif
                handleCmd(pCurrentCon,cmd,&pthreadPollInfo);
                #ifdef DEBUG
                printf("主线程处理完命令了\n");
                #endif
            }
        }
    }
} 