#include "factory.h"
#include <mysql/mysql.h>
extern MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
typedef struct handleArgs{
    int fd;
    char cmd[20]; 
}handleArgs_t,*pHandleArgs_t;
void *recvFileHandle(void *p){
    int fd=*(int*)p;
    recvFile(fd);
    #ifdef DEBUG
    printf("receive success\n");
    #endif
    return NULL;
}
void *sendFileHandle(void *p){
    pHandleArgs_t pInfo=(pHandleArgs_t)p;
    #ifdef DEBUG
    printf("gets function comes in\n");
    #endif
    char fileName[20]={0};
    strcpy(fileName,pInfo->cmd+5);
    #ifdef DEBUG
    printf("fileName:%s\n",fileName);
    #endif
    transFile(pInfo->fd,fileName);
    #ifdef DEBUG
    printf("send success\n");
    #endif
    return NULL;
}
void handleClient(int fd){
    struct epoll_event event;
    int epfd=epoll_create(1);
    event.events=EPOLLIN;
    event.data.fd=fd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    #ifdef DEBUG
    printf("线程开始处理客户端请求\n");
    #endif
    handleLogin(fd);
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
        pthread_t pthID;
        pthread_create(&pthID,NULL,recvFileHandle,(void*)&fd);
        return 0;
    }
    else if(bzero(buf,sizeof(buf)),memcpy(buf,cmd,4),strcmp(buf,"gets")==0){
        #ifdef DEBUG
        printf("gets cmd\n");
        #endif
        handleArgs_t handleInfo;
        bzero(&handleInfo,sizeof(handleInfo));
        strcpy(handleInfo.cmd,cmd);
        handleInfo.fd=fd;
        pthread_t pthID;
        #ifdef DEBUG
        printf("即将创建gets线程\n");
        #endif
        pthread_create(&pthID,NULL,sendFileHandle,(void*)&handleInfo);
        return 0;
    }
    return 0;
} 
int handleLogin(int fd){
    char loginFlag;
    char userName[20]={0};
    char cryp[100]={0};
    char salt[9]={0};
    int dataLen=0;
    int t;
    recvCycle(fd,&loginFlag,1);
    #ifdef DEBUG
    printf("recv flag:%c\n",loginFlag);
    #endif
    if(loginFlag=='0'){   
        GenerateSalt(salt);
        #ifdef DEBUG
        printf("salt:%s\n",salt);
        #endif
        send(fd,salt,sizeof(salt),0);
        recvCycle(fd,&dataLen,4);
        recvCycle(fd,userName,dataLen);
        recvCycle(fd,&dataLen,4);
        recvCycle(fd,cryp,dataLen);
        #ifdef DEBUG
        printf("name:%s,cryp:%s\n",userName,cryp);
        #endif
        char query[100]="insert into user_information values(";
        sprintf(query,"%s%s%s%s%s%s%s%s%s%s%s%s%s",query,"'",userName,"'",",","'",salt,"'",",","'",cryp,"'",")");
        #ifdef DEBUG
        printf("%s\n",query);
        #endif
        
        t=mysql_query(conn,query);
        if(t){
            printf("Error making query:%s\n",mysql_error(conn));
            return -1;
        }
        res=mysql_use_result(conn);
        mysql_free_result(res);
        #ifdef DEBUG
        printf("Database is updated\n");
        #endif
    }
    else if(loginFlag=='1'){
        recvCycle(fd,&dataLen,4);
        recvCycle(fd,userName,dataLen);
        #ifdef DEBUG
        printf("userName:%s\n",userName);
        #endif
        char crypBuf[100]={0};
        char query[100]="select * from user_information where userName=";
        sprintf(query,"%s%s%s%s",query,"'",userName,"'");
        t=mysql_query(conn,query);
        if(t){
            printf("Error making query:%s\n",mysql_error(conn));
            return -1; 
        }
        res=mysql_use_result(conn);
        if(res){
            row=mysql_fetch_row(res);
            if(NULL==row){
                send(fd,"1",1,0);
                mysql_free_result(res);
                return -1;
            }
            send(fd,"0",1,0);
            strcpy(salt,row[1]);
            send(fd,salt,sizeof(salt),0);
            recvCycle(fd,&dataLen,4);
            recvCycle(fd,cryp,dataLen);
            strcpy(crypBuf,row[2]);
            if(strcmp(cryp,crypBuf)==0){
                send(fd,"0",1,0);
                #ifdef DEBUG
                printf("check success\n");
                #endif
            }
            else{
                send(fd,"1",1,0);
            }
        }    
    }
    mysql_free_result(res);
    return 0;
}