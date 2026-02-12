#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>
#include "hardware/gpio.h"

const uint LED_PIN = 25;
const uint MAX_BUFFER_SIZE = 256;  // максимальный размер буфера

int main()
{
    stdio_init_all();
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    char buffer[MAX_BUFFER_SIZE];
    
    printf("Система готова. Введите предложение и нажмите Enter:\n");
    printf("------------------------------------------------\n");
    
    while (1)
    {
        gpio_put(LED_PIN, 0);  // LED выключен - ожидание ввода
        
        // Очистка буфера
        memset(buffer, 0, MAX_BUFFER_SIZE);
        
        // Чтение строки с помощью gets или fgets
        // Используем gets для простоты (или fgets для безопасности)
        int i = 0;
        char c;
        
        // Чтение символов до Enter
        while ((c = getchar()) != '\n' && c != '\r' && i < MAX_BUFFER_SIZE - 1) {
            buffer[i++] = c;
            gpio_put(LED_PIN, 1);  // LED мигает при вводе
            sleep_ms(50);
            gpio_put(LED_PIN, 0);
        }
        buffer[i] = '\0';  // завершающий нуль
        
        gpio_put(LED_PIN, 1);  // LED горит - обработка
        
        printf("\n--- Вывод информации ---\n");
        
        // 1. Дублирование предложения
        printf("Введенное предложение: %s\n", buffer);
        printf("Дублирование: %s %s\n", buffer, buffer);
        
        // 2. Вывод ASCII кодов
        printf("\nASCII коды символов:\n");
        printf("Символ | Десятичный | Шестнадцатиричный\n");
        printf("-------|------------|------------------\n");
        
        int total_sum = 0;
        int length = strlen(buffer);
        
        for (int j = 0; j < length; j++) {
            printf("  '%c'  |    %3d     |      0x%02X\n", 
                   buffer[j], buffer[j], buffer[j]);
            total_sum += buffer[j];
        }
        
        // 3. Дополнительная информация
        printf("\nСтатистика:\n");
        printf("- Длина предложения: %d символов\n", length);
        printf("- Сумма ASCII кодов: %d\n", total_sum);
        printf("- Средний ASCII код: %.2f\n", (float)total_sum / length);
        printf("- Размер в памяти: %ld байт\n", sizeof(char) * (length + 1));
        
        // Декодирование чисел (если введены цифры)
        if (buffer[0] >= '0' && buffer[0] <= '9') {
            int number = atoi(buffer);
            printf("\nРаспознано число: %d\n", number);
            printf("- В двоичной: ");
            for (int bit = 31; bit >= 0; bit--) {
                printf("%d", (number >> bit) & 1);
                if (bit % 4 == 0) printf(" ");
            }
            printf("\n- В шестнадцатиричной: 0x%X\n", number);
        }
        
        printf("\nВведите следующее предложение (Enter для отправки):\n");
        printf("------------------------------------------------\n");
        
        gpio_put(LED_PIN, 0);
        sleep_ms(100);
    }
    
    return 0;
}
