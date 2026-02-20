#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>
#include "hardware/gpio.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

const uint LED_PIN = 25;
const uint MAX_BUFFER_SIZE = 256;  // максимальный размер буфера

int main()
{
    stdio_init_all();
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, false);
       
    while (1)
    {
      char symbol = getchar();
      printf("received char: %c [ ASCII code: %d ]\n", symbol, symbol);
      switch(symbol)
{
case 'e':
	gpio_put(LED_PIN, true);
	printf("led enable done\n");
	break;

default:
	break;
}

switch(symbol)
{
case 'd':
	gpio_put(LED_PIN, false);
	printf("led disable done\n");
	break;

default:
	break;
}

switch(symbol)
{
case 'v':
	printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
	break;

default:
	break;
}
    }
}
