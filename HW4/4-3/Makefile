TARGET=deadlock
OBJECT=deadlock.o

CC = mpicc
CFLAGS = -std=c99 -Wall

all: $(TARGET)
$(TARGET): $(OBJECT)

clean:
	rm -rf $(TARGET) $(OBJECT)

run:
	thorq --add --mode mpi --node 1 --slots 6 ./deadlock
