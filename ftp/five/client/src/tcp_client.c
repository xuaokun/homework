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
reInput:
    scanf("%c",&flag);
    if(flag=='0'){
        send(socketFd,&flag,1,0);
        regAccount(socketFd);
    }
    else if(flag=='1'){
        send(socketFd,&flag,1,0);
        loginAccount(socketFd);
    }
    else{
        goto reInput;
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
    char fileMD[33]={0};
    int MdFlag;
    train_t train;
    pthread_t pthId;
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
                if(strcmp(train.buf,"pwd")==0){
                    send(socketFd,&train,4+train.dataLen,0);
                    bzero(&train,sizeof(train));
                    recvCycle(socketFd,&train.dataLen,4);
                    recvCycle(socketFd,train.buf,train.dataLen);
                    printf("%s\n",train.buf);
                }
                //实现长命令分离
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,4),(strcmp(buf,"gets")==0||strcmp(buf,"puts")==0)){
                    #ifdef DEBUG
                    printf("gets or puts cmd\n");
                    #endif
                    strcpy(fileMD,train.buf+5);
                    if(strcmp(buf,"puts")==0){//若为puts，先对MD进行检测看是否存在该文件，若存在再进行发送命令
                        ret=checkFile(fileMD);
                        if(ret==-1){
                            printf("未查找到要上传的文件\n");
                            continue;
                        }  
                        send(socketFd,&train,4+train.dataLen,0);
                        computerFileMD5(fileMD,fileMD);
                        #ifdef DEBUG
                        printf("file MD5:%s\n",fileMD);
                        #endif
                        send(socketFd,fileMD,32,0);
                        recvCycle(socketFd,&MdFlag,sizeof(MdFlag));
                        if(1==MdFlag){
                            printf("上传成功\n");
                            continue;
                        }
                    }
                    else{
                        int fileFlag;
                        send(socketFd,&train,4+train.dataLen,0);
                        recvCycle(socketFd,&fileFlag,sizeof(int));
                        if(1==fileFlag){
                            printf("服务器上无此文件\n");
                            continue;
                        }
                    }
                    int newSocketFd;
                    newSocketFd=socket(AF_INET,SOCK_STREAM,0);
                    ERROR_CHECK(newSocketFd,-1,"socket");
                    ret=connect(newSocketFd,(struct sockaddr*)&serAdd,sizeof(serAdd));
                    ERROR_CHECK(ret,-1,"connect");
                    pthread_create(&pthId,NULL,newHandle,(void *)&newSocketFd);
                    if(strcmp(buf,"gets")==0){
                        recvFile(socketFd);
                        pthread_cancel(pthId);
                    }
                    else{
                        char fileName[20]={0};
                        strcpy(fileName,train.buf+5);
                        transFile(socketFd,fileName,fileMD);
                        #ifdef DEBUG
                        printf("puts file success\n");
                        #endif
                        pthread_cancel(pthId);    
                    }
                }
            }
        }
    }
    close(socketFd);
}
