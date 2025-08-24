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
    # Clock principal (25MHz onboard) - Pino correto para Colorlight i9
    ("clk25", 0, Pins("P3"), IOStandard("LVCMOS33")), # OSCILADOR ONBOARD   (OK)
    
    # LED - Pino correto
    ("user_led", 0, Pins("L2"), IOStandard("LVCMOS33")), # LED ONBOARD  (OK)

    # SPI para SDCard - Pinout correto para Colorlight i9
    ("spi", 0,
        Subsignal("clk",  Pins("N2")),  # SPI CLK   (OK)
        Subsignal("mosi", Pins("M1")),  # SPI MOSI  (OK)
        Subsignal("miso", Pins("T2")),  # SPI MISO  (OK)
        Subsignal("cs_n", Pins("T3")),  # SPI CS    (OK)
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

        # ⭐⭐ MÍNIMA MODIFICAÇÃO: Contador para fazer LED piscar ⭐⭐
        self.counter = Signal(24)  # Contador de 24 bits
        self.sync += self.counter.eq(self.counter + 1)  # Incrementa a cada clock
        self.comb += platform.request("user_led").eq(self.counter[23])  # LED pisca com bit mais significativo

# ------------------------------
# Build
# ------------------------------
def main():
    platform = ColorlightPlatform()
    soc = SDSoC(platform)
    builder = Builder(soc, output_dir="build", compile_software=False)
    builder.build()
    print("✅ Build completo! O LED em L2 deve piscar ~1.5 vezes por segundo.")

if __name__ == "__main__":
    main()