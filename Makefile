CC = gcc
CFLAGS = -Wall -O3 -fopenmp

CFLAGS += -DDEBUG -g3

all: test.x time.x empirical.x

empirical.x: empirical.c
	icc -O0 -qopenmp -std=gnu99 -fasm-blocks $< -o $@

test.x: test.o vpu-count.o
	$(CC) $(CFLAGS) $^ -o $@

time.x: time.o vpu-count.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o
	-rm -f *.x

