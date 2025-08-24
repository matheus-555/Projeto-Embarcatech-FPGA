#!/usr/bin/env python3
from migen import *
from litex.build.generic_platform import *
from litex.build.lattice import LatticePlatform
from litex.soc.integration.soc_core import SoCCore
from litex.soc.integration.builder import Builder
from litex.soc.cores.spi import SPIMaster
from litex.soc.cores.bitbang import I2CMaster

# ------------------------------
# Colorlight i9 platform - CORRIGIDO
# ------------------------------
_io = [
    ("clk25", 0, Pins("P3"), IOStandard("LVCMOS33")),
    ("user_led", 0, Pins("L2"), IOStandard("LVCMOS33")),
    ("spi", 0,
        Subsignal("clk",  Pins("N2")),
        Subsignal("mosi", Pins("M1")),
        Subsignal("miso", Pins("T2")),
        Subsignal("cs_n", Pins("T3")),
        IOStandard("LVCMOS33")
    ),
    ("i2c", 0,
        Subsignal("scl", Pins("J20")),
        Subsignal("sda", Pins("K20")),
        IOStandard("LVCMOS33")
    ),
    ("serial", 0,
        Subsignal("tx", Pins("J16")),
        Subsignal("rx", Pins("J18")),
        IOStandard("LVCMOS33")
    ),
]

class ColorlightPlatform(LatticePlatform):
    # CORREÇÃO CRÍTICA: Defina clock padrão
    default_clk_name = "clk25"
    default_clk_period = 40.0  # 25MHz = 40ns
    
    def __init__(self):
        LatticePlatform.__init__(self, "LFE5U-45F-6BG381C", _io, toolchain="trellis")

# ------------------------------
# SoC CORRIGIDO - Funcionará!
# ------------------------------
class MySoC(SoCCore):
    def __init__(self, platform, **kwargs):
        # CORREÇÃO: Deixe o LiteX criar o clock domain automaticamente
        # NÃO crie cd_sys manualmente!
        
        SoCCore.__init__(
            self,
            platform,
            clk_freq=25e6,  # Já define automaticamente
            cpu_type="picorv32",
            csr_data_width=32,
            with_uart=True,
            uart_baudrate=115200,
            with_timer=True,
            integrated_rom_size=0x10000,
            integrated_main_ram_size=0x4000,
            **kwargs,
        )

        # AGORA adicione os periféricos customizados
        # SPI para SD
        spi_pads = platform.request("spi")
        self.submodules.sd_spi = SPIMaster(
            pads=spi_pads,
            data_width=8,
            sys_clk_freq=25e6,
            spi_clk_freq=400e3,
        )
        self.add_csr("sd_spi")

        # I2C bit-bang
        i2c_pads = platform.request("i2c")
        self.submodules.i2c0 = I2CMaster(i2c_pads)
        self.add_csr("i2c0")

        # LED heartbeat - use o domínio sys do LiteX
        counter = Signal(24)
        self.sync += counter.eq(counter + 1)
        self.comb += platform.request("user_led").eq(counter[23])

def main():
    platform = ColorlightPlatform()
    soc = MySoC(platform)

    builder = Builder(soc, output_dir="build", compile_software=True)
    builder.build()

    print("Build completo! Agora a UART deve funcionar.")

if __name__ == "__main__":
    main()