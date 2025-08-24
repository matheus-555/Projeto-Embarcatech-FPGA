# O toolchain de ferramentas pode ser baixado no seguite link
# https://github.com/YosysHQ/oss-cad-suite-build

# Para programmer CMSIS-DAP ( comum em algumas placas) verifica o modulo específico da FPGA
# openFPGALoader -c cmsisdap --detect

FILE_CONSTRAINT=constraints.lpf
BOARD=colorlight-i9

# Transforma arquivo verilog em json
yosys -p "read_verilog \
    top.v \
    soc.v \
    spi_sd.v \
    VexRiscv.v \
    ; \
    synth_ecp5 -json out.json -abc9"

echo 'digite algo para continuar...'
read ''

# A partir do json faz o roteamento
nextpnr-ecp5 --json out.json --lpf $FILE_CONSTRAINT --textcfg out.config --package CABGA381 --45k --speed 6

# Gerando bitsream
ecppack --input out.config --bit out.bit

# Carrega o bitstream na FPGA
openFPGALoader -b $BOARD out.bit