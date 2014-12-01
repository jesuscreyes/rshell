CC=g++

CFLAGS = -Wall -Werror -ansi 

all: bin rshell

bin:
	[ ! -d $bin ] && mkdir bin

rshell:
	$(CC) $(CFLAGS) src/hw3.cpp -o ./bin/rshell

clean:
	rm -f *.0 rshell
	rm -rf *.0 bin
	echo Clean done
