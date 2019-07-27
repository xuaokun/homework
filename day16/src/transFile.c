#include "process_pool.h"

int transFile(int newFd){
    train_t train;
    struct stat buf;
    //send file name
    train.dataLen=strlen(FILENAME);
    strcpy(train.buf,FILENAME);
    send(newFd,&train,4+train.dataLen,0);
    //send file size
    int fd=open(FILENAME,O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    fstat(fd,&buf);
    train.dataLen=sizeof(buf.st_size);
    memcpy(train.buf,&buf.st_size,train.dataLen);
    send(newFd,&train,4+train.dataLen,0);
    //send file 
    int ret;
    while((train.dataLen=read(fd,train.buf,sizeof(train.buf)))){
        ret=send(newFd,&train,4+train.dataLen,0);
        ERROR_CHECK(ret,-1,"send");    
    }
    send(newFd,&train,4,0);
    return 0;
}