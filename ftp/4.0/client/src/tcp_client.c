#include "../include/client.h"
struct sockaddr_in serAdd;
char userName[20]={0};
char dirHead[50]={0};
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
        regAccount(socketFd,userName);
    }
    else if(flag=='1'){
        send(socketFd,&flag,1,0);
        loginAccount(socketFd,userName);
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
    char fileName[20]={0};
    int MdFlag,rmFlag;
    train_t train;
    pthread_t pthId;
    train.dataLen=3;
    strcpy(train.buf,"pwd");
    send(socketFd,&train,4+train.dataLen,0);
    bzero(&train,sizeof(train));
    recvCycle(socketFd,&train.dataLen,4);
    recvCycle(socketFd,dirHead,train.dataLen);
    while(1){
        printf("[\033[1;32;40m%s\033[0m@\033[1;34;40m%s\033[0m]$",userName,dirHead);
        fflush(stdout);
        readyCount=epoll_wait(epfd,evs,2,-1);
        for(i=0;i<readyCount;i++){
            if(evs[i].data.fd==STDIN_FILENO){
                bzero(buf,sizeof(buf));
                bzero(&train,sizeof(train));
                read(STDIN_FILENO,buf,sizeof(buf));
                train.dataLen=strlen(buf)-1;
                if(!train.dataLen){
                    break;
                }
                memcpy(train.buf,buf,train.dataLen);
                if(strcmp(train.buf,"pwd")==0){
                    send(socketFd,&train,4+train.dataLen,0);
                    bzero(&train,sizeof(train));
                    recvCycle(socketFd,&train.dataLen,4);
                    recvCycle(socketFd,train.buf,train.dataLen);
                    printf("%s\n",train.buf);
                }
                else if(train.buf[0]=='c'&&train.buf[1]=='d'){
                    send(socketFd,&train,4+train.dataLen,0);
                    recvCycle(socketFd,&train.dataLen,4);
                    bzero(dirHead,sizeof(dirHead));
                    recvCycle(socketFd,dirHead,train.dataLen);
                }
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,2),(strcmp(buf,"ls")==0)){
                    send(socketFd,&train,4+train.dataLen,0);
                    //接收指定目录的文件信息并显示
                    bzero(&train,sizeof(train));
                    recvCycle(socketFd,&train.dataLen,4);
                    recvCycle(socketFd,train.buf,train.dataLen);
                    printf("%20s %15s %8s %20s\n","filename","filesize","filetype","owner");
                    printf("%s\n",train.buf);
                }
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,5),(strcmp(buf,"mkdir")==0)){
                    send(socketFd,&train,4+train.dataLen,0);
                }
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,6),(strcmp(buf,"remove")==0)){
                    send(socketFd,&train,4+train.dataLen,0);
                }
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,5),(strcmp(buf,"rmdir")==0)){
                    send(socketFd,&train,4+train.dataLen,0);
                    recvCycle(socketFd,&rmFlag,sizeof(int));
                    if(rmFlag==-1){
                        printf("you can't delete this directory\n");
                        continue;
                    }
                }
                //实现长命令分离
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,4),(strcmp(buf,"gets")==0||strcmp(buf,"puts")==0)){
                    #ifdef DEBUG
                    printf("gets or puts cmd\n");
                    #endif
                    struct stat state;
                    strcpy(fileName,train.buf+5);
                    if(strcmp(buf,"puts")==0){//若为puts，先对MD进行检测看是否存在该文件，若存在再进行发送命令
                        ret=checkFile(fileName);
                        if(ret==-1){
                            printf("未查找到要上传的文件\n");
                            continue;
                        }
                        //检查服务该目录下是否含有同名文件  
                        send(socketFd,&train,4+train.dataLen,0);
                        train.dataLen=strlen(fileName);
                        memcpy(train.buf,fileName,train.dataLen);
                        send(socketFd,&train,train.dataLen+4,0);
                        recvCycle(socketFd,&MdFlag,sizeof(MdFlag));
                        if(MdFlag!=1){
                            printf("there is a file having a same name\n");
                            continue;
                        }
                        //实现秒传功能
                        computerFileMD5(fileName,fileMD);
                        #ifdef DEBUG
                        printf("file MD5:%s\n",fileMD);
                        #endif
                        send(socketFd,fileMD,32,0);
                        recvCycle(socketFd,&MdFlag,sizeof(MdFlag));
                        if(1==MdFlag){
                            train.dataLen=strlen(fileName);
                            memcpy(train.buf,fileName,train.dataLen);
                            send(socketFd,&train,train.dataLen+4,0);
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
                        int fileFd=open(fileName,O_RDONLY);
                        if(fileFd!=-1){
                            printf("there is part of file,do you want to continue get it from last point(y) or get a new file From server(n)?y or n:");
                            fflush(stdout);
                            char flag;
                label1:     getchar();
                            scanf("%c",&flag);
                            if(flag=='y'){
                                fstat(fileFd,&state);
                                train.dataLen=sizeof(state.st_size);
                                close(fileFd);
                            }
                            else if('n'==flag){
                                train.dataLen=0;
                            }
                            else{
                                goto label1;
                            }
                        }
                        else{
                            train.dataLen=0;
                        }
                        send(socketFd,&train.dataLen,sizeof(int),0);
                        
                    }
                    int newSocketFd;
                    newSocketFd=socket(AF_INET,SOCK_STREAM,0);
                    ERROR_CHECK(newSocketFd,-1,"socket");
                    ret=connect(newSocketFd,(struct sockaddr*)&serAdd,sizeof(serAdd));
                    ERROR_CHECK(ret,-1,"connect");
                    pthread_create(&pthId,NULL,newHandle,(void *)&newSocketFd);
                    if(strcmp(buf,"gets")==0){
                        recvFile(socketFd,train.dataLen);
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
                else{
                    printf("you sent a error command\n");
                }
            }
        }
    }
    close(socketFd);
}
