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
    int ret=bind(socketfd,(struct sockaddr*)&serAdd,sizeof(struct sockaddr));
    ERROR_CHECK(ret,-1,"bind");
    listen(socketfd,10);
    struct sockaddr_in cliAdd;
    bzero(&cliAdd,sizeof(cliAdd));
    int addrlen=sizeof(cliAdd);
    int new_fd=accept(socketfd,(struct sockaddr *)&cliAdd,&addrlen);
    ERROR_CHECK(new_fd,-1,"accept");
    printf("%s:%d is connected successful\n",inet_ntoa(cliAdd.sin_addr),ntohs(cliAdd.sin_port));
    char buf[1024]={0};
   while(1){
        memset(buf,0,sizeof(buf));
        recv(new_fd,buf,sizeof(buf),0);
        printf("server gets:%s\n",buf);
        memset(buf,0,sizeof(buf));
        scanf("%s",buf);
        send(new_fd,buf,strlen(buf),0);
    }
    close(new_fd);
    close(socketfd);
    return 0;
}