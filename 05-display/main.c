#include <stdio.h>
#include <stdlib.h>
#include "hardware/spi.h"
#include <string.h>
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"
#include "ili9341-driver.h"
#include "ili9341-display.h"
#include "ili9341-font.h"

static ili9341_display_t ili9341_display = {0};
#define DEVICE_NAME "RP2040 Device"
#define DEVICE_VRSN "1.0.0"
#define ILI9341_PIN_MISO 4
#define ILI9341_PIN_CS 10
#define ILI9341_PIN_SCK 6
#define ILI9341_PIN_MOSI 7
#define ILI9341_PIN_DC 8
#define ILI9341_PIN_RESET 9

void version_callback(const char* args)
{
    printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void disp_screen_callback(const char* args)
{
    uint32_t c = 0;
    int result = sscanf(args, "%x", &c);
    
    uint16_t color = COLOR_PINK;
    
    if (result == 1)
    {
        color = RGB888_2_RGB565(c);
    }
    
    ili9341_fill_screen(&ili9341_display, color);
}

void rp2040_gpio_cs_write(bool level)
{
    gpio_put(ILI9341_PIN_CS, level);
}

void rp2040_gpio_dc_write(bool level)
{
    gpio_put(ILI9341_PIN_DC, level);
}

void rp2040_gpio_reset_write(bool level)
{
    gpio_put(ILI9341_PIN_RESET, level);
}

void rp2040_delay_ms(uint32_t ms)
{
    sleep_ms(ms);
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

void disp_px_callback(const char* args)
{
    uint32_t x = 0, y = 0, c = 0;
    int result = sscanf(args, "%u %u %x", &x, &y, &c);
    
    uint16_t color = COLOR_WHITE;
    
    if (result == 3)
    {
        color = RGB888_2_RGB565(c);
        ili9341_draw_pixel(&ili9341_display, x, y, color);
    }
}

void disp_line_callback(const char* args)
{
    uint32_t x1 = 0, y1 = 0, x2 = 0, y2 = 0, c = 0;
    int result = sscanf(args, "%u %u %u %u %x", &x1, &y1, &x2, &y2, &c);
    
    uint16_t color = COLOR_WHITE;
    
    if (result == 5)
    {
        color = RGB888_2_RGB565(c);
        ili9341_draw_line(&ili9341_display, x1, y1, x2, y2, color);
    }
}

void disp_rect_callback(const char* args)
{
    uint32_t x = 0, y = 0, w = 0, h = 0, c = 0;
    int result = sscanf(args, "%u %u %u %u %x", &x, &y, &w, &h, &c);
    
    uint16_t color = COLOR_WHITE;
    
    if (result == 5)
    {
        color = RGB888_2_RGB565(c);
        ili9341_draw_rect(&ili9341_display, x, y, w, h, color);
    }
}

void disp_frect_callback(const char* args)
{
    uint32_t x = 0, y = 0, w = 0, h = 0, c = 0;
    int result = sscanf(args, "%u %u %u %u %x", &x, &y, &w, &h, &c);
    
    uint16_t color = COLOR_WHITE;
    
    if (result == 5)
    {
        color = RGB888_2_RGB565(c);
        ili9341_draw_filled_rect(&ili9341_display, x, y, w, h, color);
    }
}

void disp_text_callback(const char* args)
{
    uint32_t x = 0, y = 0;
    uint32_t color_val = 0, bg_color_val = 0;
    char text[64] = {0};
    char font_name[32] = {0};
    
    int result = sscanf(args, "%u %u %63s %31s %x %x", 
                        &x, &y, text, font_name, &color_val, &bg_color_val);
    
    if(result == 6)
    {
        uint16_t color = RGB888_2_RGB565(color_val);
        uint16_t bg_color = RGB888_2_RGB565(bg_color_val);
        
        // Выбор шрифта по имени
        const ili9341_font_t *font = NULL;
        if(strcmp(font_name, "jetbrains") == 0) {
            font = &jetbrains_font;
        } else {
            font = &jetbrains_font;  // По умолчанию
        }
        
        ili9341_draw_text(&ili9341_display, x, y, text, font, color, bg_color);
    }
}

void led_on_callback(const char* args)
{
    led_task_state_set(LED_STATE_ON);
    printf("LED turned ON\n");
}

void led_off_callback(const char* args)
{
    led_task_state_set(LED_STATE_OFF);
    printf("LED turned OFF\n");
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

void rp2040_spi_write(const uint8_t *data, uint32_t size)
{
    spi_write_blocking(spi0, data, size);
}

void rp2040_spi_read(uint8_t *buffer, uint32_t length)
{
    spi_read_blocking(spi0, 0, buffer, length);
}

void help_callback(const char* args)
{
    printf("\nAvailable commands:\n");
    printf("  help                 - show this help\n");
    printf("  version              - get device info\n");
    printf("  disp_screen [color]  - fill screen with color (RGB888 hex)\n");
    printf("  on                   - turn LED on\n");
    printf("  off                  - turn LED off\n");
    printf("  blink [period]       - make LED blink (period in ms, default 500ms)\n");
    printf("  set_period <period>  - set LED blink period without changing state\n");
    printf("  status               - show current LED status and settings\n");
    printf("\nExamples:\n");
    printf("  disp_screen          - fill screen with black\n");
    printf("  disp_screen FF0000   - fill screen with red\n");
    printf("  disp_screen 00FF00   - fill screen with green\n");
    printf("  disp_screen 0000FF   - fill screen with blue\n");
    printf("  disp_screen FFFFFF   - fill screen with white\n");
    printf("  blink 1000           - blink with 1 second period\n");
    printf("  set_period 2000      - set period to 2 seconds\n");
    printf("  status               - check current settings\n");
    printf("\n");
}

api_t device_api[] =
{
    {"help", help_callback, "show this help"},
    {"version", version_callback, "get device name and firmware version"},
    {"disp_screen", disp_screen_callback, "fill screen with color (RGB888 hex)"},
    {"disp_px", disp_px_callback, " fill fixel with color (RGB888 hex)"},
    {"disp_line", disp_line_callback, "draw line x1 y1 x2 y2 color"},
    {"disp_rect", disp_rect_callback, "draw rect x y w h color"},
    {"disp_frect", disp_frect_callback, "draw filled rect x y w h color"},
    {"disp_text", disp_text_callback, "write a text on screen"},
    {"on", led_on_callback, "turn LED on"},
    {"off", led_off_callback, "turn LED off"},
    {"blink", led_blink_callback, "make LED blink [period_ms]"},
    {"set_period", led_set_period_callback, "set LED blink period in ms"},
    {"status", led_status_callback, "get current LED status"},
    {NULL, NULL, NULL},
};

int main()
{
    stdio_init_all();
    stdio_task_init();
    protocol_task_init(device_api);
    led_task_init(device_api);
    spi_init(spi0, 62500000);
    
    gpio_set_function(ILI9341_PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(ILI9341_PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(ILI9341_PIN_MOSI, GPIO_FUNC_SPI);

    gpio_init(ILI9341_PIN_CS);
    gpio_init(ILI9341_PIN_DC);
    gpio_init(ILI9341_PIN_RESET);
    
    gpio_set_dir(ILI9341_PIN_CS, GPIO_OUT);
    gpio_set_dir(ILI9341_PIN_DC, GPIO_OUT);
    gpio_set_dir(ILI9341_PIN_RESET, GPIO_OUT);
    
    gpio_put(ILI9341_PIN_CS, 1);
    gpio_put(ILI9341_PIN_DC, 0);
    gpio_put(ILI9341_PIN_RESET, 0);
    
    sleep_ms(10);
    gpio_put(ILI9341_PIN_RESET, 1);
    sleep_ms(120);
    
    ili9341_hal_t ili9341_hal = {0};
    ili9341_hal.spi_write = rp2040_spi_write;
    ili9341_hal.spi_read = rp2040_spi_read;
    ili9341_hal.gpio_cs_write = rp2040_gpio_cs_write;
    ili9341_hal.gpio_dc_write = rp2040_gpio_dc_write;
    ili9341_hal.gpio_reset_write = rp2040_gpio_reset_write;
    ili9341_hal.delay_ms = rp2040_delay_ms;
    
    ili9341_init(&ili9341_display, &ili9341_hal);
    ili9341_set_rotation(&ili9341_display, ILI9341_ROTATION_90);
    
    ili9341_fill_screen(&ili9341_display, COLOR_BLACK);
    sleep_ms(300);
    
    ili9341_draw_filled_rect(&ili9341_display, 10, 10, 100, 60, COLOR_RED);
    ili9341_draw_filled_rect(&ili9341_display, 120, 10, 100, 60, COLOR_GREEN);
    ili9341_draw_filled_rect(&ili9341_display, 230, 10, 80, 60, COLOR_BLUE);
    
    ili9341_draw_rect(&ili9341_display, 10, 90, 300, 80, COLOR_WHITE);
    
    ili9341_draw_line(&ili9341_display, 0, 0, 319, 239, COLOR_YELLOW);
    ili9341_draw_line(&ili9341_display, 319, 0, 0, 239, COLOR_CYAN);
    
    ili9341_draw_text(&ili9341_display, 20, 100, "Hello, ILI9341!", &jetbrains_font, COLOR_WHITE, COLOR_BLACK);
    ili9341_draw_text(&ili9341_display, 20, 116, "RP2040 / Pico SDK", &jetbrains_font, COLOR_YELLOW, COLOR_BLACK);
    
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
