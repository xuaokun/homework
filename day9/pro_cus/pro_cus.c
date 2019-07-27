#include <func.h>

int main()
{
    int semid=semget(1010,2,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    unsigned short arr[2]={10,0};//10为仓库位置数，0为产品的个数
    semctl(semid,0,SETALL,arr);
    struct sembuf sopp,sopv;
    if(!fork()){
        sopp.sem_num=1;
        sopp.sem_op=-1;
        sopp.sem_flg=SEM_UNDO;
        sopv.sem_num=0;
        sopv.sem_op=1;
        sopv.sem_flg=SEM_UNDO;
        printf("customer:\n");
        while(1){
            printf("space:%d,product num:%d\n",\
                   semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
            semop(semid,&sopp,1);
            printf("I am consuming...\n");
            semop(semid,&sopv,1);
            printf("space:%d,product num:%d\n",\
                   semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
            sleep(2);
        }
    }
    else{
            sopp.sem_num=0;
            sopp.sem_op=-1;
            sopp.sem_flg=SEM_UNDO;
            sopv.sem_num=1;
            sopv.sem_op=1;
            sopv.sem_flg=SEM_UNDO;
            printf("product:\n");
            while(1){
                printf("space:%d,product num:%d\n",\
                       semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
                semop(semid,&sopp,1);
                printf("I am producing...\n");
                semop(semid,&sopv,1);
                printf("space:%d,product num:%d\n",\
                       semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
                sleep(1);
            }
    }
}

