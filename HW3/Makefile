TARGET=mat_mul
OBJECTS=timer.o

CFLAGS=-std=c99 -O2 -Wall
LDFLAGS=-lm

all: $(TARGET)

$(TARGET): $(OBJECTS)

clean:
	rm -rf $(TARGET) $(OBJECTS)

run: $(TARGET)
	thorq --add ./$(TARGET)
