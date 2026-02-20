#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"

#define DEVICE_NAME "RP2040 Device"
#define DEVICE_VRSN "1.0.0"

// Callback для получения версии устройства
void version_callback(const char* args)
{
    printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

// Callback для включения LED
void led_on_callback(const char* args)
{
    led_task_state_set(LED_STATE_ON);
    printf("LED turned ON\n");
}

// Callback для выключения LED
void led_off_callback(const char* args)
{
    led_task_state_set(LED_STATE_OFF);
    printf("LED turned OFF\n");
}

// Callback для мигания LED
void led_blink_callback(const char* args)
{
    led_task_state_set(LED_STATE_BLINK);
    
    // Проверяем, передан ли параметр с периодом мигания
    if (args != NULL && args[0] != '\0') {
        int period_ms = atoi(args);
        if (period_ms > 0) {
            printf("LED blinking started with period %d ms\n", period_ms);
            
        } else {
            printf("LED blinking started with default period (500ms)\n");
        }
    } else {
        printf("LED blinking started with default period (500ms)\n");
    }
}


api_t device_api[] =
{
    {"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "turn LED on"},
    {"off", led_off_callback, "turn LED off"},
    {"blink", led_blink_callback, "make LED blink [period_ms]"},
    {NULL, NULL, NULL},
};

int main()
{
    
    stdio_init_all();
    stdio_task_init();
    protocol_task_init(device_api);
    led_task_init(device_api);
    
    printf("Device started. Type 'version' to get device info.\n");
    printf("Available commands:\n");
    printf("  version         - get device info\n");
    printf("  on              - turn LED on\n");
    printf("  off             - turn LED off\n");
    printf("  blink [period]  - make LED blink (period in ms, default 500ms)\n");
    
    while (1)
    {
     
        char* command_string = stdio_task_handle();
        
        if (command_string != NULL)
        {
            
            protocol_task_handle(command_string);
        }
        
    
        led_task_handle(NULL);
        
        sleep_ms(10);
    }
    
    return 0;
}
