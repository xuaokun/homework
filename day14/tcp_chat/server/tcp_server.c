#include <func.h>
int tcpInit(int*,char*,char*);
int main(int arc,char*argv[]){
    ARGS_CHECK(arc,3);
    int sfd;
    tcpInit(&sfd,argv[1],argv[2]);
    fd_set rdset;
    fd_set needMonitorFd;
    char buf[1024];
    FD_ZERO(&needMonitorFd);
    FD_SET(STDIN_FILENO,&needMonitorFd);
    FD_SET(sfd,&needMonitorFd);
    int ret,newFd;
    while(1){
        memcpy(&rdset,&needMonitorFd,sizeof(fd_set));
        ret=select(14,&rdset,NULL,NULL,NULL);
        ERROR_CHECK(ret,-1,"select");
        if(FD_ISSET(sfd,&rdset)){
            newFd=accept(sfd,NULL,NULL);
            ERROR_CHECK(ret,-1,"accept");
            FD_SET(newFd,&needMonitorFd);          
        }
        if(FD_ISSET(STDIN_FILENO,&rdset)){
            bzero(buf,sizeof(buf));
            ret=read(STDIN_FILENO,buf,sizeof(buf));
            if(0==ret){
                printf("byebye\n");
                break;
            }
            send(newFd,buf,strlen(buf)-1,0);
        }
        if(FD_ISSET(newFd,&rdset)){
            bzero(buf,sizeof(buf));
            ret=recv(newFd,buf,sizeof(buf),0);
            if(0==ret){
                printf("byebye\n");
                FD_CLR(newFd,&needMonitorFd);
                close(newFd);
            }
            else printf("%s\n",buf);
        }
    }
    close(sfd);
}