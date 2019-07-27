#include "client.h"
void *newHandle(void *p){
    struct sockaddr* pServerAddr=(struct sockaddr*)p;
    int newSocketFd;
    newSocketFd=socket(AF_INET,SOCK_STREAM,0);
    CHILD_THREAD_ERROR_CHECK(newSocketFd,"socket");
    int ret=connect(newSocketFd,pServerAddr,sizeof(struct sockaddr));
    CHILD_THREAD_ERROR_CHECK(ret,"connect");
        int epfd=epoll_create(2);
    struct epoll_event event,evs[2];
    event.events=EPOLLIN;
    event.data.fd=newSocketFd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,newSocketFd,&event);
    event.events=EPOLLIN;
    event.data.fd=STDIN_FILENO;
    epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);
    int readyCount,i;
    char buf[20]={0};
    train_t train;
    while(1){
        readyCount=epoll_wait(epfd,evs,2,0);
        for(i=0;i<readyCount;i++){
            if(evs[i].data.fd==STDIN_FILENO){
                bzero(buf,sizeof(buf));
                bzero(&train,sizeof(train));
                read(STDIN_FILENO,buf,sizeof(buf));
                train.dataLen=strlen(buf)-1;
                if(!train.dataLen){
                    break;
                }
                #ifdef DEBUG
                printf("命令长度%d\n",train.dataLen);
                #endif
                memcpy(train.buf,buf,train.dataLen);
                send(newSocketFd,&train,4+train.dataLen,0);
                if(strcmp(train.buf,"pwd")==0){
                    bzero(&train,sizeof(train));
                    recvCycle(newSocketFd,&train.dataLen,4);
                    recvCycle(newSocketFd,train.buf,train.dataLen);
                    printf("%s\n",train.buf);
                }
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,4),strcmp(buf,"gets")==0){
                    #ifdef DEBUG
                    printf("gets cmd\n");
                    #endif
                    pthread_t pthId;
                    pthread_create(&pthId,NULL,newHandle,(void *)&pServerAddr);
                    recvFile(newSocketFd);
                    #ifdef DEBUG
                    printf("gets file\n");
                    #endif  
                }
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,4),strcmp(buf,"puts")==0){
                    #ifdef DEBUG
                    printf("puts cmd success\n");
                    #endif
                    char fileName[20]={0};
                    strcpy(fileName,train.buf+5);
                    transFile(newSocketFd,fileName);
                    #ifdef DEBUG
                    printf("puts file success\n");
                    #endif  
                }
            }
        }
    }
    close(newSocketFd);
    return NULL;
}