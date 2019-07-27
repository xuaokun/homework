#include <func.h>
typedef struct{
    int flag;
    char buf[1024];
    int who;//0 is me ,1 is another
}msgbuf_t;
msgbuf_t *pMap;
int shmid;
pid_t connect_id;
void sigFunc(){
    kill(connect_id,SIGINT);
    pMap->flag=2;
    shmdt(pMap);
    shmctl(shmid,IPC_RMID,NULL);
    unlink("1.fifo");
    unlink("2.fifo");
    printf("resource is released.\n");
    exit(1);
}
int main()
{//a和a1的共享内存设置
    shmid=shmget(1002,sizeof(msgbuf_t),IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    pMap=(msgbuf_t *)shmat(shmid,NULL,0);
    ERROR_CHECK(pMap,(msgbuf_t *)-1,"shmat");
    bzero(pMap,sizeof(msgbuf_t));
    //a和b的管道初始化
    int fdw=open("1.fifo",O_WRONLY);
    ERROR_CHECK(fdw,-1,"open");
    int fdr=open("2.fifo",O_RDONLY);
    ERROR_CHECK(fdr,-1,"open");
    pid_t id=getpid();
    write(fdw,&id,sizeof(id));
    read(fdr,&connect_id,sizeof(connect_id));
    printf("the connected id is:%d\n",connect_id);
    signal(SIGINT,sigFunc);
    fd_set rdset;
    int ret;
    printf("Please send a message to B\n");
    while(1){
        FD_ZERO(&rdset);
        FD_SET(fdr,&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        ret=select(fdr+1,&rdset,NULL,NULL,NULL);
        ERROR_CHECK(ret,-1,"select");
        if(pMap->flag==0){
            if(FD_ISSET(fdr,&rdset)){
                bzero(pMap->buf,sizeof(pMap->buf));
                ret=read(fdr,pMap->buf,sizeof(pMap->buf));
                if(ret==0){
                    printf("the connection isbad.\n");
                    kill(connect_id,SIGINT);
                }
                pMap->flag=1;
                pMap->who=1;
            }
            if(FD_ISSET(STDIN_FILENO,&rdset)){
                bzero(pMap->buf,sizeof(pMap->buf));
                read(STDIN_FILENO,pMap->buf,sizeof(pMap->buf));
                write(fdw,pMap->buf,strlen(pMap->buf)-1);
                pMap->flag=1;
                pMap->who=0;
            }
        }

    }
    close(fdr);
    close(fdw); 
}

