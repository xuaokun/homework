#include <func.h>

int main()
{
    int fds[2];
    pipe(fds);
    if(!fork()){
        close(fds[0]);
        write(fds[1],"hello",5);
        close(fds[1]);
    }
    else{
        close(fds[1]);
        char buf[128]={0};
        wait(NULL);
        read(fds[0],buf,sizeof(buf));
        puts(buf);
        close(fds[0]);
        exit(0);
    }
    return 0;
}

