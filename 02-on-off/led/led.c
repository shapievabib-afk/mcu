#include "led.h"
#include "hardware/gpio.h"

// Какой пин используем для светодиода (на Pico это обычно 25)
#define LED_PIN 25

void led_init(void) {
    // Настраиваем пин как выход
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

void led_enable(void) {
    // Включаем светодиод (устанавливаем высокий уровень)
    gpio_put(LED_PIN, 1);
}

void led_disable(void) {
    // Выключаем светодиод (устанавливаем низкий уровень)
    gpio_put(LED_PIN, 0);
}
