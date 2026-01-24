CC = gcc
CFLAGS = -m32 -O2 -Wall -Wextra
SRC = kernel.c
BIN = ../build/kernel.bin

all: $(BIN)

../build/kernel.bin: $(SRC)
	mkdir -p ../build
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

clean:
	rm -rf ../build
