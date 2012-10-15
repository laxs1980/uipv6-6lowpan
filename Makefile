# Test Makefile for uip6

CC=gcc
LD=gcc

INCLUDES=-I"./lib/"\
	-I"./sys/"\
	-I"./net/"\
	-I"./cpu/native/net/"\
	-I"./"

CFLAGS=-std=c99\
	   -DUIP_CONF_IPV6\
	   -DUIP_CONF_IPV6_RPL\
	   -g

SRC=$(wildcard ./lib/*.c)
SRC+=$(wildcard ./net/*.c)
SRC+=$(wildcard ./dev/*.c)
SRC+=$(wildcard ./net/mac/*.c)
SRC+=$(wildcard ./net/rime/*.c)
SRC+=$(wildcard ./net/rpl/*.c)
SRC+=$(wildcard ./sys/*.c)
SRC+=$(wildcard ./platform/native/*.c)
SRC+=$(wildcard ./cpu/native/net/*.c)

ifdef APPNAME
	SRC+=./apps/$(APPNAME)/main.c
	INCLUDES+=-I"./apps/$(APPNAME)/"
endif

OBJ=$(SRC:.c=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

$(OBJ): %.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	-rm $(OBJ) main

dbg-show-src:
	echo $(SRC)

dbg-show-obj:
	echo $(OBJ)


