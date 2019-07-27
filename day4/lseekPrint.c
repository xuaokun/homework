#include <func.h>
struct student{
    char name[20];
    int no;
    float  score;
}S[3];
int main()
{
    FILE *fp=fopen("a.txt","rb+");
    for(int i=0;i<3;i++){
        scanf("%s%d%f",S[i].name,&S[i].no,&S[i].score);
        fprintf(fp,"%s %d %5.2f\n",S[i].name,S[i].no,S[i].score); 
    }
    int fd=fileno(fp);
    lseek(fd,0,SEEK_SET);
    char buf[1024]={0};
    char *tmp;
    printf("***********************\n");
    while(read(fd,buf,sizeof(buf))){
         printf("%s",buf);
    }
    fclose(fp);
    return 0;
}

