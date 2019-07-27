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
    char *pMap=(char*)mmap(NULL,buf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(pMap,(char*)-1,"mmap");
    int ret;
    ret=send(newFd,pMap,buf.st_size,0);
    if(ret==-1){
        return -1;
    }
    send(newFd,&train,4,0);
    return 0;
}