#include "factory.h"
#include <mysql/mysql.h>
extern MYSQL *conn;

int generateLog(char *userName,char *cmd){
    char query[100]="insert into log (userName,reqAndCmd) values(";
    sprintf(query,"%s%s%s%s%s%s",query,"'",userName,"','",cmd,"')");
    int t=mysql_query(conn,query);
    if(t){
        printf("Error making query:%s\n",mysql_error(conn));
        return -1;
    }
    #ifdef DEBUG
    printf("%s\n",query);
    #endif
    return 0;
}