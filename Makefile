CC=g++

CFLAGS = -Wall -Werror -ansi 

all: bin rshell ls cp

bin:
	[ ! -d $bin ] && mkdir bin

rshell:
	$(CC) $(CFLAGS) src/hw2.cpp -o ./bin/rshell

ls:
	$(CC) $(CFLAGS) src/ls.cpp -o ./bin/ls

cp:
	$(CC) $(CFLAGS) src/cp.cpp -o ./bin/cp



clean:
	rm -f *.0 rshell ls cp
	rm -rf *.0 bin
	echo Clean done
