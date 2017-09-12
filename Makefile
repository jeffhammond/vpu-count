CC = gcc
CFLAGS = -g -Wall -O3 -fopenmp

#CFLAGS += -DDEBUG

all: test.x time.x

test.x: test.o vpu-count.o
	$(CC) $(CFLAGS) $^ -o $@

time.x: time.o vpu-count.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o
	-rm -f *.x

