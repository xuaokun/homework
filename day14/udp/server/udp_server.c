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
    printf("gets %s client ip:%s port:%d\n",buf,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
    sendto(sfd,"I hen shuai",11,0,(struct sockaddr*)&client,sizeof(client));
    close(sfd);
}

