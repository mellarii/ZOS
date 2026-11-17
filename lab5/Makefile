CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDLIBS = -pthread -lrt

all: mtx

num1: num1.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

num2: num2.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

mtx: mtx.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f mtx