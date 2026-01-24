CC = gcc
CFLAGS = -m32 -O2 -Wall -Wextra -ffreestanding -c
SRC = kernel.c
OBJ = ../build/kernel.o

all: $(OBJ)

../build/kernel.o: $(SRC)
	mkdir -p ../build
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ)

clean:
	rm -rf ../build
