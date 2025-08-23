# O toolchain de ferramentas pode ser baixado no seguite link
# https://github.com/YosysHQ/oss-cad-suite-build

# Transforma arquivo verilog em json
yosys -p "read_verilog \
    blink.v \
    pwm.v \
    ; \
    synth_ecp5 -json out.json -abc9"

echo 'digite algo para continuar...'
read ''

# A partir do json faz o roteamento
nextpnr-ecp5 --json out.json --lpf blink.lpf --textcfg out.config --package CABGA381 --45k --speed 6

# Gerando bitsream
ecppack --input out.config --bit out.bit

# Carrega o bitstream na FPGA
openFPGALoader -b colorlight-i9 out.bit