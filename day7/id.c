#include <func.h>

int main()
{
   printf("当前进程ID:%d,父进程ID:%d,真实用户ID:%d,真实组ID=%d,有效用户ID:%d,有效组ID:%d\n",getpid(),getppid(),getuid(),getgid(),geteuid(),getegid());
    return 0;
}

