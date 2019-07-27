#include <func.h>

int main()
{
    int fd1=open("log1.txt",O_WRONLY|O_CREAT);
    ERROR_CHECK(fd1,-1,"open");
    int fd2=open("log2.txt",O_WRONLY|O_CREAT);
    ERROR_CHECK(fd2,-1,"open");
   // printf("\n");
    close(1);
    int fdO=dup(fd1);
    close(fd1);
    close(2);
    int fdE=dup(fd2);
    close(fd2);
    printf("hello world\n");
    int fdTest=open("noExit.txt",O_RDONLY);
    ERROR_CHECK(fdTest,-1,"open");
    return 0;
}

