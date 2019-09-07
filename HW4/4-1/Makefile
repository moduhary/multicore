TARGET=host
OBJECTS=timer.o host.o 

CFLAGS=-std=c99 -O3 -Wall
LDFLAGS=-lm -lOpenCL

all: $(TARGET)

$(TARGET): $(OBJECTS)

clean:
	rm -rf $(TARGET) $(OBJECTS)

run: $(TARGET)
	thorq --add --mode single --name mat_mul_cl --device gpu/7970 ./$(TARGET)
