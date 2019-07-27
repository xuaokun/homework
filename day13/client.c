#include <func.h>

int main(int arc,char *argv[]){
    ARGS_CHECK(arc,3);
    int socketfd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(socketfd,-1,"socketfd");
    struct sockaddr_in serAdd;
    memset(&serAdd,0,sizeof(serAdd));
    serAdd.sin_family=AF_INET;
    serAdd.sin_port=htons(atoi(argv[2]));
    serAdd.sin_addr.s_addr=inet_addr(argv[1]);
    int ret;
    ret=connect(socketfd,(struct sockaddr *)&serAdd,sizeof(serAdd));
    ERROR_CHECK(ret,-1,"connect");
    
    char buf[1024]={0};
    while(1){
        memset(buf,0,sizeof(buf));
        scanf("%s",buf);
        send(socketfd,buf,strlen(buf),0);
        memset(buf,0,sizeof(buf));
        recv(socketfd,buf,sizeof(buf),0);
        printf("client gets:%s\n",buf);
        close(socketfd);
    }
    
}