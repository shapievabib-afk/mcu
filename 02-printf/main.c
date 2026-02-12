#include "pico/stdlib.h"
#include <stdio.h>  // исправлено
#include "hardware/gpio.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"
const uint LED_PIN = 25;

int main()
{
    stdio_init_all();
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    while (1)
    {
        printf("Device name: '%s'\n", DEVICE_NAME);  // добавил ;
        printf("Hello world, it is %s\n", DEVICE_NAME);  // заменил cout на printf
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(1000);
    }
    
    return 0;  // добавил return
}
