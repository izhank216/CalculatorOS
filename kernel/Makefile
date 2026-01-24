CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
LD_FLAGS = -m elf_i386 -Ttext 0x1000 -nostdlib -oformat binary
SRC = kernel.c
BIN = ../build/kernel.bin

all: $(BIN)

../build/kernel.bin: $(SRC)
	mkdir -p ../build
	$(CC) $(CFLAGS) $(SRC) -Wl,-Ttext=0x1000,-oformat,binary -o $(BIN)

clean:
	rm -rf ../build
