CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11

all: modules

modules: modules.c main.c
	$(CC) $(CFLAGS) -o $@ $^

test: modules.c test.c
	$(CC) $(CFLAGS) -o $@ $^
	./$@

clean:
	rm -f modules test

.PHONY: all test clean
