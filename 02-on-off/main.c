#include <stdio.h>
#include "pico/stdlib.h"
#include "main.h"
#include "led/led.h"

// Информация об устройстве
#define DEVICE_NAME "Raspberry Pi Pico"
#define FW_VERSION "1.0.0"

int main() {
    // Инициализация ввода/вывода для printf
    stdio_init_all();
    
    // Инициализация светодиода
    led_init();
    
    printf("\n--- %s v%s ---\n", DEVICE_NAME, FW_VERSION);
    printf("Команды: 'e' - включить LED, 'd' - выключить LED, 'v' - версия\n");
    
    while(1) {
        // Ждем символ из терминала
        int symbol = getchar_timeout_us(1000); // Таймаут 1 секунда
        
        if(symbol != PICO_ERROR_TIMEOUT) {
            switch(symbol) {
                case 'e':
                    led_enable();
                    printf("LED включен\n");
                    break;
                    
                case 'd':
                    led_disable();
                    printf("LED выключен\n");
                    break;
                    
                case 'v':
                    printf("Устройство: %s\n", DEVICE_NAME);
                    printf("Версия: %s\n", FW_VERSION);
                    break;
                    
                default:
                    printf("Неизвестная команда: '%c' (код: %d)\n", symbol, symbol);
                    break;
            }
        }
        
        // Просто небольшая задержка, без tinyusb_task!
        sleep_ms(10);
    }
}
