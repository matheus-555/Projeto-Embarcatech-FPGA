#ifndef __BMP280_H__
#define __BMP280_H__


/**
 *  PLACA SENSOR ROXA 
 * 
*/

#include <stdint.h>

// Sensor Bosch temperatura e pressao
#define BMP280_ADDR  0x76UL




// Estrutura de calibração
struct bmp280_calib {
    uint16_t dig_T1;
    int16_t dig_T2, dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
};


struct bmp280_calib calib;
int32_t t_fine;



// Lê byte do BMP280
uint8_t bmp280_read(uint8_t reg) {
    uint8_t value;
    i2c_read(BMP280_ADDR, reg, &value, 1, true, 1);
    return value;
}

// Lê múltiplos bytes
void bmp280_read_buf(uint8_t reg, uint8_t *data, uint8_t len) {
    i2c_read(BMP280_ADDR, reg, data, len, true, 1);
}

// Escreve byte no BMP280
void bmp280_write(uint8_t reg, uint8_t value) {
    i2c_write(BMP280_ADDR, reg, &value, 1, 1);
}

// Inicializa BMP280
void bmp280_init() {
    // Configura modo normal, oversampling x1
    bmp280_write(0xF4, 0x27);
    
    // Lê dados de calibração
    uint8_t cal[24];
    bmp280_read_buf(0x88, cal, 24);
    
    calib.dig_T1 = (cal[1] << 8) | cal[0];
    calib.dig_T2 = (cal[3] << 8) | cal[2];
    calib.dig_T3 = (cal[5] << 8) | cal[4];
    calib.dig_P1 = (cal[7] << 8) | cal[6];
    calib.dig_P2 = (cal[9] << 8) | cal[8];
    calib.dig_P3 = (cal[11] << 8) | cal[10];
    calib.dig_P4 = (cal[13] << 8) | cal[12];
    calib.dig_P5 = (cal[15] << 8) | cal[14];
    calib.dig_P6 = (cal[17] << 8) | cal[16];
    calib.dig_P7 = (cal[19] << 8) | cal[18];
    calib.dig_P8 = (cal[21] << 8) | cal[20];
    calib.dig_P9 = (cal[23] << 8) | cal[22];
}

// Lê temperatura (retorna em centésimos de grau C)
int32_t bmp280_read_temp() {
    uint8_t data[3];
    bmp280_read_buf(0xFA, data, 3);
    int32_t adc_T = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    
    int32_t var1 = ((((adc_T >> 3) - ((int32_t)calib.dig_T1 << 1))) * 
                   ((int32_t)calib.dig_T2)) >> 11;
    int32_t var2 = (((((adc_T >> 4) - ((int32_t)calib.dig_T1)) * 
                    ((adc_T >> 4) - ((int32_t)calib.dig_T1))) >> 12) * 
                    ((int32_t)calib.dig_T3)) >> 14;
    
    t_fine = var1 + var2;
    return (t_fine * 5 + 128) >> 8; // Retorna em centésimos de grau
}

// Lê pressão (retorna em Pa)
int32_t bmp280_read_pressure() {
    uint8_t data[3];
    bmp280_read_buf(0xF7, data, 3);
    int32_t adc_P = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    
    int64_t var1 = ((int64_t)t_fine) - 128000;
    int64_t var2 = var1 * var1 * (int64_t)calib.dig_P6;
    var2 = var2 + ((var1 * (int64_t)calib.dig_P5) << 17);
    var2 = var2 + (((int64_t)calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)calib.dig_P3) >> 8) + 
           ((var1 * (int64_t)calib.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)calib.dig_P1) >> 33;
    
    int64_t p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calib.dig_P8) * p) >> 19;
    
    p = ((p + var1 + var2) >> 8) + (((int64_t)calib.dig_P7) << 4);
    return (int32_t)p; // Retorna em Pa
}

// Formata temperatura para exibição
void format_temp(int32_t temp_celsiusx100, char *buffer) {
    int32_t whole = temp_celsiusx100 / 100;
    int32_t frac = temp_celsiusx100 % 100;
    if (frac < 0) frac = -frac; // Lida com negativos
    sprintf(buffer, "%d.%02d C", whole, frac);
}

// Formata pressão para exibição
void format_pressure(int32_t pressure_pa, char *buffer) {
    int32_t hpa = pressure_pa / 100;
    int32_t whole = hpa / 10;
    int32_t frac = hpa % 10;
    sprintf(buffer, "%d.%d hPa", whole, frac);
}






#endif