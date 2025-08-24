# entra no ambiente litex
source ~/litex/litex-venv/bin/activate

# gera o build
python3 soc.py

# carega bitstream na fpga
openFPGALoader -b colorlight-i9 build/gateware/soc.bit