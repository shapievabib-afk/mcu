#pragma once
//#ifdef ADC_TASK_H
//#define ADC_TASK_H
#include <stdint.h>
#include "../adc-task/adc-task.h"

float adc_task_read_voltage(void);
float adc_task_read_temprature(void);
//void led_task_handle(char* command_string);

//void led_task_state_set(led_state_t state);
