CC = gcc
CFLAGS = -g -Wall

CFLAGS += -DDEBUG

all: test.x

test.x: test.o vpu-count.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o
	-rm -f *.x

