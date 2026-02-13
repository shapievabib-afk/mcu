import serial
import time
import sys
import glob
import threading

def find_pico_port():
    """Автоматически найти порт Pico"""
    ports = glob.glob('/dev/ttyACM*') + glob.glob('/dev/ttyUSB*')
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            return port
        except (OSError, serial.SerialException):
            pass
    return None

# Ищем порт
serialName = find_pico_port()
if not serialName:
    print("Pico не найден!")
    print("1. Проверьте подключение Pico")
    print("2. Зажмите BOOTSEL и переподключите")
    print("3. Скопируйте .uf2 файл на Pico")
    sys.exit(1)

print(f"Найден порт: {serialName}")
serialBaudRate = 115200
ser = serial.Serial(serialName, baudrate=serialBaudRate, timeout=1)

DEVICE_NAME = "Raspberry Pi Pico"
FW_VERSION = "1.0.0"

def led_enable():
    ser.write(b'e\n')
    print("> LED включен")

def led_disable():
    ser.write(b'd\n')
    print("> LED выключен")

def read_from_pico():
    """Функция для чтения данных от Pico"""
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if line:
                print(f"Pico: {line}")
        time.sleep(0.01)

print('Starting...')
time.sleep(2)  # Ждем инициализации

# Запускаем поток для чтения данных от Pico
read_thread = threading.Thread(target=read_from_pico, daemon=True)
read_thread.start()

print(f"\n--- {DEVICE_NAME} v{FW_VERSION} ---")
print("Команды:")
print("  'e' - включить LED")
print("  'd' - выключить LED")
print("  'v' - версия")
print("  'q' - выход")
print("--------------------------------")

try:
    while True:
        # Читаем команду с клавиатуры
        cmd = input("> ").strip().lower()
        
        if cmd == 'e':
            led_enable()
        elif cmd == 'd':
            led_disable()
        elif cmd == 'v':
            ser.write(b'v\n')
            print("> Запрос версии отправлен")
        elif cmd == 'q':
            print("Выход...")
            break
        else:
            print(f"Неизвестная команда: '{cmd}'")
            
except KeyboardInterrupt:
    print("\nПрограмма остановлена пользователем")
    
finally:
    ser.close()
    print("Порт закрыт")
