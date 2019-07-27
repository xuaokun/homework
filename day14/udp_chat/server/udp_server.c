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
    int ret=bind(sfd,(struct sockaddr*)&serAdd,sizeof(serAdd));
    ERROR_CHECK(ret,-1,"bind");
    struct sockaddr_in client;
    bzero(&client,sizeof(client));
    socklen_t fromlen=sizeof(client);
    char buf[1024];
    bzero(buf,sizeof(buf));
    recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&client,&fromlen);
    printf("the connection is %s:client ip:%s port:%d\n",buf,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
    fd_set rdset;
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(sfd,&rdset);
        ret=select(sfd+1,&rdset,NULL,NULL,NULL);
        ERROR_CHECK(ret,-1,"select");
        if(FD_ISSET(STDIN_FILENO,&rdset)){
            bzero(buf,sizeof(buf));
            read(STDIN_FILENO,buf,sizeof(buf));
            sendto(sfd,buf,strlen(buf)-1,0,(struct sockaddr*)&client,sizeof(client));
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

