CFLAGS  = -Wall -Wextra

all: fork_demo

fork_demo: fork_demo.o
	gcc fork_demo.o -o fork_demo

fork_demo.o: fork_demo.c
	gcc $(CFLAGS) -c fork_demo.c -o fork_demo.o

clean:
	rm -f fork_demo fork_demo.o

.PHONY: clean