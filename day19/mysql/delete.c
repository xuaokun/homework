#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char* argv[]){
    if(argc!=2){
        printf("Eroor agrs\n");
        return -1;
    }
    MYSQL *conn;
    char *server="localhost";
    char *user="root";
    char *password="123";
    char *database="student";
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
        printf("Error connecting server:%s\n",mysql_error(conn));
        return -1;
    }
    else{
        printf("connecting...\n");
    }
    char query[300]="delete from person where firstName='";
    sprintf(query,"%s%s%s",query,argv[1],"'");
    puts(query);
    int t=mysql_query(conn,query);
    if(t){
        printf("Error making delete:%s\n",mysql_error(conn));
        return -1;
    }
    else{
        printf("delete success\n");
    }
    mysql_close(conn);
    return 0;
}