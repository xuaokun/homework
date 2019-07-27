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
    int readyCount,i,j;
    char cmd[20]={0};
    train_t train;
    connectInfo_t cInfo[5];
    int cnt=0;
    while(1){
        readyCount=epoll_wait(epfd,evs,21,0);
        for(i=0;i<readyCount;i++){
            if(evs[i].data.fd==socketFd){
                newFd=accept(socketFd,NULL,NULL);//建立一个新连接生成描述符
 #ifdef DEBUG
                printf("client comes\n");
 #endif         
                cInfo[cnt].fd=newFd;
                int ret=handleLogin(&cInfo[cnt]);
                if(0==ret){
                    epollInAdd(epfd,newFd);
                    cnt++;
                    continue;
                }
                else{
                    bzero(&cInfo[cnt],sizeof(connectInfo_t));
                    close(newFd);
                    continue;
                }
            }
            else{
                newFd=evs[i].data.fd;
                for(j=0;j<cnt;j++){
                    if(cInfo[j].fd==newFd){
                        break;
                    }
                }
                bzero(&train,sizeof(train));
                recvCycle(newFd,&train.dataLen,4);
                recvCycle(newFd,train.buf,train.dataLen);
                
                bzero(cmd,sizeof(cmd));
                memcpy(cmd,train.buf,train.dataLen);
                generateLog(cInfo[j].userName,cmd);
                #ifdef DEBUG
                printf("服务器开始处理命令:%s\n",cmd);
                #endif
                handleCmd(newFd,cmd,&pthreadPollInfo);
            }
        }
    }
} 