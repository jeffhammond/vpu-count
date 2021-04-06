CC = icc
CFLAGS = -Wall -g3 -O3 -fopenmp -std=c99

INTEL_STATIC = -static-intel -no-intel-extensions

CFLAGS += -DDEBUG
CFLAGS += -DSUPPORT_TIGERLAKE
CFLAGS += -DSUPPORT_ICELAKE
CFLAGS += -DSUPPORT_ICELAKE_SERVER
CFLAGS += -DSUPPORT_CANNONLAKE
CFLAGS += -DSUPPORT_XEON_PHI
CFLAGS += -DSUPPORT_PRE_AVX512

all: test.x time.x empirical.x

# Unfortunately, the assembly syntax used in this code
# does not compile with GCC or Clang.
empirical.x: empirical.c
	icc -O0 -std=gnu99 -fasm-blocks ${INTEL_STATIC} $< -o $@

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

