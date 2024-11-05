CC = gcc
CFLAGS = -g -Wall

all: test_malloc test_malloc_with_manager

test_malloc: tests.c
	$(CC) $(CFLAGS) -o test_malloc tests.c

test_malloc_with_manager: malloc.c tests.c
	$(CC) $(CFLAGS) -o test_malloc_with_manager malloc.c tests.c

clean:
	rm -f test_malloc test_malloc_with_manager