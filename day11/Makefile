SURS:=$(wildcard *.c)
BINS:=$(SURS:%.c=%)
CC:=gcc
all:$(BINS)

%:%.c
	$(CC) $< -o $@ -lpthread
clean:
	rm -rf $(BINS)
