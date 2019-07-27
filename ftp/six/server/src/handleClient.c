#include "factory.h"
#include <mysql/mysql.h>
extern MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
int handleCmd(pConnectInfo_t cIn,char *cmd,ppthread_pool_info_t pIn){
    int fd=cIn->fd;
    train_t train;
    pQueue_t pQue=&pIn->que;
    char buf[10]={0};
    char fileMD[33]={0};
    int ret;
    if(strcmp(cmd,"ls")==0){
        #ifdef DEBUG
        printf("ls is received\n");
        #endif
        lsHandle(cIn->nowDir);//展示当前目录下的所有文件内容
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
        printf("cd cmd is received\n");
        #endif
        //cdHandle(cmd+3);//进入到对应目录
    }
    else if(bzero(buf,sizeof(buf)),memcpy(buf,cmd,5),strcmp(buf,"mkdir")==0){
        #ifdef DEBUG
        printf("coming\n");
        #endif
        char query[150]="insert into dir(precod,filename,owner,filesize,filetype) values(";
        sprintf(query,"%s%d%s%s%s%s%s%ld%s%s",query,cIn->nowDir,",'",cmd+6,"','",cIn->userName,"',",0,",'f','","')");
        #ifdef DEBUG
        printf("%s\n",query);
        #endif
        int t=mysql_query(conn,query);
        if(t){
            printf("Error making query about inserting into dir:%s\n",mysql_error(conn));
        }
        #ifdef DEBUG
        printf("insert into dir success\n");
        #endif
    }
    else if(bzero(buf,sizeof(buf)),memcpy(buf,cmd,6),strcmp(buf,"remove")==0){
        #ifdef DEBUG
        printf("remove cmd is received\n");
        #endif
        remove(cmd+7);
        return 0;
    }
    else if(bzero(buf,sizeof(buf)),memcpy(buf,cmd,4),strcmp(buf,"puts")==0){
        #ifdef DEBUG
        printf("puts cmd\n");
        #endif
        recvCycle(fd,fileMD,32);
        ret=checkFileExitByMd(fileMD);
        send(fd,&ret,sizeof(ret),0);
        if(ret==1){
            printf("服务器中已存在该文件,无需重新上传\n");    
            return 0;
        }
         pFdNode_t pNew=(pFdNode_t)calloc(1,sizeof(fdNode_t));
         pNew->newFd=fd;
         strcpy(pNew->userName,cIn->userName);
         strcpy(pNew->fileMD,fileMD);
         strcpy(pNew->cmd,cmd);
         pthread_mutex_lock(&pQue->mutex);
         queInsert(pQue,pNew);//互斥进行队列中描述符的插入
         pthread_mutex_unlock(&pQue->mutex);
         struct epoll_event event;
         event.events=EPOLLIN;
         event.data.fd=fd;
         #ifdef DEBUG
         printf("del fd:%d\n",fd);
         #endif
         epoll_ctl(pIn->epfd,EPOLL_CTL_DEL,fd,&event);//这时候主线程应该删除对连接描述符的监控，防止命令接收与puts文件的接收有冲突   
         pthread_cond_signal(&pIn->cond);//若为puts或者gets命令，唤醒一个线程
        return 0;
    }
    else if(bzero(buf,sizeof(buf)),memcpy(buf,cmd,4),strcmp(buf,"gets")==0){
        #ifdef DEBUG
        printf("gets cmd\n");
        #endif
        char md[33]={0};
        ret=checkFileExitByName(cmd+5,md);
        send(fd,&ret,sizeof(ret),0);
        if(ret==1){
            printf("服务器中不存在该文件,无法下载\n");
            return 0;
        }
         pFdNode_t pNew=(pFdNode_t)calloc(1,sizeof(fdNode_t));
         pNew->newFd=fd;
         strcpy(pNew->fileMD,md);
         strcpy(pNew->cmd,cmd);
         strcpy(pNew->userName,cIn->userName);
         pthread_mutex_lock(&pQue->mutex);
         queInsert(pQue,pNew);//互斥进行队列中描述符的插入
         pthread_mutex_unlock(&pQue->mutex);
         struct epoll_event event;
         event.events=EPOLLIN;
         event.data.fd=fd;
         epoll_ctl(pIn->epfd,EPOLL_CTL_DEL,fd,&event);//这时候主线程应该删除对连接描述符的监控，防止命令接收与puts文件的接收有冲突 
         pthread_cond_signal(&pIn->cond);//若为puts或者gets命令，唤醒一个线程
        return 0;
    }
    return 0;
} 
int handleLogin(pConnectInfo_t cIn){
    int fd=cIn->fd;
    char loginFlag;
    char cryp[100]={0};
    char salt[9]={0};
    char tocken[36]={0};
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
        char query[150]="insert into user_information values(";
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
        char query[150]="select * from user_information where userName=";
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
            strcpy(cryp,row[2]);
            if(strcmp(cryp,cryp)==0){
                send(fd,"0",1,0);
                mysql_free_result(res);
                generateLog(cIn->userName,"login");
                //生成tocken
                time_t now;
                now=time(NULL);
                sprintf(tocken,"%s%ld",cIn->userName,now);
                computerStringMD5(tocken,tocken);
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
        char query[150]="select * from user_information where tocken=";
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
int checkFileExitByMd(char *MD){
    char query[150]="select * from dir where md5code=";
    sprintf(query,"%s%s%s%s",query,"'",MD,"'");
    int t=mysql_query(conn,query);
    if(t){
        printf("Error making query about md5code:%s\n",mysql_error(conn));
        return 1;
    }
    res=mysql_use_result(conn);
    if(res){
        row=mysql_fetch_row(res);
        if(row){
            mysql_free_result(res);
            return 1;//返回1实现秒传功能
        }
    }
    mysql_free_result(res);
    return 0;
}
int checkFileExitByName(char *fileName,char *md){
    char query[150]="select * from dir where filename=";
    sprintf(query,"%s%s%s%s",query,"'",fileName,"'");
    int t=mysql_query(conn,query);
    if(t){
        printf("Error making query about md5code:%s\n",mysql_error(conn));
        return 1;
    }
    res=mysql_use_result(conn);
    if(res){
        row=mysql_fetch_row(res);
        if(!row){
            mysql_free_result(res);
            return 1;//返回1表示无此文件
        }
    }
    strcpy(md,row[6]);
    mysql_free_result(res);
    return 0;
}
int lsHandle(char *nowDir){
    return 0;
}