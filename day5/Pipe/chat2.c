#include <func.h>

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,3);
    int fdr=open(argv[1],O_RDONLY);
    int fdw=open(argv[2],O_WRONLY);
    printf("I am chat2\n");
    fd_set rdset;
    int ret;
    char buf[1024]={0};
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(fdr,&rdset);
        ret=select(fdr+1,&rdset,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&rdset)){
            memset(buf,0,sizeof(buf));
            ret=read(STDIN_FILENO,buf,sizeof(buf));
            if(ret==0){
                printf("我下线了\n");
                break;
            }
            write(fdw,buf,strlen(buf)-1);//不将\n写入管道
        }   
        if(FD_ISSET(fdr,&rdset)){
            memset(buf,0,sizeof(buf));
         ret=read(fdr,buf,sizeof(buf));
            if(0==ret){
                printf("对方已经断开连接。\n");
                break;
            }   
            printf("%s\n",buf);

        }   

    }   
    close(fdw);
    close(fdr);
    return 0;

}
