#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main()
{
    FILE *read_fp=popen("ps -ax","r");
    char buf[1025];
    int char_read;
    memset(buf,0,sizeof(buf));
    if(NULL!=read_fp){
        char_read=fread(buf,sizeof(char),1024,read_fp);
        while(char_read>0){
            buf[char_read-1]=0;
            printf("reading:%s\n",buf);
            char_read=fread(buf,sizeof(char),1024,read_fp);
        }
        pclose(read_fp);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}

