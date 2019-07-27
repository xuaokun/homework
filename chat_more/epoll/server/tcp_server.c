#include <func.h>
#include <sys/epoll.h>
typedef struct user{
    int fd;
    struct sockaddr_in client;
    char usrName[20];
    char passwd[20];
}userMsg;
int tcpInit(int*,char*,char*);
int main(int arc,char*argv[]){
    ARGS_CHECK(arc,3);
    int sfd;
    tcpInit(&sfd,argv[1],argv[2]);
    int epfd=epoll_create(1);
    struct epoll_event event,evs[3];
    event.events=EPOLLIN;
    event.data.fd=STDIN_FILENO;
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    event.data.fd=sfd;
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    char buf[1024]={0};
    int readFdCount,i;
    int newfd;
    userMsg users[50];
    bzero(users,sizeof(users));
    int cnt=0;
    //if user comes
    struct sockaddr_in cli;
    bzero(&cli,sizeof(cli));
    socklen_t cliLen=sizeof(cli);
    char ip[16]={0},userName[20],password[20],usrFlag,passwdFlag;
    while(1){
            readFdCount=epoll_wait(epfd,evs,51,-1);
            for(i=0;i<readFdCount;i++){
                if(evs[i].events==EPOLLIN&&evs[i].data.fd==sfd){
                    bzero(&cli,sizeof(cli));
                    newfd=accept(sfd,(struct sockaddr *)&cli,&cliLen);
                    ERROR_CHECK(newfd,-1,"accept");
                    int j=0;
                    strcpy(ip,inet_ntoa(cli.sin_addr));
                    for(;j<cnt;j++){
                        if(strcmp(inet_ntoa(users[j].client.sin_addr),ip)==0){
                            usrFlag=1;//value 1:user exists
                            send(newfd,&usrFlag,1,0);
                            recv(newfd,password,sizeof(password),0);
                            if(strcmp(password,users[j].passwd)==0){
                              printf("%s is comming,IP:%s,port:%d\n",users[j].usrName,inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
                              users[j].fd=newfd;
                              passwdFlag=1;
                              send(newfd,&passwdFlag,1,0);
                              break;  
                            }
                            else{
                                passwdFlag=0;
                                send(newfd,&passwdFlag,1,0);
                                close(newfd);
                                break;
                            }
                        }
                    }
                    if(j==cnt){
                        usrFlag=0;// value 0:user exists
                        send(newfd,&usrFlag,1,0);
                        recv(newfd,userName,sizeof(userName),0);
                        strcpy(users[j].usrName,userName);
                        recv(newfd,password,sizeof(password),0);
                        strcpy(users[j].passwd,password);
                        users[j].fd=newfd;
                        users[j].client=cli;
                        cnt++;
                        printf("%s is comming,IP:%s,port:%d\n",users[j].usrName,inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
                    }    
                    event.events=EPOLLIN;
                    event.data.fd=newfd;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,newfd,&event);
                }
                if(evs[i].events==EPOLLIN&&evs[i].data.fd==STDIN_FILENO){
                    bzero(buf,sizeof(buf));
                    ret=read(STDIN_FILENO,buf,sizeof(buf));
                    if(ret==0){
                        printf("byebye\n");
                        goto end;
                    }
                    if(buf[0]=='\n'){
                        break;
                    }
                    for(int j=0;j<cnt;j++){
                        send(users[j].fd,"host",20,0);
                        send(users[j].fd,buf,strlen(buf)-1,0);
                    }
                }
                else if(evs[i].events==EPOLLIN){
                    int j;
                    int fd=evs[i].data.fd;
                    for(j=0;j<cnt;j++){
                        if(users[j].fd==fd){
                            bzero(buf,sizeof(buf));
                            ret=recv(fd,buf,sizeof(buf),0);
                            if(ret==0){
                                printf("%s is off-line\n",users[j].usrName);
                                event.data.fd=fd;
                                ret=epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&event);
                                ERROR_CHECK(ret,-1,"epoll_ctl");
                                close(fd);
                                break;
                            }
                            printf("%s:%s\n",users[j].usrName,buf);
                            strcpy(userName,users[j].usrName);
                            for(int j=0;j<cnt;j++){
                                if(users[j].fd!=fd){
                                    send(users[j].fd,userName,sizeof(userName),0);
                                   send(users[j].fd,buf,strlen(buf),0); 
                                }
                            }
                        }
                    }
                }
            }
        }
end:
    close(newfd);
    close(sfd);
    return 0;
}

