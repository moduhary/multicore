TARGET=mat_mul
TARGET2=mat_mul2
OBJECTS=timer.o mat_mul.o
OBJECTS2=timer.o mat_mul2.o

CC = mpicc
CFLAGS=-std=c99 -O3 -Wall

all: $(TARGET) $(TARGET2)

$(TARGET): $(OBJECTS)
$(TARGET2): $(OBJECTS2)

clean:
	rm -rf $(TARGET) $(OBJECTS)
	rm -rf $(TARGET2) $(OBJECTS2)

run: $(TARGET) $(TARGET2)
	thorq --add --mode mpi --nodes 1 --slots 8 --name mat ./$(TARGET)
	thorq --add --mode mpi --nodes 1 --slots 8 --name mat2 ./$(TARGET2)
