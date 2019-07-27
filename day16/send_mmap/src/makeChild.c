#include "process_pool.h"
int makeChild(process_data_t*p,int num){
    pid_t pid;
    int fds[2];
    int ret;
    for(int i=0;i<num;i++){
        ret=socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        ERROR_CHECK(ret,-1,"socketpair");
        pid=fork();
        if(0==pid){
            close(fds[0]);
            childHandle(fds[1]);
        }
        
        else{
            close(fds[1]);
            p[i].pid=pid;
            p[i].pipeFd=fds[0];
            p[i].busy=0;
        }
    }
    return 0;
}
int childHandle(int pipeFd){
    int newFd;
   char  finishFlag;
   while(1){
    recvFd(pipeFd,&newFd);//receve the task from the main process
    transFile(newFd); 
    close(newFd);
    write(pipeFd,&finishFlag,1);
    } 
    return 0;
}
  