SURS:=$(wildcard *.c)
BINS:=$(patsubst %.c,%.o,$(SURS))
ELF:=client
CC:=gcc
CFLAGS=-g -Wall -lcrypt -lcrypto
$(ELF):$(BINS)
	$(CC) $^ -o $@ -lcrypt -pthread -lcrypto
clean:
	rm -rf $(BINS)$(ELF)
