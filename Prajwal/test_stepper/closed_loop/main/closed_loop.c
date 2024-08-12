#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_rom_sys.h"  

#define GPIO_STEP    GPIO_NUM_14
#define GPIO_DIR     GPIO_NUM_12

#define STEPS_PER_REVOLUTION 200

void init_stepper(void *arg) {
    gpio_set_direction(GPIO_DIR, GPIO_MODE_OUTPUT);

    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_9_BIT,  // 9-bit 
        .freq_hz          = 500,  // 500 Hz frequency
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = GPIO_STEP,
        .duty           = 256,  // 50% duty cycle 
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    while(1) {
        gpio_set_level(GPIO_DIR, 0);

        
        ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, 500);
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 256); 
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

        
        for (int i = 0; i < STEPS_PER_REVOLUTION; i++) {
            esp_rom_delay_us(10); 
            if (i % 10 == 0) {  
                vTaskDelay(1); 
            }
        }

       
        ledc_stop(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);
    }
}

void app_main(void) {
    BaseType_t result = xTaskCreate(init_stepper, "stepper", 4096, NULL, 5, NULL);
    if (result != pdPASS) {
        ESP_LOGE("app_main", "Failed to create init_stepper task!");
        while(1) {
          
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
}
