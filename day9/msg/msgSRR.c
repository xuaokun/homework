#include <func.h>
typedef struct msgbuf{
    long mtype;
    char mtext[50];
}MSG_T;
int main()
{
    int msgid=msgget(1234,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    struct msgbuf msg_info;
    bzero(&msg_info,sizeof(msg_info));
    msg_info.mtype=1;
    strcpy(msg_info.mtext,"hello");
   int ret= msgsnd(msgid,&msg_info,strlen(msg_info.mtext),0);
    ERROR_CHECK(msgid,-1,"msgsnd");
    bzero(&msg_info,sizeof(msg_info));
    ret=msgrcv(msgid,&msg_info,sizeof(msg_info.mtext),1,0);
    ERROR_CHECK(ret,-1,"msgrcv");
    printf("gets:%s\n",msg_info.mtext);
    ret=msgctl(msgid,IPC_RMID,NULL);
    return 0;
}

