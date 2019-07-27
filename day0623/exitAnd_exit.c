#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("it's exit\n");
    printf("This is the content in buffer.");
   // exit(0);
    _exit(0);
}

