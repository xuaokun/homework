q

#define N 10000000
int main()
{
    int shmid=shmget(1000,1<<20,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmid");
    int *p=(int *)shmat(shmid,NULL,0);
    ERROR_CHECK(p,(int*)-1,"shmat");
    *p=0;
    if(!fork()){
        for(int i=0;i<N;i++){                                                                                                
            p[0]=p[0]+1;
        }
    }
    else{
        for(int i=0;i<N;i++){
            p[0]+=1;
        }
        wait(NULL);
        printf("%d\n",p[0]);
    }
    return 0;
}

