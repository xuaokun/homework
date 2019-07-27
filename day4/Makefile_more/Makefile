SURS:=$(wildcard *.c)
BINS:=$(SURS:%.c=%)
CC:=gcc
all:$(BINS)

%:%.c
	$(CC) $< -o $@
