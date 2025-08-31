#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <generated/csr.h>
#include <libbase/i2c.h>

#include "bmp280.h"



#define SENSOR_TEMP_HUMIDADE_AHT_10_ADDR 0x76UL








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



    
    // Verifica se BMP280 está presente
    if (!i2c_poll(BMP280_ADDR)) {
        printf("BMP280 not found at 0x%02X\n", BMP280_ADDR);
        printf("Try address 0x77 if SDO is HIGH\n");
        return;
    }
    
    printf("BMP280 found! Initializing...\n");
    bmp280_init();
    
    char temp_str[16];
    char press_str[16];
    

    for(;;)
    {
        int32_t temp = bmp280_read_temp();    // Centésimos de grau
        int32_t press = bmp280_read_pressure(); // Pa
        
        format_temp(temp, temp_str);
        format_pressure(press, press_str);
        
        printf("Temp: %s | Press: %s\n", temp_str, press_str);

        busy_wait(1500);
    }



}