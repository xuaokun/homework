#include <func.h>
typedef struct msgbuf{
    long mtype;
    char mtext[64];
}MSG_T;
int main(int argc,char *argv[])
{
    int msgid=msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    sleep(10);
    int ret=msgctl(msgid,IPC_RMID,NULL);
    return 0;
}

