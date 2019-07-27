#include <func.h>

int main()
{
    int shmid;
    shmid =shmget(2000,1<<20,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmid");
    char *p=(char *)shmat(shmid,NULL,0);
    ERROR_CHECK(p,(char *)-1,"shmat");
    memset(p,0,1024);
    strcpy(p,"How are you");
    int ret;
    ret=shmdt(p);//shmdt将当前进程与共享内存分离
    ERROR_CHECK(ret,-1,"shmdt");
    return 0;
}

