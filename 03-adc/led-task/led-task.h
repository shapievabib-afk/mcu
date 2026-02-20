#pragma once

#include <stdint.h>
#include "../protocol-task/protocol-task.h"

typedef enum
{
	LED_STATE_OFF = 0,
	LED_STATE_ON = 1,
	LED_STATE_BLINK = 2,
} led_state_t;

void led_task_init(api_t* device_api);

void led_task_handle(char* command_string);

void led_task_state_set(led_state_t state);
