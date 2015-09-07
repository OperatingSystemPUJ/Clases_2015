CC=gcc

%.o: %.c
	$(CC) -c -o $@ $<
all: fork_00 fork_01 execs sendmail forks vfork sqlite_00

forks: forks.o
	gcc -o forks forks.o
fork_00: fork_00.o
	gcc -o fork_00 fork_00.o
fork_01: fork_01.o
	gcc -o fork_01 fork_01.o
execs: execs.o
	gcc -o execs execs.o
sendmail: sendmail.o
	gcc -o sendmail sendmail.c
vfork: vfork.o
	g++ -o vfork vfork.cc
sqlite_00: sqlite_00.o
	gcc -o sqlite_00 sqlite_00.c -lsqlite3
tar:
	tar cf ../exercises.tar . 
	mv ../exercises.tar .
	
clean:
	rm -f *.o forks fork_00 fork_01 execs sendmail vfork sqlite_00
