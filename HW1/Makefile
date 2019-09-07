
all: p1 p2 p3

p1: float.c double.c
	gcc -o float.o float.c
	gcc -o double.o double.c

p2: flops.c
	gcc -O2 -o flops.o flops.c

p3: fma.c ma.c
	gcc -O2 -o fma.o -mavx -mfma fma.c
	gcc -O2 -o ma.o -mavx ma.c

clean:
	rm *.o
