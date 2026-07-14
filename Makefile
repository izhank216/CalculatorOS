CC = gcc
CFLAGS = -m32 -O2 -Wall -Wextra -ffreestanding -fno-pie -fno-stack-protector -c
LD = ld
SRC = kernel/kernel.c
OBJ = build/kernel.o
BIN = build/kernel.bin
ISO = calcos.iso
ISO_DIR = build/isofiles

all: $(ISO)

# Compile kernel
$(OBJ): $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ)

# Link kernel to flat binary
$(BIN): $(OBJ)
	$(LD) -m elf_i386 -Ttext 0x1000 --oformat binary $(OBJ) -o $(BIN)

# Prepare GRUB ISO
$(ISO): $(BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BIN) $(ISO_DIR)/boot/kernel.bin
	@echo 'set timeout=0' > $(ISO_DIR)/boot/grub/grub.cfg
	@echo 'menuentry "CalculatorOS" { multiboot /boot/kernel.bin }' >> $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

clean:
	rm -rf build $(ISO)
