TARGET=mat_mul
OBJECTS=timer.o mat_mul.o

CC = gcc
CFLAGS=-std=c99 -fopenmp -O3 -Wall
LDFLAGS=-fopenmp

all: $(TARGET)

$(TARGET): $(OBJECTS)

clean:
	rm -rf $(TARGET) $(OBJECTS)

run: $(TARGET)
	thorq --add --mode single --name mat_mul ./$(TARGET) -2d >> mat_mul.stdout
	thorq --add --mode single --name mat_mul ./$(TARGET) -4d >> mat_mul.stdout
	thorq --add --mode single --name mat_mul ./$(TARGET) -8d >> mat_mul.stdout
	thorq --add --mode single --name mat_mul ./$(TARGET) -16d >> mat_mul.stdout
	thorq --add --mode single --name mat_mul ./$(TARGET) -32d >> mat_mul.stdout

