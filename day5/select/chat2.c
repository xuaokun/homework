#include <func.h>

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,3);
    int fdw=open(argv[1],O_WRONLY);
    int fdr=open(argv[2],O_RDONLY);
    printf("I am chat2,fdr=%d,fdw=%d\n",fdr,fdw);
    char buf[128]={0};
    fd_set rdset;
    int ret;
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(fdr,&rdset);
        ret=select(fdr+1,&rdset,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&rdset)){
            memset(buf,0,sizeof(buf));
            read(STDIN_FILENO,buf,sizeof(buf));
            write(fdw,buf,strlen(buf)-1);
        }
        if(FD_ISSET(fdr,&rdset)){
            memset(buf,0,sizeof(buf));
           int ret= read(fdr,buf,sizeof(buf));
	   if(ret==0){
		   printf("good bye.\n");
		   break;
	   }
            printf("%s\n",buf);
        }
    }
    return 0;
}

