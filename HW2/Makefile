TARGET=mat_mul
OBJECTS=timer.o

CC=gcc
CFLAGS=-std=c99 -O3 -Wall -mavx -funroll-loops -pthread
LDFLAGS=-lm

all: $(TARGET)

$(TARGET): $(OBJECTS)

clean:
	rm -rf $(TARGET) $(OBJECTS)

run: $(TARGET)
	thorq --add --name $(TARGET) ./$(TARGET)
