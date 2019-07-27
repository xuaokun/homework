#ifndef _FACTORY_H_
#define _FACTORY_H_
#include "head.h"
#include "work_que.h"
typedef struct{
    que_t que;//连接请求的描述符队列，当线程空闲时从中取出
    int pthreadNum;//线程数目
    pthread_cond_t cond;//条件变量
    pthread_t *pthID;//存储线程ID的起始地址
    short startflag;//线程是否启动
}factory_t,*pFactory_t;
int  factoryInit(pFactory_t p,int pthreadNum,int pthreadCapacity);
int  factoryStart(pFactory_t);
int tcpInit(int*,char*,char*);
int transFile(int newFd);
int epollInAdd(int epfd,int fd);
#endif


