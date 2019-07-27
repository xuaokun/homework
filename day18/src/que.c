#include "work_que.h"
void queInit(pQue_t pQue,int capacity){
    bzero(pQue,sizeof(que_t));
    pQue->queCapacity=capacity;//队列的容量初始化
    pthread_mutex_init(&pQue->mutex,NULL);//锁的初始化
    return;
}
void queInsert(pQue_t pQue,pNode_t pNew){
    if(NULL==pQue->pHead){
        pQue->pHead=pQue->pTail=pNew;
    }
    else{
        pQue->pTail->pNext=pNew;
        pQue->pTail=pNew;
    }
    pQue->queSize++;
}
int queGet(pQue_t pq,pNode_t *pN){
    if(NULL==pq->pHead){
        return -1;
    }
    *pN=pq->pHead;
    pq->pHead=pq->pHead->pNext;
    if(NULL==pq->pHead){
        pq->pTail=NULL;
    }
    pq->queSize--;
    return 0;
}