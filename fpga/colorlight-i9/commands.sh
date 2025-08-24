# entra no ambiente litex

# --- Sintetizar o hardware
source ~/litex/litex-venv/bin/activate

# gera o build
python3 soc.py

# carega bitstream na fpga
openFPGALoader -b colorlight-i9 build/gateware/soc.bit



# --- Compilar o software 
# Compile
# Na pasta software/
riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -nostdlib -ffreestanding \
    -I ../build/software/include/generated -I ../build/software/include \
    -T ../build/software/include/generated/output_format.ld \
    sdcard_test.c -o sdcard_test.elf

riscv64-unknown-elf-objcopy -O binary sdcard_test.elf sdcard_test.bin

# Link (ajuste o caminho do linker.ld se necessário)
riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -T../include/generated/linker.ld -nostdlib -o firmware.elf main.o -lgcc

# Converta para binário
riscv64-unknown-elf-objcopy -O binary firmware.elf firmware.bin

#Converte inario para hex
hexdump -v -e '1/4 "%08X\n"' sdcard_test.bin > sdcard_test.hex