
#ifndef _RECV_H_
#define _GNU_SOURCE
#include "head.h"
#define DEBUG
typedef struct{
    int dataLen;
    char buf[1000];
}train_t;
int recvFile(int sfd);
int transFile(int newFd,char *fileName);
int recvCycle(int,void*,int);
#endif