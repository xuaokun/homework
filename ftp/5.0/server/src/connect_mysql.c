#include "../include/factory.h"
#include <mysql/mysql.h>
MYSQL *conn;
int connectSql(){
    char *server="localhost";
    char *user="root";
    char *password="123";
    char *database="ftp";
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
        printf("Error making connecting with server:%s\n",mysql_error(conn));
        return -1;
    }
    else{
        printf("connecting...\n");
    }
    return 0;
}