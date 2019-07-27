#include <func.h>
int setnoblock(int);
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
    setnoblock(sfd);
    int total=0;
    char buf[1000];
    while(1){
       ret=send(sfd,buf,sizeof(buf),0);
       total+=ret;
        if(ret==-1){
            break;
        }
        printf("ret=%d,total=%d\n",ret,total);
    }
    close(sfd);
}
