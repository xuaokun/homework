#include "factory.h"
#include <mysql/mysql.h>
extern MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
// typedef struct handleArgs{
//     int fd;
//     char userName[20];
//     char cmd[20]; 
// }handleArgs_t,*pHandleArgs_t;
// void *recvFileHandle(void *p){
//     #ifdef DEBUG
//     printf("puts func\n");
//     #endif   
//     int fd=*(int*)p;
//     recvFile(fd);
//     #ifdef DEBUG
//     printf("receive success\n");
//     #endif
//     return NULL;
// }
// void *sendFileHandle(void *p){
//     pHandleArgs_t pInfo=(pHandleArgs_t)p;
//     #ifdef DEBUG
//     printf("gets function comes in\n");
//     #endif
//     char fileName[20]={0};
//     strcpy(fileName,pInfo->cmd+5);
//     #ifdef DEBUG
//     printf("fileName:%s\n",fileName);
//     #endif
//     transFile(pInfo->fd,fileName);
//     #ifdef DEBUG
//     printf("send success\n");
//     #endif
//     return NULL;
// }
int handleCmd(int fd,char *cmd,ppthread_pool_info_t pIn){
    train_t train;
    pQueue_t pQue=&pIn->que;
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
        return 0;
    }
    else if(bzero(buf,sizeof(buf)),memcpy(buf,cmd,4),strcmp(buf,"puts")==0){
        #ifdef DEBUG
        printf("puts cmd\n");
        #endif
         pFdNode_t pNew=(pFdNode_t)calloc(1,sizeof(pFdNode_t));
         pNew->newFd=fd;
         strcpy(pNew->cmd,cmd);
         pthread_mutex_lock(&pQue->mutex);
         queInsert(pQue,pNew);//互斥进行队列中描述符的插入
         pthread_mutex_unlock(&pQue->mutex);
         struct epoll_event event;
         event.events=EPOLLIN;
         event.data.fd=fd;
         epoll_ctl(pIn->epfd,EPOLL_CTL_DEL,fd,&event);
         pthread_cond_signal(&pIn->cond);//若为puts或者gets命令，唤醒一个线程
        //这时候主线程应该建立与新的子线程的连接，防止命令接收与puts文件的接收有冲突   
        return 0;
    }
    else if(bzero(buf,sizeof(buf)),memcpy(buf,cmd,4),strcmp(buf,"gets")==0){
        #ifdef DEBUG
        printf("gets cmd\n");
        #endif
         pFdNode_t pNew=(pFdNode_t)calloc(1,sizeof(pFdNode_t));
         pNew->newFd=fd;
         strcpy(pNew->cmd,cmd);
         pthread_mutex_lock(&pQue->mutex);
         queInsert(pQue,pNew);//互斥进行队列中描述符的插入
         pthread_mutex_unlock(&pQue->mutex);
         pthread_cond_signal(&pIn->cond);//若为puts或者gets命令，唤醒一个线程
        //这时候主线程应该建立与新的子线程的连接，防止命令接收与puts文件的接收有冲突 
        return 0;
    }
    return 0;
} 
int handleLogin(connectInfo_t *cIn){
    int fd=cIn->fd;
    char loginFlag;
    char cryp[100]={0};
    char salt[9]={0};
    char tocken[36]={0};
    char crypBuf[100]={0};
    train_t train;
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
        recvCycle(fd,cIn->userName,dataLen);
        recvCycle(fd,&dataLen,4);
        recvCycle(fd,cryp,dataLen);
        #ifdef DEBUG
        printf("name:%s,cryp:%s\n",cIn->userName,cryp);
        #endif
        char query[100]="insert into user_information values(";
        sprintf(query,"%s%s%s%s%s%s%s%s%s%s%s%s%s",query,"'",cIn->userName,"'",",","'",salt,"'",",","'",cryp,"'",")");
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
        generateLog(cIn->userName,"register");
        #ifdef DEBUG
        printf("Database is updated\n");
        #endif
    }
    else if(loginFlag=='1'){
        recvCycle(fd,&dataLen,4);
        recvCycle(fd,cIn->userName,dataLen);
        #ifdef DEBUG
        printf("userName:%s\n",cIn->userName);
        #endif       
        char query[100]="select * from user_information where userName=";
        sprintf(query,"%s%s%s%s",query,"'",cIn->userName,"'");
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
                mysql_free_result(res);
                generateLog(cIn->userName,"login");
                //生成tocken
                time_t now;
                now=time(NULL);
                sprintf(tocken,"%s%ld",cIn->userName,now);
                //strcpy(tocken,MD5(tocken,strlen(tocken),NULL));
                #ifdef DEBUG
                printf("tocken:%s\n",tocken);
                #endif
                strcpy(query,"update user_information set tocken=");
                sprintf(query,"%s%s%s%s%s%ld%s%s%s",query,"'",tocken,"',","expire_time=",now," where userName='",cIn->userName,"'");
                #ifdef DEBUG
                printf("query:%s\n",query);
                #endif
                
                int t=mysql_query(conn,query);
                if (t){
                    printf("Error making query:%s\n",mysql_error(conn));
                    return -1;
                }
                train.dataLen=strlen(tocken);
                memcpy(&train.buf,tocken,train.dataLen);
                send(fd,&train,4+train.dataLen,0);
                #ifdef DEBUG
                printf("check success\n");
                #endif
            }
            else{
                send(fd,"1",1,0);
                printf("check fail\n");
                mysql_free_result(res);
                return -1;
            }
        }    
    }
    else if(loginFlag=='3'){
        recvCycle(fd,&train.dataLen,4);
        recvCycle(fd,tocken,train.dataLen);
        char query[100]="select * from user_information where tocken=";
         sprintf(query,"%s%s%s%s",query,"'",tocken,"'");
        t=mysql_query(conn,query);
        if(t){
            printf("Error making query:%s\n",mysql_error(conn));
            return -1; 
        }
        res=mysql_use_result(conn);
        if(res){
            row=mysql_fetch_row(res);
            time_t now;
            now=time(NULL);
            if(NULL==row){
                send(fd,"3",1,0); 
                mysql_free_result(res);
                return -1;
            }
                            
            else if(now<(long)(row[4]+3600)){
                #ifdef DEBUG
                printf("name:%s\n",row[0]);
                #endif
                strcpy(cIn->userName,row[0]);
                mysql_free_result(res);
                strcpy(query,"update user_information set expire_time=");
                sprintf(query,"%s%s%ld%s%s%s%s",query,"'",now,"'", "where tocken='",tocken,"'");
                #ifdef DEBUG
                printf("%s\n",query);
                #endif
                int t=mysql_query(conn,query);
                if (t){
                    printf("Error making query:%s\n",mysql_error(conn));
                    return -1;
                }
                generateLog(cIn->userName,"tockenConnect");
                send(fd,"0",1,0); 
            }
        }
        else{
            send(fd,"3",1,0);
            mysql_free_result(res);
            return -1;
        }
    }
    return 0;
}