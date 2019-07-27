#include "client.h"
extern char tocken[36];
void *newHandle(void *p){
    int newSocketFd=*(int *)p;
    train_t train;
    char checkFlag;
    //发送tocken
    send(newSocketFd,"3",1,0);
    train.dataLen=strlen(tocken);
    memcpy(train.buf,tocken,train.dataLen);
    send(newSocketFd,&train,4+train.dataLen,0);
    recvCycle(newSocketFd,&checkFlag,1);
    if(checkFlag=='0'){
        printf("登录成功\n");
    }
    close(newSocketFd);
    return NULL;
}