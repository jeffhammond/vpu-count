CC = icc
CFLAGS = -Wall -g3 -O3 -fopenmp -std=c99

CFLAGS += -DDEBUG
CFLAGS += -DSUPPORT_XEON_PHI
#CFLAGS += -DSUPPORT_ICELAKE

all: test.x time.x empirical.x

# Unfortunately, the assembly syntax used in this code
# does not compile with GCC or Clang.
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
	-rm -rf *.dSYM

