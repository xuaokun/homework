#include <func.h>
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
    fd_set rdset;
    char buf[1024];
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(sfd,&rdset);
        ret=select(sfd+1,&rdset,NULL,NULL,NULL);
        ERROR_CHECK(ret,-1,"select");
        if(FD_ISSET(STDIN_FILENO,&rdset)){
            bzero(buf,sizeof(buf));
            ret=read(STDIN_FILENO,buf,sizeof(buf));          
            if(0==ret){
                printf("byebye\n");
                break;
            }
            send(sfd,buf,strlen(buf)-1,0);
            
        }
        if(FD_ISSET(sfd,&rdset)){
            bzero(buf,sizeof(buf));
            ret=recv(sfd,buf,sizeof(buf),0);
            if(0==ret){
                printf("byebye\n");
                break;
            }
            printf("%s\n",buf);
        }
    }
    close(sfd);
}
