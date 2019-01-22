CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra -pthread -Wpedantic

fib: fib_threads

clean:
	rm -f fib_threads *.o *~	
