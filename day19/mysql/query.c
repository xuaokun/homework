#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char *argv[]){
    if(argc!=2){
        printf("error args\n");
        return -1;
    }
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char *user="root";
    char *password="123";
    char *database="student";
    char query[300]="select * from person where personID=";
    sprintf(query,"%s%s",query,argv[1]);
    puts(query);
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){//成功返回conn,失败返回NULL
        printf("Error connecting the database:%s\n",mysql_error(conn));
        return -1;
    }
    else{
        printf("connecting...\n");
    }
    int  t=mysql_query(conn,query);
    if(t){//成功返回0，失败返回非零
        printf("Error making query:%s\n",mysql_error(conn));
        return -1;
    }
    res=mysql_use_result(conn);
    if(res){//成功返回结构体，失败返回NULL
        while((row=mysql_fetch_row(res))!=NULL){
            for(t=0;t<mysql_num_fields(res);t++){
                printf("%8s ",row[t]);
            }
            printf("\n");
        }
    }
    else{
        printf("Don't find data\n");
    }
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}