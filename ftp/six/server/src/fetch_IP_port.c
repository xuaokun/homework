#include "factory.h"
void fetchIPInfo(int confFd,char *IP,char *port){
    char buf[50]={0};
    int cnt=0;
    confFd=open("configure",O_RDONLY);
    read(confFd,buf,sizeof(buf));
#ifdef DEBUG
     printf("buf:%s\n",buf);
#endif
    int i=0;
    while(buf[i]!=':'){
            i++;
        }
    i++;
    while(buf[i]!='\r'&&buf[i]!='\n'){
        IP[cnt++]=buf[i];
        i++;
    }
    cnt=0;
    while(buf[i]!=':'){
        i++;
    }
    i++;
    strcpy(port,&buf[i]);
}
    