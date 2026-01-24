CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -c
LD = ld
SRC = kernel.c
OBJ = ../build/kernel.o
BIN = ../build/kernel.bin

all: $(BIN)

../build/kernel.o: $(SRC)
	mkdir -p ../build
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ)

../build/kernel.bin: $(OBJ)
	$(LD) -m elf_i386 -Ttext 0x1000 --oformat binary $(OBJ) -o $(BIN)

clean:
	rm -rf ../build
