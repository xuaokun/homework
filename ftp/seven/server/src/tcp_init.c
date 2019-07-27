#include "factory.h"
int tcpInit(int *sfd,char *ip,char *port){
    int socketFd=socket(AF_INET,SOCK_STREAM,0);//创建一个TCP通信的socket描述符
    ERROR_CHECK(socketFd,-1,"socket");
    struct sockaddr_in serAdd;//sockaddr_in结构体存储服务器端的IP和port信息
    bzero(&serAdd,sizeof(serAdd));//先对结构体清零
    serAdd.sin_family=AF_INET;//IPV4
    serAdd.sin_addr.s_addr=inet_addr(ip);//点分十进制的IP地址直接从字符型转为网络字节序
    serAdd.sin_port=htons(atoi(port));//端口号转为网络字节序    
    int ret;
    int reuse=1;
    ret=setsockopt(socketFd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));//将port端口号设置为可重用
    ERROR_CHECK(ret,-1,"setsockopt");
    ret=bind(socketFd,(struct sockaddr*)&serAdd,sizeof(serAdd));//将socket描述符与IP、端口号绑定
    ERROR_CHECK(ret,-1,"bind");
    listen(socketFd,10);//监听socket描述符，最大连接数为10
    *sfd=socketFd;//将得到的socket描述符写回
    return 0;
}

