#include "client.h"
struct sockaddr_in serAdd;

int main(int arc,char*argv[]){
    ARGS_CHECK(arc,3);
    int socketFd;
    socketFd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(socketFd,-1,"socket");
    
    bzero(&serAdd,sizeof(serAdd));
    serAdd.sin_family=AF_INET;
    serAdd.sin_port=htons(atoi(argv[2]));
    serAdd.sin_addr.s_addr=inet_addr(argv[1]);
    int ret=connect(socketFd,(struct sockaddr*)&serAdd,sizeof(serAdd));
    ERROR_CHECK(ret,-1,"connect");
    char flag;
    printf("注册请输入0，登录请输入1\n");
    scanf("%c",&flag);
    if(flag=='0'){
        // #ifdef DEBUG
        // printf("flag='0'\n");
        // #endif
        send(socketFd,&flag,1,0);
        regAccount(socketFd);
    }
    else if(flag=='1'){
        send(socketFd,&flag,1,0);
        loginAccount(socketFd);
    }
    int epfd=epoll_create(2);
    struct epoll_event event,evs[2];
    event.events=EPOLLIN;
    event.data.fd=socketFd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,socketFd,&event);
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
                send(socketFd,&train,4+train.dataLen,0);
                if(strcmp(train.buf,"pwd")==0){
                    bzero(&train,sizeof(train));
                    recvCycle(socketFd,&train.dataLen,4);
                    recvCycle(socketFd,train.buf,train.dataLen);
                    printf("%s\n",train.buf);
                }
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,4),strcmp(buf,"gets")==0){
                    #ifdef DEBUG
                    printf("gets cmd\n");
                    #endif
                    pthread_t pthId;
                    pthread_create(&pthId,NULL,newHandle,(void *)&serAdd);
                    recvFile(socketFd);
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
                    transFile(socketFd,fileName);
                    #ifdef DEBUG
                    printf("puts file success\n");
                    #endif  
                }
            }
        }
    }
    close(socketFd);
}
