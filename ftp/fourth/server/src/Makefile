SURS:=$(wildcard *.c)
BINS:=$(patsubst %.c,%.o,$(SURS))
ELF:=thread_pool_server
CC:=gcc
CFLAGS=-Wall  -I /usr/include/mysql -lmysqlclient -lcrypto
$(ELF):$(BINS)
	$(CC) $^ -o $@ -pthread -I /usr/include/mysql -lmysqlclient -lcrypto
clean:
	rm -rf $(BINS)$(ELF)
