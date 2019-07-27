#define _GNU_SOURCE
#include <func.h>
int recvCycle(int,void*,int);
int main(int arc,char*argv[]){
    ARGS_CHECK(arc,3);
    int sfd;
    sfd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd,-1,"socket");
    struct sockaddr_in serAdd;
    bzero(&serAdd,sizeof(serAdd));
    serAdd.sin_family=AF_INET;
    serAdd.sin_port=htons(atoi(argv[2]));
    serAdd.sin_addr.s_addr=inet_addr(argv[1]);
    int ret=connect(sfd,(struct sockaddr*)&serAdd,sizeof(serAdd));
    ERROR_CHECK(ret,-1,"connect");
    char buf[1000]={0};
    int dataLen;
    //receive file name
    recvCycle(sfd,&dataLen,4);
    recvCycle(sfd,buf,dataLen);
    int fd;
    fd=open(buf,O_CREAT|O_RDWR,0666);
    ERROR_CHECK(fd,-1,"open");
//receive file size
    off_t fileSize,downloadSize=0;
    recvCycle(sfd,&dataLen,4);
    recvCycle(sfd,&fileSize,dataLen);
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
    }
    gettimeofday(&end,NULL);
    printf("use time:%ld\n",(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec);
    close(fd);
    close(sfd);
}
