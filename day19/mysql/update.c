#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char* argv[]){
    if(argc!=2){
        printf("Error args\n");
        return -1;
    }
    MYSQL *conn;
    char *server="localhost";
    char *user="root";
    char *password="123";
    char *database="student";
    conn=mysql_init(conn);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
        printf("Error connecting server:%s\n",mysql_error(conn));
        return -1;
    }
    else{
        printf("connecting...\n");
    }
    char query[300]="update  person set lastName='";
    sprintf(query,"%s%s%s",query,argv[1],"' where personID=2");
    puts(query);
    int t=mysql_query(conn,query);
    if(t){
        printf("Error making update:%s\n",mysql_error(conn));
        return -1;
    }
    else{
        printf("update success\n");
    }
    mysql_close(conn);
    return 0;
}