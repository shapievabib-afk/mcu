#include "led-task.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const uint LED_PIN = 25;
uint LED_BLINK_PERIOD_US = 500000;

uint64_t led_ts;
led_state_t led_state;

void led_task_init(api_t* device_api) 
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);
    led_ts = time_us_64();
    led_state = LED_STATE_OFF;

    (void)device_api; 
}

void led_task_handle(char* command_string)  
{
    uint64_t current_time = time_us_64();
    
    if (command_string != NULL) {
        
    }

    switch (led_state)
    {
        case LED_STATE_OFF:
            gpio_put(LED_PIN, 0);
            break;
            
        case LED_STATE_ON:
            gpio_put(LED_PIN, 1);
            break;
            
        case LED_STATE_BLINK:
            if (current_time - led_ts >= LED_BLINK_PERIOD_US) {
                gpio_put(LED_PIN, !gpio_get(LED_PIN));
                led_ts = current_time;
            }
            break;
            
        default:
            break;
    }
}

void led_task_state_set(led_state_t state)
{
    led_state = state;
    led_ts = time_us_64();  
    switch (state) {
        case LED_STATE_OFF:
            gpio_put(LED_PIN, 0);
            break;
            
        case LED_STATE_ON:
            gpio_put(LED_PIN, 1);
            break;
            
        case LED_STATE_BLINK:
            gpio_put(LED_PIN, 0);  
            break;
    }
}


void led_task_set_blink_period(uint32_t period_ms)
{
    if (period_ms > 0) {
        LED_BLINK_PERIOD_US = period_ms * 1000;
    }
}
