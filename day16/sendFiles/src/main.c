#include "process_pool.h"

int main(int argc,char*argv[]){
    if(argc!=4){
        printf("./process_pool IP port processNum\n");
        return -1;
    }
    int processNum=atoi(argv[3]);
    process_data_t *pData=(process_data_t*)calloc(processNum,sizeof(process_data_t));
    makeChild(pData,processNum);
    #ifdef DEBUG
    for(int i=0;i<processNum;i++){
        printf("pid=%d,fd=%d\n",pData[i].pid,pData[i].pipeFd);
    }
    #endif
    int socketFd;
    tcpInit(&socketFd,argv[1],argv[2]);
    int epfd=epoll_create(1);
    struct epoll_event *evs;
    evs=(struct epoll_event*)calloc(processNum,sizeof(struct epoll_event));
    epollInAdd(epfd,socketFd);
    for(int i=0;i<processNum;i++){
        epollInAdd(epfd,pData[i].pipeFd);
    }
    int readCount,i,j,newFd;
    char finishflag;
    while(1){
        readCount=epoll_wait(epfd,evs,processNum+1,0);
        for(i=0;i<readCount;i++){
            if(evs[i].data.fd==socketFd){
                newFd=accept(socketFd,NULL,NULL);
                for(j=0;j<processNum;j++){
                    if(0==pData[j].busy){
                        sendFd(pData[j].pipeFd,newFd);
                        pData[j].busy=1;
                        printf("pid%d is busy\n",pData[j].pid);
                        break;
                    }
                }
                close(newFd);
            }
            for(j=0;j<processNum;j++){
                if(pData[j].pipeFd==evs[i].data.fd){
                    recv(pData[j].pipeFd,&finishflag,1,0);
                    pData[j].busy=0;
                    printf("pid%d is not busy\n",pData[j].pid);
                    break;
                }
            }
        }
    }
    return 0;
}