#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_rom_sys.h"  // Include the header for esp_rom_delay_us

#define GPIO_STEP    GPIO_NUM_14
#define GPIO_DIR     GPIO_NUM_12

#define STEPS_PER_REVOLUTION 200

void init_stepper(void *arg) {
    gpio_set_direction(GPIO_DIR, GPIO_MODE_OUTPUT);

    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_9_BIT,  // 9-bit resolution
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
        .duty           = 256,  // 50% duty cycle for 9-bit resolution
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    while(1) {
        gpio_set_level(GPIO_DIR, 0);

        // Set frequency and start PWM
        ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, 500);
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 256); 
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

        // Replace ms delay with 10 microseconds delay
        for (int i = 0; i < STEPS_PER_REVOLUTION; i++) {
            esp_rom_delay_us(10);  // 10 microseconds delay

            // Yield control to the scheduler
            if (i % 10 == 0) {  // Adjust modulus to control yielding frequency
                vTaskDelay(1);  // Yield control back to FreeRTOS
            }
        }

        // Consider removing this if continuous motion is desired
        ledc_stop(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);
    }
}

void app_main(void) {
    BaseType_t result = xTaskCreate(init_stepper, "stepper", 4096, NULL, 5, NULL);
    if (result != pdPASS) {
        ESP_LOGE("app_main", "Failed to create init_stepper task!");
        while(1) {
            // Stay here to prevent returning if the task creation fails
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
    // Optionally, add a loop to keep the app_main running
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000)); // Idle loop to keep main alive
    }
}
