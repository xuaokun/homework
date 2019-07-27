SURS:=$(wildcard *.c)
BINS:=$(patsubst %.c,%.o,$(SURS))
ELF:=thread_pool_server
CC:=gcc
CFLAGS=-Wall
$(ELF):$(BINS)
	$(CC) $^ -o $@ -pthread
clean:
	rm -rf $(BINS)$(ELF)
