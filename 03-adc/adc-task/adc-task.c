#include "hardware/adc.h"
#include "adc-task.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const uint GPIO_PIN = 26;
const uint ADC_PIN = 0;
const uint ADC_TEMPRATURE = 4;

adc_task_state_t adc_state = ADC_TASK_STATE_IDLE;
uint64_t adc_ts = 0;
const uint ADC_TASK_MEAS_PERIOD_US = 100000;

void adc_task_init()
{
 adc_init();
  adc_set_temp_sensor_enabled(true);
 adc_gpio_init(GPIO_PIN);
 
 printf("ADC inicialized on GPIO ^^ %d, channel %d\n", GPIO_PIN, ADC_PIN);
 
}

void adc_task_set_state(adc_task_state_t state)
{
adc_state = state;
    adc_ts = time_us_64();  
    
    if (state == ADC_TASK_STATE_RUN)
    {
      printf("Telemetry started\n");
    }
    else
    {
     printf("Telemetry stop\n");
    }
}

float adc_task_read_temprature()
{
  adc_select_input(ADC_TEMPRATURE);
  
  uint16_t temprature_counts = adc_read();
  
  float temp_V = (temprature_counts * 3.3f) / 4096.0f;
  
  float temp_C = 27.0f - (temp_V - 0.706f) / 0.001721f;
  
  return temp_C;
}

void adc_task_handle()  
{
    uint64_t current_time = time_us_64();
    
    switch (adc_state)
    {
        case ADC_TASK_STATE_RUN:
            if (current_time - adc_ts >= ADC_TASK_MEAS_PERIOD_US) {
               float voltage_V = adc_task_read_voltage();
                float temp_C = adc_task_read_temprature(); 
                printf("%f %f\n", voltage_V, temp_C);
                
                adc_ts = current_time;
            }
            break;
            
        case ADC_TASK_STATE_IDLE:
            break;
            
        default:
            break;
    }
}

float adc_task_read_voltage(void)
{
  adc_select_input(ADC_PIN);
  
  uint16_t voltage_counts = adc_read();
  
  float voltage_V = (voltage_counts * 3.3f) / 4096.0f;
  
  return voltage_V;
}


