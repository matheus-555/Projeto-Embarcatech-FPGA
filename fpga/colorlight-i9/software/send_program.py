# send_program.py
import serial
import time
import sys

ser = serial.Serial('/dev/ttyUSB1', 115200, timeout=2)

# Ler o arquivo binário
with open('sdcard_test.bin', 'rb') as f:
    data = f.read()

# Escrever cada palavra de 32 bits
for i in range(0, len(data), 4):
    address = 0x40000000 + i
    word = data[i:i+4]
    if len(word) < 4:
        word = word + b'\x00' * (4 - len(word))
    
    hex_value = int.from_bytes(word, byteorder='little')
    
    # Enviar comando mem_write
    command = f"mem_write {address} 0x{hex_value:08X}\r\n"
    ser.write(command.encode())
    print(f"Writing to {address:08X}: 0x{hex_value:08X}")
    
    # Esperar resposta
    time.sleep(0.1)
    response = ser.read(ser.in_waiting or 1)
    print(response.decode(), end='')

# Executar o programa
ser.write(b'exec 0x40000000\r\n')
print("Executing program...")

ser.close()