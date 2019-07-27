#include "client.h"
extern char tocken[36];
extern struct sockaddr_in serAdd;
extern char userName[20];
void *newHandle(void *p){
    int newSocketFd=*(int *)p;
    train_t train;
    char checkFlag;
    //发送tocken
    send(newSocketFd,"3",1,0);
    train.dataLen=strlen(tocken);
    memcpy(train.buf,tocken,train.dataLen);
    send(newSocketFd,&train,4+train.dataLen,0);
    recvCycle(newSocketFd,&checkFlag,1);
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
    train.dataLen=3;
    char dirHead[50]={0};
    strcpy(train.buf,"pwd");
    send(newSocketFd,&train,4+train.dataLen,0);
    bzero(&train,sizeof(train));
    recvCycle(newSocketFd,&train.dataLen,4);
    recvCycle(newSocketFd,dirHead,train.dataLen);
    while(1){
        printf("[%s@%s]$",userName,dirHead);
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
                #ifdef DEBUG
                printf("命令长度%d\n",train.dataLen);
                #endif
                memcpy(train.buf,buf,train.dataLen);
                if(strcmp(train.buf,"pwd")==0){
                    send(newSocketFd,&train,4+train.dataLen,0);
                    bzero(&train,sizeof(train));
                    recvCycle(newSocketFd,&train.dataLen,4);
                    recvCycle(newSocketFd,train.buf,train.dataLen);
                    printf("%s\n",train.buf);
                }
                else if(train.buf[0]=='c'&&train.buf[1]=='d'){
                    send(newSocketFd,&train,4+train.dataLen,0);
                    recvCycle(newSocketFd,&train.dataLen,4);
                    bzero(dirHead,sizeof(dirHead));
                    recvCycle(newSocketFd,dirHead,train.dataLen);
                }
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,2),(strcmp(buf,"ls")==0)){
                    send(newSocketFd,&train,4+train.dataLen,0);
                    //接收指定目录的文件信息并显示
                    bzero(&train,sizeof(train));
                    recvCycle(newSocketFd,&train.dataLen,4);
                    recvCycle(newSocketFd,train.buf,train.dataLen);
                    printf("%20s %15s %8s %20s\n","filename","filesize","filetype","owner");
                    printf("%s\n",train.buf);
                }
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,5),(strcmp(buf,"mkdir")==0)){
                    send(newSocketFd,&train,4+train.dataLen,0);
                }
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,6),(strcmp(buf,"remove")==0)){
                    send(newSocketFd,&train,4+train.dataLen,0);
                }
                //实现长命令分离
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,4),(strcmp(buf,"gets")==0||strcmp(buf,"puts")==0)){
                    #ifdef DEBUG
                    printf("gets or puts cmd\n");
                    #endif
                    printf("file is transfering...\n");  
                }
            }
        }
    }
    return NULL;
}