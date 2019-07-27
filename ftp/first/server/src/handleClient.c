#include "factory.h"
void handleClient(int fd){
    struct epoll_event event;
    int epfd=epoll_create(1);
    event.events=EPOLLIN;
    event.data.fd=fd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    #ifdef DEBUG
    printf("线程开始处理客户端请求\n");
    #endif
    char path[50]={0};
    chdir("../../cloudFile");
    getcwd(path,sizeof(path));
    #ifdef DEBUG
    printf("当前路径：%s\n",path);
    #endif
    train_t train;
    char cmd[20]={0};
    while(1){
        epoll_wait(epfd,&event,1,1000);
        if(event.data.fd==fd){
            bzero(&train,sizeof(train));
            recvCycle(fd,&train.dataLen,4);
            recvCycle(fd,train.buf,train.dataLen);
            bzero(cmd,sizeof(cmd));
            memcpy(cmd,train.buf,train.dataLen);
            #ifdef DEBUG
            printf("服务器开始处理命令:%s\n",cmd);
            #endif
            handleCmd(fd,cmd);
            
        }
    }
}
int handleCmd(int fd,char *cmd){
    train_t train;
    char buf[10]={0};
    if(strcmp(cmd,"ls")==0){
        #ifdef DEBUG
        printf("ls is received\n");
        #endif
    }
    else if(strcmp(cmd,"pwd")==0){
        char path[50]={0};
        getcwd(path,sizeof(path));
        bzero(&train,sizeof(train));
        train.dataLen=strlen(path);
        memcpy(train.buf,path,train.dataLen);
        send(fd,&train,4+train.dataLen,0);
        #ifdef DEBUG
        printf("send success:%s\n",train.buf);
        #endif
    }
    else if(cmd[0]=='c'&&cmd[1]=='d'){
        #ifdef DEBUG
        printf("cd cmd\n");
        #endif
    }
    else if(bzero(buf,sizeof(buf)),memcpy(buf,cmd,6),strcmp(buf,"remove")==0){
        #ifdef DEBUG
        printf("remove cmd\n");
        #endif
        remove(cmd+7);
    }
    else if(bzero(buf,sizeof(buf)),memcpy(buf,cmd,4),strcmp(buf,"puts")==0){
        #ifdef DEBUG
        printf("puts cmd\n");
        #endif
        recvFile(fd);
        #ifdef DEBUG
        printf("receive success\n");
        #endif
    }
    else if(bzero(buf,sizeof(buf)),memcpy(buf,cmd,4),strcmp(buf,"gets")==0){
        #ifdef DEBUG
        printf("gets cmd\n");
        #endif
        char fileName[20]={0};
        strcpy(fileName,cmd+5);
        transFile(fd,fileName);
        #ifdef DEBUG
        printf("send success\n");
        #endif
    }
    return 0;
} 