#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "led-task/led-task.h"
#include "bme280-driver.h"

#define DEVICE_NAME "RP2040 Device"
#define DEVICE_VRSN "1.0.0"
#define BME280_ADDR 0x76

void version_callback(const char* args)
{
    printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void led_set_period_callback(const char* args)
{
    if (args == NULL || args[0] == '\0') {
        printf("Error: Period value not specified\n");
        printf("Usage: set_period <period_ms>\n");
        return;
    }
    
    uint32_t period_ms = 0;
    int result = sscanf(args, "%u", &period_ms);
    
    if (result != 1 || period_ms == 0) {
        printf("Error: Invalid period value. Please specify positive number (ms)\n");
        return;
    }
    
    led_task_set_blink_period(period_ms);
    printf("LED blink period set to %u ms\n", period_ms);
    
    if (led_task_state_get() == LED_STATE_BLINK) {
        printf("LED is currently blinking with new period %u ms\n", period_ms);
    }
}

void led_on_callback(const char* args)
{
    led_task_state_set(LED_STATE_ON);
    printf("LED turned ON\n");
}

void write_reg_callback(const char* args)
{
    if (args == NULL || args[0] == '\0') {
        printf("Error: Register address and value not specified\n");
        printf("Usage: write_reg <addr> <value>\n");
        printf("Example: write_reg f2 01\n");
        return;
    }
    
    uint32_t addr, value;
    int result = sscanf(args, "%x %x", &addr, &value);
    
    if (result != 2) {
        printf("Error: Invalid parameters. Usage: write_reg <addr> <value>\n");
        printf("Example: write_reg f2 01\n");
        return;
    }
    
    if (addr > 0xFF) {
        printf("Error: Register address 0x%X exceeds 0xFF\n", addr);
        return;
    }
    
    if (value > 0xFF) {
        printf("Error: Register value 0x%X exceeds 0xFF\n", value);
        return;
    }
    
    bme280_write_reg((uint8_t)addr, (uint8_t)value);
    printf("Write to register [0x%02X] = 0x%02X completed\n", addr, value);
}

void led_off_callback(const char* args)
{
    led_task_state_set(LED_STATE_OFF);
    printf("LED turned OFF\n");
}

void rp2040_i2c_read(uint8_t* buffer, uint16_t length)
{
    i2c_read_timeout_us(i2c1, BME280_ADDR, buffer, length, false, 100000);
}

void rp2040_i2c_write(uint8_t* data, uint16_t size)
{
    i2c_write_timeout_us(i2c1, BME280_ADDR, data, size, false, 100000);
}

void led_blink_callback(const char* args)
{
    if (args != NULL && args[0] != '\0') {
        uint32_t period_ms = 0;
        int result = sscanf(args, "%u", &period_ms);
        
        if (result == 1 && period_ms > 0) {
            led_task_set_blink_period(period_ms);
            printf("LED blink period set to %u ms\n", period_ms);
        } else {
            printf("Warning: Invalid period value. Using current period: %u ms\n", 
                   led_task_get_blink_period());
        }
    } else {
        printf("Using current blink period: %u ms\n", led_task_get_blink_period());
    }
    
    led_task_state_set(LED_STATE_BLINK);
    printf("LED blinking started with period %u ms\n", led_task_get_blink_period());
}

void led_status_callback(const char* args)
{
    led_state_t current_state = led_task_state_get();
    uint32_t current_period = led_task_get_blink_period();
    
    printf("\n=== LED Status ===\n");
    printf("State: ");
    switch(current_state) {
        case LED_STATE_OFF:
            printf("OFF\n");
            break;
        case LED_STATE_ON:
            printf("ON\n");
            break;
        case LED_STATE_BLINK:
            printf("BLINKING\n");
            printf("Blink period: %u ms\n", current_period);
            break;
        default:
            printf("UNKNOWN\n");
    }
    printf("==================\n");
}

void handle_read_regs_command(uint8_t addr, uint8_t n)
{
    if (addr > 0xFF) {
        printf("Error: Address 0x%X exceeds 0xFF\n", addr);
        return;
    }
    
    if (n > 0xFF) {
        printf("Error: Count %d exceeds 0xFF\n", n);
        return;
    }
    
    if (addr + n > 0x100) {
        printf("Error: Address range 0x%X - 0x%X exceeds register space\n", addr, addr + n - 1);
        return;
    }
    
    uint8_t buffer[256] = {0};
    bme280_read_regs(addr, buffer, n);
    
    for (int i = 0; i < n; i++) {
        printf("bme280 register [0x%X] = 0x%02X\n", addr + i, buffer[i]);
    }
}

void read_regs_callback(const char* args)
{
    if (args == NULL || args[0] == '\0') {
        printf("Error: Address and count not specified\n");
        printf("Usage: read_regs <addr> <count>\n");
        printf("Example: read_regs d0 1\n");
        return;
    }
    
    uint32_t addr, count;
    int result = sscanf(args, "%x %u", &addr, &count);
    
    if (result != 2) {
        printf("Error: Invalid parameters. Usage: read_regs <addr> <count>\n");
        printf("Example: read_regs d0 1\n");
        return;
    }
    
    handle_read_regs_command((uint8_t)addr, (uint8_t)count);
}

void help_callback(const char* args)
{
    printf("\nAvailable commands:\n");
    printf("  help                 - show this help\n");
    printf("  version              - get device info\n");
    printf("  on                   - turn LED on\n");
    printf("  off                  - turn LED off\n");
    printf("  blink [period]       - make LED blink (period in ms, default 500ms)\n");
    printf("  set_period <period>  - set LED blink period without changing state\n");
    printf("  status               - show current LED status and settings\n");
    printf("  read_regs <addr> <count> - read BME280 registers\n");
    printf("  write_reg <addr> <value> - write to BME280 register\n");
    printf("\nExamples:\n");
    printf("  blink 1000           - blink with 1 second period\n");
    printf("  set_period 2000      - set period to 2 seconds\n");
    printf("  status               - check current settings\n");
    printf("  read_regs d0 1       - read BME280 chip ID (should be 0x60)\n");
    printf("  write_reg f2 01      - write 0x01 to register 0xF2 (config register)\n");
    printf("\n");
}

api_t device_api[] =
{
    {"help", help_callback, "show this help"},
    {"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "turn LED on"},
    {"off", led_off_callback, "turn LED off"},
    {"blink", led_blink_callback, "make LED blink [period_ms]"},
    {"set_period", led_set_period_callback, "set LED blink period in ms"},
    {"status", led_status_callback, "get current LED status"},
    {"read_regs", read_regs_callback, "read BME280 registers: read_regs <addr> <count>"},
    {"write_reg", write_reg_callback, "write to BME280 register: write_reg <addr> <value>"},
    {NULL, NULL, NULL},
};

int main()
{
    stdio_init_all();
    stdio_task_init();
    protocol_task_init(device_api);
    led_task_init(device_api);
    
    i2c_init(i2c1, 100000);
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);
    gpio_pull_up(14);
    gpio_pull_up(15);
    
    bme280_init(rp2040_i2c_read, rp2040_i2c_write);
    printf("BME280 driver initialized\n");
    
    printf("\nReading BME280 chip ID...\n");
    handle_read_regs_command(0xD0, 1);
    
    printf("\n========================================\n");
    printf("Device started: %s v%s\n", DEVICE_NAME, DEVICE_VRSN);
    printf("========================================\n");
    
    help_callback(NULL);
    printf("\nEnter command: ");
    
    while (1)
    {
        char* command_string = stdio_task_handle();
        
        if (command_string != NULL)
        {
            protocol_task_handle(command_string);
            printf("\nEnter command: ");
            fflush(stdout);
        }
        
        led_task_handle(NULL);
        sleep_ms(10);
    }
    
    return 0;
}
