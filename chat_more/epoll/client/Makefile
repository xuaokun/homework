SURS:=$(wildcard *.c)
BINS:=$(patsubst %.c,%.o,$(SURS))
ELF:=client
CC:=gcc
CFLAGS=-g -Wall
$(ELF):$(BINS)
	$(CC) $^ -o $@
clean:
	rm -rf $(BINS)$(ELF)
