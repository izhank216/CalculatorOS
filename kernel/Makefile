CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
LD = ld
SRC = kernel.c
OBJ = ../build/kernel.o
ELF = ../build/kernel.elf
BIN = ../build/kernel.bin

all: $(BIN)

../build/kernel.o: $(SRC)
	mkdir -p ../build
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

$(ELF): $(OBJ)
	$(LD) -m elf_i386 -Ttext 0x1000 -nostdlib $(OBJ) -o $(ELF)

$(BIN): $(ELF)
	objcopy -O binary $(ELF) $(BIN)

clean:
	rm -rf ../build
