#include "factory.h"

int transFile(int newFd,char *MD,char *fileName){
    train_t train;
    struct stat buf;
    //send file name
    train.dataLen=strlen(MD);
    strcpy(train.buf,MD);
    send(newFd,&train,4+train.dataLen,0);
    //send file size
    #ifdef DEBUG
    printf("即将发送文件%s\n",fileName);
    #endif
    int fd=open(MD,O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    fstat(fd,&buf);
    train.dataLen=sizeof(buf.st_size);
    memcpy(train.buf,&buf.st_size,train.dataLen);
    send(newFd,&train,4+train.dataLen,0);
    //send file 
    int ret;
    ret=sendfile(newFd,fd,NULL,buf.st_size);
    ERROR_CHECK(ret,-1,"sendfiles");
    return 0;
}
