
#ifndef _RECV_H_
#define _GNU_SOURCE
#include "head.h"
//#define DEBUG
typedef struct{
    int dataLen;
    char buf[1000];
}train_t;
int regAccount(int fd,char *);
int loginAccount(int fd,char *);
int recvFile(int sfd,int down);
int transFile(int newFd,char *fileName,char *MD);
int recvCycle(int,void*,int);
void *newHandle(void *p);
int computerFileMD5(char *filepath, char *md5_str);
int computerStringMD5(char *str, char *md5_str);
int checkFile(char *fileName);
#endif