#include <func.h>

int main()
{
    int semid=semget(1001,1,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    int ret=semctl(semid,0,SETVAL,1);
    ERROR_CHECK(ret,-1,"semctl");
    ret=semctl(semid,0,GETVAL);
    printf("the sem value is:%d\n",ret);
    ret=semctl(semid,0,IPC_RMID);
    ERROR_CHECK(ret,-1,"semctl");
    return 0;
}

