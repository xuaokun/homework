#include <stdio.h>

int main()
{
    FILE *fp=popen("cat /etc/profile","r");
    char buf[512]={0};
    while(fgets(buf,sizeof(buf),fp)){
        puts(buf);
    }
    pclose(fp);
    return 0;
}

