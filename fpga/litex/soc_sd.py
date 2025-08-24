#!/usr/bin/env python3
from migen import *
from litex.build.generic_platform import *
from litex.build.lattice import LatticePlatform
from litex.soc.integration.soc_core import SoCCore
from litex.soc.integration.builder import Builder
from litex.soc.cores.spi import SPIMaster

# ------------------------------
# Colorlight i9 platform - PINOS CORRETOS
# ------------------------------
_io = [
    # Clock principal (24MHz onboard) - Pino correto para Colorlight i9
    ("clk25", 0, Pins("H2"), IOStandard("LVCMOS33")),
    
    # LED - Pino correto
    ("user_led", 0, Pins("P3"), IOStandard("LVCMOS33")),

    # SPI para SDCard - Pinout correto para Colorlight i9
    ("spi", 0,
        Subsignal("clk", Pins("U1")),   # SPI CLK
        Subsignal("mosi", Pins("T1")),  # SPI MOSI
        Subsignal("miso", Pins("V1")),  # SPI MISO  
        Subsignal("cs_n", Pins("T2")),  # SPI CS
        IOStandard("LVCMOS33")
    ),

    # Botão de reset (opcional)
    ("cpu_reset", 0, Pins("R2"), IOStandard("LVCMOS33")),
]

class ColorlightPlatform(LatticePlatform):
    def __init__(self):
        # DISPOSITIVO CORRETO: LFE5U-45F (baseado na sua detecção)
        LatticePlatform.__init__(self, "LFE5U-45F-6BG381C", _io, toolchain="trellis")

# ------------------------------
# SoC com PicoRV32 + SPI
# ------------------------------
class SDSoC(SoCCore):
    def __init__(self, platform, **kwargs):
        # Configuração básica do SoC
        clk_freq = int(25e6)
        
        # Cria explicitamente o domínio de clock sys
        self.clock_domains.cd_sys = ClockDomain()
        self.comb += self.cd_sys.clk.eq(platform.request("clk25"))
        
        SoCCore.__init__(self, platform, clk_freq,
                         cpu_type="picorv32",
                         csr_data_width=32,
                         with_uart=False,
                         integrated_rom_size=0x8000,
                         integrated_main_ram_size=0x2000,
                         **kwargs)

        # Adiciona SPI SDCard
        self.submodules.sd_spi = SPIMaster(
            pads=platform.request("spi"),
            data_width=8,
            sys_clk_freq=clk_freq,
            spi_clk_freq=1e6
        )
        self.add_csr("sd_spi")

# ------------------------------
# Build
# ------------------------------
def main():
    platform = ColorlightPlatform()
    soc = SDSoC(platform)
    builder = Builder(soc, output_dir="build", compile_software=False)
    builder.build()

if __name__ == "__main__":
    main()