#include "client.h"
int checkFile(char *fileName){
    int fd;
    fd=open(fileName,O_RDONLY);
    if(-1==fd){
        return -1;
    }
    close(fd);
    return 0;
}