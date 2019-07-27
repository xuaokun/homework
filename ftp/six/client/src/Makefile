SURS:=$(wildcard *.c)
BINS:=$(patsubst %.c,%.o,$(SURS))
ELF:=client
CC:=gcc
CFLAGS=-g -Wall -lcrypt
$(ELF):$(BINS)
	$(CC) $^ -o $@ -lcrypt -pthread
clean:
	rm -rf $(BINS)$(ELF)
