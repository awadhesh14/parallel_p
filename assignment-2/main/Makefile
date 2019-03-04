CC=nvcc

all: tpd
	qsub samole-script

tpd: tpd.o
	$(CC) -o tpd tpd.o

tpd.o: tpd.cu
	$(CC) -o tpd.o -c tpd.cu

clean:
	rm -f tpd.o tpd
