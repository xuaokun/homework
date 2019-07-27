#include <func.h>

int main()
{
    pid_t ret=fork();
    if(0==ret){
        while(1);
    }
    else{
        exit(10);
    }
}

