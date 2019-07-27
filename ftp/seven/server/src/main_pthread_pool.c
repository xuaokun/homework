#include "factory.h"
int main(){
    pthread_pool_info_t pthreadPollInfo;//用于存储线程池信息
    factoryInit(&pthreadPollInfo,PTHREADNUM,PTHREADCAPACITY);//线程初始化
    factoryStart(&pthreadPollInfo);//线程启动
 #ifdef DEBUG   
    printf("pthread all start!\n"); 
 #endif
    connectSql();//服务器开启即连接sql
    int socketFd,confFd=0;
    char IP[16]={0};
    char port[5]={0};
    fetchIPInfo(confFd,IP,port);//从配置文件获得IP和端口号
#ifdef DEBUG
    printf("IP:%s,port:%s\n",IP,port);
#endif
    tcpInit(&socketFd,IP,port);//创建socket描述符并监听
    int newFd;
    struct epoll_event evs[21]; 
    int epfd=epoll_create(1);
    pthreadPollInfo.epfd=epfd;
    epollInAdd(epfd,socketFd);
    int readyCount,i;
    char cmd[20]={0};
    train_t train;
    pConInfoQueue_t pConInfo=(pConInfoQueue_t)calloc(1,sizeof(conInfoQueue_t));//用于记录每个客户端连接的队列指针
    bzero(pConInfo,sizeof(conInfoQueue_t));
    pConInfo->pHead=pConInfo->pTail=NULL;
    while(1){
        readyCount=epoll_wait(epfd,evs,21,-1);
#ifdef DEBUG
        printf("主线程监听到信号了\n");
 #endif
        for(i=0;i<readyCount;i++){
            if(evs[i].data.fd==socketFd){//处理新的连接请求
                newFd=accept(socketFd,NULL,NULL);//建立一个新连接生成描述符
 #ifdef DEBUG
                printf("client comes\n");
 #endif         
                pConnectInfo_t pNew=(pConnectInfo_t)calloc(1,sizeof(connectInfo_t));
                pNew->fd=newFd;
                printf("开始登陆验证\n");
                int ret=handleLogin(pNew);//用户登录处理函数
                if(0==ret){
                    epollInAdd(epfd,newFd);//验证成功则将连接描述符加入到监听集合
                    connectInfoInsert(pConInfo,pNew);//将新的连接记录到队列
                    continue;
                }
                else{
                    connectInfoDelete(pConInfo,pNew);
                    close(newFd);
                    continue;
                }
            }
            else{//处理客户端命令
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