// led-task.h
#ifndef LED_TASK_H
#define LED_TASK_H

#include <stdint.h>

typedef enum {
    LED_STATE_OFF,
    LED_STATE_ON,
    LED_STATE_BLINK
} led_state_t;

// Инициализация LED задачи
void led_task_init(void* api);

// Управление состоянием
void led_task_state_set(led_state_t state);
led_state_t led_task_state_get(void);

// Управление периодом мигания
void led_task_set_blink_period(uint32_t period_ms);

uint32_t led_task_get_blink_period(void);

// Обработчик задачи (должен вызываться в цикле)
void led_task_handle(void* param);

#endif // LED_TASK_H
