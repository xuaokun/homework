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
    off_t fileSize,downloadSize=0,lastLoadSize=0,slice;
    recvCycle(sfd,&dataLen,4);
    recvCycle(sfd,&fileSize,dataLen);
    slice=fileSize/1000;
//receve file
    while(1){
        recvCycle(sfd,&dataLen,4);
        if(dataLen>0){
            recvCycle(sfd,buf,dataLen);
            write(fd,buf,dataLen);
            downloadSize+=dataLen;
            if(downloadSize-lastLoadSize>=slice){
                printf("%5.2f%%\r",(float)downloadSize/fileSize*100);
                fflush(stdout);
                lastLoadSize=downloadSize;
            }
        }
        else{
            printf("100.00%%\n");
            break;
        }
    }
    close(fd);
    close(sfd);
}
