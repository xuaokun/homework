#include <func.h>

int main(int arc,char *argv[])
{
    ARGS_CHECK(arc,2);
    struct hostent *p;
    p=gethostbyname(argv[1]);
   if(NULL==p){
   printf("getbyname fail.\n");
   return -1;
   }
   printf("正式主机名：%s\n",p->h_name);
   printf("主机别名\n");
   for(int i=0;p->h_aliases[i]!=NULL;i++){
   printf("%s\n",p->h_aliases[i]);
   }
    printf("%d\n",p->h_addrtype);
    printf("%d\n",p->h_length);
   char ip[16]={0};
   for(int i=0;p->h_addr_list[i]!=NULL;i++){
   bzero(ip,sizeof(ip));
   inet_ntop(p->h_addrtype,p->h_addr_list[i],ip,sizeof(ip));
   puts(ip);
   }
    return 0;
}

