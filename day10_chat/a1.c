#include <func.h>
typedef struct {
    int flag;
    char buf[1024];                                                
    int who;//0 is me ,1 is another
}msgbuf_t;
int main()
{
    int shmid=shmget(1002,sizeof(msgbuf_t),IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    msgbuf_t *pMap;
    pMap=(msgbuf_t *)shmat(shmid,NULL,0);
    ERROR_CHECK(pMap,(msgbuf_t *)-1,"shmat");
    while(1){
        if(2==pMap->flag){
            shmdt(pMap);
            break;
        }
        else if(1==pMap->flag){
            if(pMap->who==0){
                printf("%74s",pMap->buf);
            }
            else{
                printf("%s",pMap->buf);
            }
            pMap->flag=0;
        }
    }

}


