#include "hardware/adc.h"
#include "adc-task.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const uint GPIO_PIN = 26;
const uint ADC_PIN = 0;
const uint ADC_TEMPRATURE = 4;

void adc_task_init()
{
 adc_init();
  adc_set_temp_sensor_enabled(true);
 adc_gpio_init(GPIO_PIN);
 
 printf("ADC inicialized on GPIO ^^ %d, channel %d\n", GPIO_PIN, ADC_PIN);
 
}

float adc_task_read_voltage(void)
{
  adc_select_input(ADC_PIN);
  
  uint16_t voltage_counts = adc_read();
  
  float voltage_V = (voltage_counts * 3.3f) / 4096.0f;
  
  return voltage_V;
}

float adc_task_read_temprature()
{
  adc_select_input(ADC_TEMPRATURE);
  
  uint16_t temprature_counts = adc_read();
  
  float temp_V = (temprature_counts * 3.3f) / 4096.0f;
  
  float temp_C = 27.0f - (temp_V - 0.706f) / 0.001721f;
  
  return temp_C;
}
