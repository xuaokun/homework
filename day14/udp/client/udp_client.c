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
    bzero(buf,sizeof(buf));
    sendto(sfd,"I hen niu",9,0,(struct sockaddr*)&serAdd,sizeof(serAdd));
    recvfrom(sfd,buf,sizeof(buf),0,NULL,NULL);
    printf("client gets %s\n",buf);
    close(sfd);
}