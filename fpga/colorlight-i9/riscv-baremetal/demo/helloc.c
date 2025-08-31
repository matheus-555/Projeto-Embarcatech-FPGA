#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <generated/csr.h>
#include <libbase/i2c.h>

#define SENSOR_TEMP_PRESSAO_ADDR 0x3B


void i2c_scan_bus(void)
{
    printf("=== I2C Bus Scan ===\n");
    
    #ifdef CSR_I2C0_BASE
    printf("I2C controller found at 0x%08X\n", CSR_I2C0_BASE);
    
    int found_devices = 0;
    bool ret = false;
    
    // Varre todos os endereços I2C possíveis (0x08 a 0x77)
    for (uint8_t address = 0x08; address < 0x78; address++) {
        // CORREÇÃO: i2c_poll já espera o endereço com bit R/W
        // então use apenas o endereço de 7 bits!
        ret = i2c_poll(address); // Apenas o endereço de 7 bits
        
        if (ret) {
            printf("Device found at 0x%02X", address);
            found_devices++;
            
            // Identificação de dispositivos comuns
            if (address == 0x27) printf(" -> PCF8574 (I/O Expander)");
            else if (address == 0x3C) printf(" -> SSD1306 (OLED Display)");
            else if (address == 0x68) printf(" -> DS1307 (RTC)");
            else if (address == 0x50) printf(" -> AT24C32 (EEPROM)");
            else if (address == 0x48) printf(" -> PCF8591 (ADC)");
            else if (address == 0x76) printf(" -> BMP280/BME280 (Sensor)");
            else if (address == 0x77) printf(" -> BMP280/BME280 (Sensor)");
            printf("\n");
        }
    }
    
    printf("Found %d I2C device(s)\n", found_devices);
    
    #else
    printf("No I2C controller found!\n");
    #endif
    
    printf("=== Scan completed ===\n");
}


void helloc(void)
{
    printf("Colorlight I5 I2C Demo\n");
    printf("======================\n");
    
    printf("I2C Controller Test\n");
    printf("-------------------\n");

    i2c_reset();

    busy_wait(1000);
    
    // 1. Scan do barramento I2C
    i2c_scan_bus();
    
    // // 2. Teste de EEPROM (se houver dispositivo em 0x50)
    // i2c_eeprom_test();
    // busy_wait(2000000);
    
    // // 3. Teste de registro
    // i2c_register_test();
    // busy_wait(2000000);
    
    // // 4. Teste de velocidade
    // i2c_speed_test();
    
    // printf("======================\n");
    // printf("I2C Demo Completed!\n");
    // printf("Connect I2C devices to:\n");
    // printf("  SDA: PMOD pin\n"); 
    // printf("  SCL: PMOD pin\n");
}