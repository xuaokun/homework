#include <func.h>
int setnoblock(int);
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
    char buf[10]={0};
    int readFdCount,i;
    int newfd;
    while(1){
            readFdCount=epoll_wait(epfd,evs,3,-1);
            for(i=0;i<readFdCount;i++){
                if(evs[i].events==EPOLLIN&&evs[i].data.fd==sfd){
                    newfd=accept(sfd,NULL,NULL);
                    setnoblock(newfd);
                    event.events=EPOLLIN|EPOLLET;
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
                    send(newfd,buf,strlen(buf)-1,0);
                }
                if(evs[i].events==EPOLLIN&&evs[i].data.fd==newfd){
                    while(1){
                        bzero(buf,sizeof(buf));
                        ret=recv(newfd,buf,sizeof(buf)-1,0);
                        if(ret==0){
                            event.data.fd=newfd;
                            ret=epoll_ctl(epfd,EPOLL_CTL_DEL,newfd,&event);
                            ERROR_CHECK(ret,-1,"epoll_ctl");
                            close(newfd);
                            break;
                        }
                        else if(ret==-1){
                            break;
                        }
                        else{
                            printf("%s",buf);
                        }
                    }
                    printf("\n");
                }
            }
        }
end:
    close(newfd);
    close(sfd);
    return 0;
}

