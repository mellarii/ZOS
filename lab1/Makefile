CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11 -O2
LDFLAGS :=

TARGETS := mycat mychmod mygrep myls

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS)