CC=gcc

all: sssp

sssp: sssp.o
	$(CC) -o sssp sssp.o

sssp.o: sssp.c
	$(CC) -o sssp.o -c sssp.c ../input/mmio.c

clean:
	rm -f sssp.o sssp
