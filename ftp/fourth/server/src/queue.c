#include "work_queue.h"
void queInit(pQueue_t pQ,int maxNum){
    bzero(pQ,sizeof(queue_t));
    pQ->pHead=pQ->pTail=NULL;
    pQ->queueCapacity=maxNum;
    pthread_mutex_init(&pQ->mutex,NULL);
}
void queInsert(pQueue_t pQue,pFdNode_t pNew){
    if(NULL==pQue->pHead){
        pQue->pHead=pQue->pTail=pNew;
    }
    else{
        pQue->pTail->pNext=pNew;
        pQue->pTail=pNew;
    }
    pQue->queSize++;
}
int queGet(pQueue_t pq,pFdNode_t *pN){
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