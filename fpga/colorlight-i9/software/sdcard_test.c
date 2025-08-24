// software/simple_test.c
void main() {
    // Teste apenas a UART primeiro
    volatile unsigned int *uart_status = (unsigned int*)0x90000000;
    volatile unsigned int *uart_data = (unsigned int*)0x90000004;
    
    char *message = "UART OK!\r\n";
    
    while (*message) {
        while (*uart_status & 0x08); // Wait for TX ready
        *uart_data = *message++;
    }
    
    while(1); // Loop infinito
}