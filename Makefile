CC=g++

CFLAGS = -Wall -Werror -ansi -pedantic

all: bin rshell 

bin:
	[ ! -d $bin ] && mkdir bin

rshell:
	$(CC) $(CFLAGS) src/main.cpp -o ./bin/rshell

clean:
	
