#include <func.h>
int main(int arc,char*argv[]){
    ARGS_CHECK(arc,3);
    int sfd;
    sfd=socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(sfd,-1,"socket");
    struct sockaddr_in serAdd;
    bzero(&serAdd,sizeof(serAdd));
    serAdd.sin_family=AF_INET;
    serAdd.sin_port=htons(atoi(argv[2]));
    serAdd.sin_addr.s_addr=inet_addr(argv[1]);
    char buf[1024];
    fd_set rdset;
    int ret;
    bzero(buf,sizeof(buf));
    sendto(sfd,"ok",2,0,(struct sockaddr*)&serAdd,sizeof(serAdd));
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(sfd,&rdset);
        ret=select(sfd+1,&rdset,NULL,NULL,NULL);
        ERROR_CHECK(ret,-1,"select");
        if(FD_ISSET(STDIN_FILENO,&rdset)){
            bzero(buf,sizeof(buf));
            read(STDIN_FILENO,buf,sizeof(buf));
            sendto(sfd,buf,strlen(buf)-1,0,(struct sockaddr*)&serAdd,sizeof(serAdd));
        }
        if(FD_ISSET(sfd,&rdset)){
            bzero(buf,sizeof(buf));
            recvfrom(sfd,buf,sizeof(buf),0,NULL,NULL);
            printf("%s\n",buf);
        }
    }
    close(sfd);
    return 0;
}