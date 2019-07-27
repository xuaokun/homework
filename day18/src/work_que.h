#ifndef _WORK_QUE_H_
#define _WORK_QUE_H_
#include "head.h"
typedef struct tag_node{
    int newfd;
    struct tag_node *pNext;
}Node_t,*pNode_t;
typedef struct que{
    pNode_t pHead,pTail;//队列头和队列尾   
    int queCapacity;//队列容量
    int queSize;//队列当前长度
    pthread_mutex_t mutex;//队列锁
}que_t,*pQue_t;
void queInit(pQue_t,int);
void queInsert(pQue_t,pNode_t);
int queGet(pQue_t,pNode_t*);
#endif
