#include <func.h>
typedef struct msgbuf{
    long mtype;
    char mtext[64];
}MSG_T;
int main(int argc,char *argv[])
{
    int msgid=msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    struct msgbuf msg_info;
    msg_info.mtype=atoi(argv[1]);
    strcpy(msg_info.mtext,argv[2]);
    int ret=msgsnd(msgid,&msg_info,strlen(msg_info.mtext),0);
    ERROR_CHECK(ret,-1,"msgsnd");
    
    return 0;
}

