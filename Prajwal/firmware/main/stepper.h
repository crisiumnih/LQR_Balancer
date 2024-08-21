#ifndef STEPPER_CONTROL_H
#define STEPPER_CONTROL_H
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_rom_sys.h"  

#define GPIO_STEP    GPIO_NUM_14
#define GPIO_DIR     GPIO_NUM_27

#define STEPS_PER_REVOLUTION 200

void init_stepper(void *arg);
void set_stepper_frequency(int freq);
void set_stepper_direction(int direction);
void stepper_update();
#endif