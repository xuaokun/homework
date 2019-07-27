#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char* argv[]){
    MYSQL *conn;
    char *server="localhost";
    char *user="root";
    char *password="123";
    char *database="student";
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
        printf("Error connecting the database:%s\n",mysql_error(conn));
        return -1;
    }
    else{
        printf("connecting...\n");
    }
    char query[300]="insert into person(firstName,lastName,age) values('xu','aokun',22)";
    puts(query);
    int t=mysql_query(conn,query);
    if(t){
        printf("Error making query:%s\n",mysql_error(conn));
        return -1;
    }
    else{
        printf("insert success\n");
    }
    mysql_close(conn);
    return 0;
}