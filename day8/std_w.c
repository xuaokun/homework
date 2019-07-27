#include <stdio.h>

int main()
{
    char buf[]={"aaa bbb ccc ddd eee fff ggg hhh"};
    FILE *fp=popen("wc -w","w");
    fwrite(buf,sizeof(buf),1,fp);
    pclose (fp);
    return 0;
}

