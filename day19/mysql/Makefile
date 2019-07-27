con_mysql:query.c delete.c insert.c update.c course_select.c
	gcc course_select.c -o course_select_sql -I /usr/include/mysql -lmysqlclient
	gcc query.c -o query_sql -I /usr/include/mysql -lmysqlclient
	gcc delete.c -o delete_sql -I /usr/include/mysql -lmysqlclient
	gcc insert.c -o insert_sql -I /usr/include/mysql -lmysqlclient
	gcc update.c -o update_sql -I /usr/include/mysql -lmysqlclient
clean:
	rm query_sql delete_sql insert_sql update_sql