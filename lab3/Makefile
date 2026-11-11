CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11
TARGETS = pipe fifo

all: $(TARGETS)

pipe: pipe.c
	$(CC) $(CFLAGS) -o $@ $<

fifo: fifo.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS) /tmp/lab3_fifo

.PHONY: all clean