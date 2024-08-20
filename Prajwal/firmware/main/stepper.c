#include "stepper.h"
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"
static const char *TAG = "stepper";

void init_stepper(void *arg) {
    // Set the GPIO direction for the DIR pin
    gpio_set_direction(GPIO_DIR, GPIO_MODE_OUTPUT);

    // Timer configuration for PWM
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_10_BIT,  // 9-bit resolution
        .freq_hz          = 1000,               // Initial frequency set to 500 Hz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Channel configuration for PWM on the STEP pin
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = GPIO_STEP,
        .duty           = 512,  // 50% duty cycle for 9-bit resolution
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    ESP_LOGI(TAG, "Stepper motor initialized");
    
    while (1) {
        // Continuous operation example, modify as needed
        set_stepper_direction(0);  // Set direction
        set_stepper_frequency(500); // Set frequency
        
        int delay_ms = (1000 * STEPS_PER_REVOLUTION) / 500;
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

void set_stepper_frequency(int freq) {
    // Set the frequency of the PWM signal
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, freq);
    // Set the duty cycle to 50% to maintain step pulses
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 512);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

    ESP_LOGI(TAG, "Stepper frequency set to %d Hz", freq);
}

void set_stepper_direction(int direction) {
    // Set the GPIO level for the DIR pin (0 or 1)
    gpio_set_level(GPIO_DIR, direction);

    ESP_LOGI(TAG, "Stepper direction set to %d", direction);
}
