#include <func.h>
typedef struct msgbuf{
    long mtype;
    char mtext[64];
}MSG_T;
int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,2);
    int msgid=msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    struct msgbuf msg_info;
    bzero(&msg_info,sizeof(msg_info));
    int ret=msgrcv(msgid,&msg_info,sizeof(msg_info.mtext),atoi(argv[1]),IPC_NOWAIT);
    ERROR_CHECK(ret,-1,"msgsnd");
    printf("my mtype is:%ld,the message is:%s\n",msg_info.mtype,msg_info.mtext);
    return 0;
}

