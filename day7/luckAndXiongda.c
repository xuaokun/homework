#include <func.h>

int main()
{
    printf("pid:%d,euid:%d\n",getpid(),geteuid());
    while(1);
    return 0;
}

