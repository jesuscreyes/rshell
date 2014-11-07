CC=g++

CFLAGS = -Wall -Werror -ansi -pedantic

all: bin rshell ls

bin:
	[ ! -d $bin ] && mkdir bin

rshell:
	$(CC) $(CFLAGS) src/hw0.cpp -o ./bin/rshell

ls:
	$(CC) $(CFLAGS) src/ls.cpp -o ./bin/ls

clean:
	
