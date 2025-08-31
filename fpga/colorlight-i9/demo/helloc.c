#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <libfatfs/ff.h>
#include <libfatfs/diskio.h>

#include <liblitesdcard/spisdcard.h>

void open_test(void)
{
    static FATFS fs;
    static FIL file;
    
    printf("Open test start\n");
    
    if (spisdcard_init() != 1) {
        printf("SD init failed\n");
        return;
    }
    printf("SD init OK\n");
    
    fatfs_set_ops_spisdcard();
    printf("FatFS ops set\n");
    
    // Monta sistema de arquivos
    FRESULT res = f_mount(&fs, "", 0);
    if (res != FR_OK) {
        printf("Mount failed: %d\n", res);
        return;
    }
    printf("Mount OK\n");
    
    // Tenta abrir arquivo
    res = f_open(&file, "test.txt", FA_WRITE | FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        printf("Open failed: %d\n", res);
        f_unmount("");
        return;
    }
    printf("Open OK\n");
    
    // Fecha arquivo
    f_close(&file);
    printf("Close OK\n");
    
    f_unmount("");
    printf("Unmount OK\n");
    
    printf("Open test completed\n");
}

void helloc(void)
{
    printf("SD Card Open Test\n");
    printf("=================\n");
    
    open_test();
    
    printf("Test completed successfully!\n");
}