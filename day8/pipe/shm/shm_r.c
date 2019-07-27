#include <func.h>

int main()
{
    int shmid=shmget(2000,1<<10,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    char *p=(char*)shmat(shmid,NULL,0);
    ERROR_CHECK(p,(char*)-1,"shmat");
    printf("%s\n",p);
    int ret;
    ret=shmctl(shmid,IPC_RMID,0);
    ERROR_CHECK(ret,-1,"shmctl");
    return 0;
}

