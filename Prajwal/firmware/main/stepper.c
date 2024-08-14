#include "stepper.h"

void init_stepper(void *arg) {
    gpio_set_direction(GPIO_DIR, GPIO_MODE_OUTPUT);

    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_9_BIT,  // 9-bit resolution
        .freq_hz          = 1000,  // Default frequency
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
}

void set_stepper_frequency(int frequency_hz) {
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, frequency_hz);
}

void set_stepper_direction(int direction) {
    gpio_set_level(GPIO_DIR, direction);
}
