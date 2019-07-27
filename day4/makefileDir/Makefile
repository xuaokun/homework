SOUR:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SOUR))
ELF:=main.exe
CC:=gcc
CFLAGS:=-g -Wall
${ELF}:${OBJS}
	gcc $^ -o $@
.PHONY:rebuild clean
rebuild:clean ${ELF}
clean:
	rm -rf ${ELF} ${OBJS}
