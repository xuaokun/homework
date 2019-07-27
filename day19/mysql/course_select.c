#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char *argv[]){
    if(argc!=2){
        printf("ERROR args,please transfer you no\n");
        return -1;
    }
     MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char *user="root";
    char *password="123";
    char *database="student";
    char query[300]="select * from teacher";
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
        printf("Error making connecting with server:%s\n",mysql_error(conn));
        return -1;
    }
    else{
        printf("connecting...\n");
    }
    int t=mysql_query(conn,query);
    if(t){
        printf("Error making select:%s\n",mysql_error(conn));
        return -1;
    }
    res=mysql_use_result(conn);
    int i;
    if(res){
        printf("已开课情况：\n");
        while((row=mysql_fetch_row(res))!=NULL){
            for(i=0;i<mysql_num_fields(res);i++){
                printf("%8s ",row[i]);
            }
            printf("\n");
        }
    }
    else{
        printf("Don't find data\n");
        return -1;
    }
    char cID[5];
    printf("请输入您要选的课程号：\n");
    scanf("%s",cID);
    strcpy(query,"insert into course values(");
    sprintf(query,"%s%s%s%s%s",query,cID,",",argv[1],")");
    t=mysql_query(conn,query);
    if(t){
        printf("Error making insert:%s\n",mysql_error(conn));
        return -1;
    }
    else{
        printf("选课成功\n");
    }
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}