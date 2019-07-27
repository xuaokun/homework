#include <func.h>

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,3);
    int fdw=open(argv[1],O_WRONLY);
    int fdr=open(argv[2],O_RDONLY);
    printf("I am chat1\n");
    fd_set rdset;
    int ret;
    struct timeval time_t;
    char buf[1024]={0};
    while(1){
        bzero(&time_t,sizeof(time_t));
        time_t.tv_sec=3;
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(fdr,&rdset);
        ret=select(fdr+1,&rdset,NULL,NULL,&time_t);
        if(ret>0){
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

        }else{
            printf("暂无新的消息收到。\n");
        }
    }
    close(fdr);
    close(fdw);
    return 0;
}

