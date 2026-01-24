NASM = nasm
NASMFLAGS = -f bin
BOOT = bootloader.S
BIN = ../build/boot.bin

all: $(BIN)

../build/boot.bin: $(BOOT)
	mkdir -p ../build
	$(NASM) $(NASMFLAGS) $(BOOT) -o $(BIN)

clean:
	rm -rf ../build
