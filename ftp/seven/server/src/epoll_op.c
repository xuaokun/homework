#include "head.h"
int epollInAdd(int epfd,int fd){
    struct epoll_event event;
    event.events=EPOLLIN;
    event.data.fd=fd;
    printf("epfd:%d\n",epfd);
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);//监听fd的读
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return 0;
}