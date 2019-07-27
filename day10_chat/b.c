#include <func.h>
typedef struct msgbuf {
    long mtype;
    char mbuf[1024];
}msg_t;  
msg_t m;
pid_t connect_id;
int msgid;
 void sigFunc(){
    kill(connect_id,SIGINT); 
    memset(m.mbuf,0,sizeof(m.mbuf));
    m.mbuf[0]='2';
    msgsnd(msgid,&m,strlen(m.mbuf),0);
    msgctl(msgid,IPC_RMID,NULL);
    printf("resource is released.\n");
    exit(1);
 }
int main()
{
//guandao
    int fdr=open("1.fifo",O_RDONLY);
    ERROR_CHECK(fdr,-1,"open");
    int fdw=open("2.fifo",O_WRONLY);
    ERROR_CHECK(fdw,-1,"open");
    pid_t id=getpid();
    write(fdw,&id,sizeof(id));
    read(fdr,&connect_id,sizeof(connect_id));
    printf("the connected id is:%d\n",connect_id);
    signal(SIGINT,sigFunc);
//msgqueue  

    bzero(&m,sizeof(m));
    m.mtype=1;
    msgid=msgget(1002,IPC_CREAT|0666);
    ERROR_CHECK(msgid,-1,"msgget");
    fd_set rdset;
    int ret;
    printf("Please send a message to A\n");
    while(1){
        FD_ZERO(&rdset);
        FD_SET(fdr,&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        ret=select(fdr+1,&rdset,NULL,NULL,NULL);
        ERROR_CHECK(ret,-1,"select");
        if(FD_ISSET(fdr,&rdset)){
            memset(m.mbuf,0,sizeof(m.mbuf));
            m.mbuf[0]='1';
             ret=read(fdr,m.mbuf+1,sizeof(m.mbuf));
             if(ret==0){
                 printf("the connection is bad.\n");
                 kill(connect_id,SIGINT);
             }
             msgsnd(msgid,&m,strlen(m.mbuf),0);
        }
        if(FD_ISSET(STDIN_FILENO,&rdset)){
            memset(m.mbuf,0,sizeof(m.mbuf));
            m.mbuf[0]='0';
            read(STDIN_FILENO,m.mbuf+1,sizeof(m.mbuf));
            write(fdw,m.mbuf+1,strlen(m.mbuf));
            msgsnd(msgid,&m,strlen(m.mbuf)-1,0);
        }
    } 
    close(fdr);
    close(fdw);                                         
}

