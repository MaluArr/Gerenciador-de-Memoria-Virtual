CC = gcc
CFLAGS = -Wall -std=c11
TARGET = vm
SRCS = vm.c

all: clean $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

