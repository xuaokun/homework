#include "../include/factory.h"

int transFile(pFdNode_t pNode){
    train_t train;
    int downloadFile=pNode->downloadSize;
    struct stat buf;
    //send file name
    train.dataLen=strlen(pNode->cmd+5);
    strcpy(train.buf,pNode->cmd+5);
    send(pNode->newFd,&train,4+train.dataLen,0);
    //send file size
    #ifdef DEBUG
    printf("即将发送文件%s\n",pNode->cmd+5);
    #endif
    int fd=open(pNode->fileMD,O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    lseek(fd,downloadFile,SEEK_SET);
    fstat(fd,&buf);
    buf.st_size-=downloadFile;
    printf("size:%ld\n",buf.st_size);
    train.dataLen=(int)sizeof(buf.st_size);
    memcpy(train.buf,&buf.st_size,train.dataLen);
    send(pNode->newFd,&train,4+train.dataLen,0);
    //send file 
    int ret;
    ret=sendfile(pNode->newFd,fd,NULL,buf.st_size);
    ERROR_CHECK(ret,-1,"sendfiles");
    return 0;
}
