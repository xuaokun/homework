#include "client.h"
extern char tocken[36];
extern struct sockaddr_in serAdd;
void cleanUp(void *p){
    int fd=*(int *)p;
    close(fd);
}
void *newHandle(void *p){
    int newSocketFd=*(int *)p;
    pthread_cleanup_push(cleanUp,(void*)&newSocketFd);
    train_t train;
    char checkFlag;
    //发送tocken
    send(newSocketFd,"3",1,0);
    train.dataLen=strlen(tocken);
    memcpy(train.buf,tocken,train.dataLen);
    send(newSocketFd,&train,4+train.dataLen,0);
    recvCycle(newSocketFd,&checkFlag,1);
    if(checkFlag=='0'){
        printf("登录成功\n");
    }
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
                // send(newSocketFd,&train,4+train.dataLen,0);
                if(strcmp(train.buf,"pwd")==0){
                    send(newSocketFd,&train,4+train.dataLen,0);
                    bzero(&train,sizeof(train));
                    recvCycle(newSocketFd,&train.dataLen,4);
                    recvCycle(newSocketFd,train.buf,train.dataLen);
                    printf("%s\n",train.buf);
                }
                //实现长命令分离
                else if(bzero(buf,sizeof(buf)),memcpy(buf,train.buf,4),(strcmp(buf,"gets")==0||strcmp(buf,"puts")==0)){
                    #ifdef DEBUG
                    printf("gets or puts cmd\n");
                    #endif
                    // pthread_attr_t thread_attr;
                    // pthread_attr_init(&thread_attr);
                    // pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
                    // newNewFd=socket(AF_INET,SOCK_STREAM,0);
                    // CHILD_THREAD_ERROR_CHECK(newNewFd,"socket");
                    // int ret=connect(newNewFd,(struct sockaddr*)&serAdd,sizeof(serAdd));
                    // CHILD_THREAD_ERROR_CHECK(ret,"connect");
                    // pthread_create(&pthId,&thread_attr,newHandle,(void *)&newNewFd);
                    // if(strcmp(buf,"gets")==0){
                    //     recvFile(newSocketFd);
                    //     #ifdef DEBUG
                    //     printf("gets file\n");
                    //     #endif
                    //     break;
                    //     }
                    // else{
                    //     char fileName[20]={0};
                    //     strcpy(fileName,train.buf+5);
                    //     transFile(newSocketFd,fileName);
                    //     #ifdef DEBUG
                    //     printf("puts file success\n");
                    //     #endif
                    //     break;     
                    // }
                    printf("file is transfering...\n");  
                }
            }
        }
    }
    pthread_cleanup_pop(1);
    return NULL;
}