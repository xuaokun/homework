#include "../include/work_queue.h"
//#define DEBUG
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
    if(pq->pHead == NULL)
        printf("NULL HEAAAD\n");
    *pN=pq->pHead;
    printf("queget\n");
#ifdef DEBUG
    printf("queget2\n");
    if(*pN == NULL)
        printf("NULL Node\n");
    printf("newFd:%d\n",(*pN)->newFd);
    printf("cmd:%s\n",(*pN)->cmd);
#endif
    printf("get task\n");
    pq->pHead=pq->pHead->pNext;

    printf("get task\n");
    if(NULL==pq->pHead){
        pq->pTail=NULL;
    }
    pq->queSize--;
    
    return 0;
}