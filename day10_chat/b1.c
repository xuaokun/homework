#include <func.h>
typedef struct msgbuf {
    long mtype;//0 is me,1 is another one
    char mbuf[1024];
}msg_t;

int main (){
    msg_t m;
    bzero(&m,sizeof(m));
    int msgid=msgget(1002,IPC_CREAT|0666);
    ERROR_CHECK(msgid,-1,"msgget");
    while(1){
        memset(m.mbuf,0,sizeof(m.mbuf));
        msgrcv(msgid,&m,sizeof(m.mbuf),1,0);
         if(m.mbuf[0]=='2'){
             break;
         }
        if(m.mbuf[0]=='0'){
            printf("%74s\n",m.mbuf+1);
        }
        else{
            printf("%s\n",m.mbuf+1);
        }
        
    }
        
}