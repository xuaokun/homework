#include <func.h>
int tcpInit(int*,char*,char*);
int main(int arc,char*argv[]){
    ARGS_CHECK(arc,3);
    int sfd;
    tcpInit(&sfd,argv[1],argv[2]);
    int newFd;
    char buf[1024];
    newFd=accept(sfd,NULL,NULL);
    printf("%d\n",newFd);
    read(0,buf,sizeof(buf));
    close(sfd);
}