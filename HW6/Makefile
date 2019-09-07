CC=nvcc
CFLAGS = -O3
TARGET=mat_mul

all: $(TARGET)

$(TARGET): mat_mul.cu timer.cpp
	$(CC) $(CFLAGS) mat_mul.cu timer.cpp -o mat_mul

clean:
	rm -rf $(TARGET)

run: $(TARGET)
	thorq --add --mode single --name mat_mul --device gpu/1080 ./$(TARGET)
