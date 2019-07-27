#ifndef _WORK_QUEUE_H_
#define _WORK_QUEUE_H_
#include "head.h"

typedef struct fdNode{
    int newFd;
    char cmd[20];
    struct fdNode *pNext;
}fdNode_t,*pFdNode_t;
typedef struct queue{
    pFdNode_t pHead,pTail;
    int queueCapacity;
    int queSize;
    pthread_mutex_t mutex;
}queue_t,*pQueue_t;
void queInit(pQueue_t,int);
void queInsert(pQueue_t,pFdNode_t);
int queGet(pQueue_t,pFdNode_t*);
#endif