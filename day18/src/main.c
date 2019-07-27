#include "factory.h"  
int exitFds[2];//管道用于父子进程通信
void sigHandle(int signum){//10号信号触发的信号处理函数
    write(exitFds[1],&signum,1);
}

int main(int argc,char *argv[]){

    pipe(exitFds);
    while(fork()){//父进程用于防止线程池进程崩溃
        signal(SIGUSR1,sigHandle);//收到信号后通知子进程退出
        int status;
        pid_t pid;
        pid=wait(&status);//等待子进程退出，返回子进程ID
        if(WIFEXITED(status)){
            printf("thread_pool pid%d exited successfully with code:%d\n",pid,WEXITSTATUS(status));
            exit(0);
        }
    }
    if(argc!=5){
        printf("./pthread_pool_server IP PORT PTHREADNUM CAPACITY\n");
        return -1;
    }
    factory_t threadInfo;
    int pthreadNum=atoi(argv[3]);//线程的数量
    int pthreadCapacity=atoi(argv[4]);//队列容纳线程的容量
    factoryInit(&threadInfo,pthreadNum,pthreadCapacity);//初始化线程池信息
    factoryStart(&threadInfo);//开始创建多线程
    int socketFd;
    tcpInit(&socketFd,argv[1],argv[2]);//初始化并监听socket描述符
    int newFd;
    pQue_t pQue=&threadInfo.que;//设队列指针 
    int epfd=epoll_create(1);//创建epoll
    epollInAdd(epfd,socketFd);//监控socket
    epollInAdd(epfd,exitFds[0]);//监控管道读端
    struct epoll_event evs[pthreadNum+2];
    int readyCount,i,j;
    while(1){
        readyCount=epoll_wait(epfd,evs,pthreadNum+2,0);
        for(i=0;i<readyCount;i++){
            if(evs[i].data.fd==socketFd){
                newFd=accept(socketFd,NULL,NULL);//建立一个新连接生成描述符
                pNode_t pNew=(pNode_t)calloc(1,sizeof(Node_t));
                pNew->newfd=newFd;
                pthread_mutex_lock(&pQue->mutex);
                queInsert(pQue,pNew);//互斥进行队列中描述符的插入
                pthread_mutex_unlock(&pQue->mutex);
                pthread_cond_signal(&threadInfo.cond);//每次新增一个描述符就唤醒一个线程
            }
        }

    }
    return 0;
}