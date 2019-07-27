#ifndef _FACTORY_H_
#define _FACTORY_H_
#define _GNU_SOURCE
#include  "head.h"
#include "work_queue.h"
#define PTHREADNUM 5
#define PTHREADCAPACITY 10
#define DEBUG
typedef struct{
    int dataLen;
    char buf[1000];
}train_t;
typedef struct pthread_pool_info{
    int pthreadNum;
    int epfd;
    queue_t que;
    pthread_cond_t cond;
    pthread_t *pthID;
}pthread_pool_info_t,*ppthread_pool_info_t;
typedef struct connectInfo{
    int fd;
    char userName[20];
}connectInfo_t;
int  factoryInit(ppthread_pool_info_t p,int pthreadNum,int pthreadCapacity);
int  factoryStart(ppthread_pool_info_t);
int connectSql();
int tcpInit(int*,char*,char*);
int transFile(int newFd,char *fileName);
int epollInAdd(int epfd,int fd);
void fetchIPInfo(int confFd,char *IP,char *port);
void handleClient(int fd);
int recvCycle(int sfd,void *buf,int len);
int handleCmd(int fd,char *cmd,ppthread_pool_info_t);
int recvFile(int sfd);
int connectSql();
int GenerateSalt(char *str);
int handleLogin(connectInfo_t *cIn);
int sendOrRecv(int fd,char *cmd);
int generateLog(char *userName,char *cmd);
//#define DEBUG(funName,Info) {printf(funName);printf(" line: %d",__LINE__);printf(" %s\n",Info);}
#endif