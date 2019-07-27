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
    int nowDir;//记录当前所在目录文件的code值
    struct connectInfo *pNext;
}connectInfo_t,*pConnectInfo_t;
typedef struct conInfoQueue{
    pConnectInfo_t pHead,pTail;
    int queSize;
}conInfoQueue_t,*pConInfoQueue_t;
int  factoryInit(ppthread_pool_info_t p,int pthreadNum,int pthreadCapacity);
int  factoryStart(ppthread_pool_info_t);
int connectSql();
int tcpInit(int*,char*,char*);
int connectInfoInsert(pConInfoQueue_t pConInfo,pConnectInfo_t);
int connectInfoDelete(pConInfoQueue_t pConInfo,pConnectInfo_t);
int transFile(int newFd,char *MD,char *fileName);
int epollInAdd(int epfd,int fd);
void fetchIPInfo(int confFd,char *IP,char *port);
void handleClient(int fd);
int recvCycle(int sfd,void *buf,int len);
int handleCmd(pConnectInfo_t cIn,char *cmd,ppthread_pool_info_t pIn);
int recvFile(pFdNode_t pNode);
int GenerateSalt(char *str);
int handleLogin(connectInfo_t *cIn);
int sendOrRecv(pFdNode_t);
int generateLog(char *userName,char *cmd);
int computerFileMD5(char *filepath, char *md5_str);
int computerStringMD5(char *str, char *md5_str);
int checkFileExitByMd(char *MD,int *fileSize);
int checkFileExitByName(char *fileName,char *md,int dir);
int lsHandle(pConnectInfo_t pCIn,char *cmd);
int cdHandle(pConnectInfo_t pCIn,char *cmd);
int rmHandle(pConnectInfo_t pCIn,char *cmd);
int getpath(pConnectInfo_t pCIn,char *path);
int rmdirHandle(pConnectInfo_t cIn,char *cmd);
//#define DEBUG(funName,Info) {printf(funName);printf(" line: %d",__LINE__);printf(" %s\n",Info);}
#endif