#include <func.h>

int recvCycle(int sfd,void *buf,int len){
    char *p=(char *)buf;
    int total=0,ret;
    while(total<len){
        ret=recv(sfd,p+total,len-total,0);
        total+=ret;
    }
    return 0;
}