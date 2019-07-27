#include "factory.h"
#include <mysql/mysql.h>
extern MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
int recvFile(pFdNode_t pNode){
    int sfd=pNode->newFd;
    char buf[1000]={0};
    int dataLen;
    //receive file name
    recvCycle(sfd,&dataLen,4);
    recvCycle(sfd,buf,dataLen);
    #ifdef DEBUG
    printf("fileName:%s\n",buf);
    #endif
    int fd,ret;
    fd=open(buf,O_CREAT|O_RDWR,0666);
    ERROR_CHECK(fd,-1,"open");
 //receive file size
    off_t fileSize,downloadSize=0,lastLoadSize=0,slice;
    recvCycle(sfd,&dataLen,4);
    recvCycle(sfd,&fileSize,dataLen);
    slice=fileSize/1000;
    struct timeval start,end;
    gettimeofday(&start,NULL);
//receve file
    int fds[2];
    pipe(fds);
    while(downloadSize<fileSize){
        ret=splice(sfd,NULL,fds[1],NULL,65536,SPLICE_F_MOVE|SPLICE_F_MORE);
        ERROR_CHECK(ret,-1,"splice");
        splice(fds[0],NULL,fd,NULL,ret,SPLICE_F_MORE|SPLICE_F_MOVE);
        downloadSize+=ret;
        if(downloadSize-lastLoadSize>=slice){
            printf("%5.2f%%\r",(float)downloadSize/fileSize*100);
            fflush(stdout);
            lastLoadSize=downloadSize;
        }
    }
    printf("100.00%%\n");
    gettimeofday(&end,NULL);
    printf("use time:%ld\n",(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec);
    close(fd);
    char query[150]="insert into dir(precod,filename,owner,filesize,filetype,md5code) values(";
    sprintf(query,"%s%d%s%s%s%s%s%ld%s%s%s",query,0,",'",pNode->cmd+5,"','",pNode->userName,"',",fileSize,",'f','",pNode->fileMD,"')");
    #ifdef DEBUG
    printf("%s\n",query);
    #endif
    int t=mysql_query(conn,query);
    if(t){
        printf("Error making query about inserting into dir:%s\n",mysql_error(conn));
    }
    #ifdef DEBUG
    printf("insert into dir success\n");
    #endif
    return 0;
}