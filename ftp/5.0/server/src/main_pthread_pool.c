#include "../include/factory.h"
int exitFds[2];//管道用于父子进程通信
void sigHandle(int signum){//10号信号触发的信号处理函数
    write(exitFds[1],&signum,1);
}
int main(){
    pipe(exitFds);
    while(fork()){//父进程用于防止线程池进程崩溃
    signal(SIGUSR1,sigHandle);//收到信号后通知子进程退出
    int status;
    pid_t pid;
    pid=wait(&status);//等待子进程退出，返回子进程ID
    if(WIFEXITED(status)){
        printf("thread_pool pid%d exited successfully with code:%d\n",pid,WEXITSTATUS(status));
        exit(0);
    }
}
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
    epollInAdd(epfd,exitFds[0]);//监控管道读端
    int readyCount,i;
    char cmd[20]={0};
    train_t train;
    conInfoQueue_t set[31];//记录各个秒数集合中的客户端描述符
    bzero(set,sizeof(set));
    int index=0;
    int mapID[CLIENTNUM+3];
    pConInfoQueue_t pConInfo=(pConInfoQueue_t)calloc(1,sizeof(conInfoQueue_t));//用于记录每个客户端连接的队列指针
    bzero(pConInfo,sizeof(conInfoQueue_t));
    pConInfo->pHead=pConInfo->pTail=NULL;
    int j;
    while(1){
        readyCount=epoll_wait(epfd,evs,CLIENTNUM+2,1000);
        for(i=0;i<readyCount;i++){
            if(evs[i].data.fd==exitFds[0]){//子进程从管道收到信号后依次退出
                for(j=0;j<PTHREADNUM;j++){
                    pthread_cancel(*(pthreadPollInfo.pthID+i));
                }
                for(j=0;j<PTHREADNUM;j++){
                    pthread_join(*(pthreadPollInfo.pthID+i),NULL);
                }
                printf("pthread exit ok\n");
                exit(0);
            }
            if(evs[i].data.fd==socketFd){//处理新的连接请求
                newFd=accept(socketFd,NULL,NULL);//建立一个新连接生成描述符
 #ifdef DEBUG
                printf("client comes,fd:%d\n",newFd);
 #endif         
                pConnectInfo_t pNew=(pConnectInfo_t)calloc(1,sizeof(connectInfo_t));
                pNew->fd=newFd;
                printf("开始登陆验证\n");
                int ret=handleLogin(pNew);//用户登录处理函数
                if(0==ret){
                    epollInAdd(epfd,newFd);//验证成功则将连接描述符加入到监听集合
                    connectInfoInsert(pConInfo,pNew);//将新的连接记录到队列
                    if(index==0){
                        connectInfoInsert(&set[30],pNew);
                        mapID[newFd]=30;
                    }
                    else{
                        connectInfoInsert(&set[index-1],pNew);
                        mapID[newFd]=index-1;
                    }
                    if(index==30){
                        index=0;
                    }
                    else{
                        index++;
                    }
                }
                else{
                    close(newFd);
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
                #ifdef DEBUG 
                printf("fd:%d\n",pCurrentCon->fd);
                #endif
                if(index==0){  
                    timeQueueDel(&set[mapID[newFd]],pCurrentCon);
                    connectInfoInsert(&set[30],pCurrentCon);
                    mapID[newFd]=30;
                }
                else{
                    #ifdef DEBUG 
                    printf("fd%d从%d秒集合中移到%d秒集合\n",newFd,mapID[newFd],index-1);
                    #endif
                    if(index-1!=mapID[newFd]){
                        timeQueueDel(&set[mapID[newFd]],pCurrentCon);
                        connectInfoInsert(&set[index-1],pCurrentCon);
                        mapID[newFd]=index-1; 
                    }

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
                if(index==30){
                    index=0;
                }
                else{
                    index++;
                }
                continue;
            }
        }
        if(0==readyCount){
            while(set[index].queSize!=0){
                close(set[index].pHead->fd);
                connectInfoDelete(&set[index],set[index].pHead);
                printf("close a client connection\n");
            }
            if(index==30){
                index=0;
                continue;
            }
            index++;
        }
    }
} 