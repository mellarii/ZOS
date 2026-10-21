CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11 -O2
LDFLAGS :=

TARGET := fork_demo

.PHONY: all clean

all: $(TARGET)

$(TARGET): fork_demo.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)