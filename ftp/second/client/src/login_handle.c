#include "client.h"
int regAccount(int fd){
    train_t train;
    bzero(&train,sizeof(train));
    char userName[20]={0};
    char password[20]={0};
    char cryp[20]={0};
    char salt[9]={0};
    printf("请输入您的用户名：");
    fflush(stdout);
    scanf("%s",userName);
    printf("请输入您的密码：");
    fflush(stdout);
    scanf("%s",password);
    recvCycle(fd,salt,9);
    strcpy(cryp,crypt(password,salt));
    #ifdef DEBUG
    printf("cryp:%s\n",cryp);
    #endif 
    train.dataLen=strlen(userName);
    memcpy(train.buf,userName,train.dataLen);
    send(fd,&train,4+train.dataLen,0);
    train.dataLen=strlen(cryp);
    memcpy(train.buf,cryp,train.dataLen);
    send(fd,&train,4+train.dataLen,0);
    return 0;
}
int loginAccount(int fd){
    train_t train;
    bzero(&train,sizeof(train));
    char userName[20]={0};
    char password[20]={0};
    char cryp[20]={0};
    char salt[9]={0};
    char checkFlag;
    printf("请输入您的用户名：");
    fflush(stdout); 
    scanf("%s",userName);  
    printf("请输入您的密码：");
    fflush(stdout);  
    scanf("%s",password);
    train.dataLen=strlen(userName);
    memcpy(train.buf,userName,train.dataLen);
    send(fd,&train,4+train.dataLen,0);
    recvCycle(fd,&checkFlag,1);
    if(checkFlag=='1'){
        printf("userName is wrong\n");
        exit(1);
    }
    recvCycle(fd,salt,9);
    strcpy(cryp,crypt(password,salt));
    #ifdef DEBUG
    printf("cryp:%s\n",cryp);
    #endif 
    train.dataLen=strlen(cryp);
    memcpy(train.buf,cryp,train.dataLen);
    send(fd,&train,4+train.dataLen,0);
    recvCycle(fd,&checkFlag,1);
    if(checkFlag=='0'){
        printf("login success\n");
    }
    else{
        printf("password is wrong\n");
        exit(1);
    }
    return 0;
}