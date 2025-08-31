# --- Carregar variaveis de ambiente litex
source ~/litex/litex-venv/bin/activate


# --- puxa o diretorio onde os arquivos estão puxando as parametrizacoes
python3 -c "import litex_boards.platforms.colorlight_i5 as p; print(p.__file__)"


# --- Compilando a definição de Hardware da colorlight-i5 na colorlight-i9
$ python3 colorlight_i5.py --board i9 --revision 7.2 --build


# carega bitstream na fpga
$ openFPGALoader -b colorlight-i9 ${PWD}/build/colorlight_i5/gateware/colorlight_i5.bit


# --- Compilar o software 
$ python3 ${PWD}/software/demo/demo.py --build-path=${PWD}/build/colorlight_i5


# --- Carregar
$ litex_term /dev/ttyACM0 --kernel=${PWD}/demo.bin


#Converte inario para hex
hexdump -v -e '1/4 "%08X\n"' sdcard_test.bin > sdcard_test.hex