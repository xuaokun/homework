#include <func.h>
#define N 10000000
int main()
{
    int semArrId=semget(1000,1,IPC_CREAT|0600);
    ERROR_CHECK(semArrId,-1,"semget");
    int shmid=shmget(1000,1<<20,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    int *p=(int*)shmat(shmid,NULL,0);
    ERROR_CHECK(p,(int*)-1,"shmat");
    int ret=semctl(semArrId,0,SETVAL,1);
    ERROR_CHECK(ret,-1,"semctl");
    struct sembuf sopp,sopv;
    sopp.sem_num=0;
    sopp.sem_op=-1;
    sopp.sem_flg=SEM_UNDO;
    sopv.sem_num=0;
    sopv.sem_op=1;
    sopv.sem_flg=SEM_UNDO;
    *p=0;
    if(!fork()){
        for(int i=0;i<N;i++){
            semop(semArrId,&sopp,1);
            p[0]=p[0]+1;
            semop(semArrId,&sopv,1);
        }   

    }   
    else{
        for(int i=0;i<N;i++){
            semop(semArrId,&sopp,1);
            p[0]=p[0]+1;
            semop(semArrId,&sopv,1);

        }   
        wait(NULL);
        printf("%d\n",p[0]);

    }   
    return 0;
}

