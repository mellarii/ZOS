CC = gcc

CFLAGS = -Wall

TARGETS = mycat mygrep myls mychmod

all: $(TARGETS)

mycat: mycat.c
	$(CC) $(CFLAGS) mycat.c -o mycat

mygrep: mygrep.c
	$(CC) $(CFLAGS) mygrep.c -o mygrep

myls: myls.c
	$(CC) $(CFLAGS) myls.c -o myls

mychmod: mychmod.c
	$(CC) $(CFLAGS) mychmod.c -o mychmod

	rm -f $(TARGETS) *.exe
