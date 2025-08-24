# software/Makefile
TARGET = sdcard_test
SRC = $(TARGET).c

# Compilador
CC = riscv64-unknown-elf-gcc
OBJCOPY = riscv64-unknown-elf-objcopy
CFLAGS = -march=rv32i -mabi=ilp32 -Os -nostdlib -ffreestanding
LDFLAGS = -T ../build/software/include/generated/output_format.ld
INCLUDES = -I ../build/software/include/generated -I ../build/software/include

all: $(TARGET).bin

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $< -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f *.elf *.bin

.PHONY: all clean