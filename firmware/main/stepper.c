#include "esp_rom_sys.h"
#include "stepper.h"
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"
static const char *TAG = "stepper";
float phi = 0;

void init_stepper(void *arg) {
    gpio_set_direction(GPIO_DIR, GPIO_MODE_OUTPUT);

    ledc_timer_config_t ledc_timer = {.speed_mode = LEDC_HIGH_SPEED_MODE,
                                      .timer_num = LEDC_TIMER_0,
                                      .duty_resolution =
                                          LEDC_TIMER_12_BIT, // -bit
                                      .freq_hz = 2000,       // 500Hz frequency
                                      .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {.speed_mode = LEDC_HIGH_SPEED_MODE,
                                          .channel = LEDC_CHANNEL_0,
                                          .timer_sel = LEDC_TIMER_0,
                                          .intr_type = LEDC_INTR_DISABLE,
                                          .gpio_num = GPIO_STEP,
                                          .duty = 2048, // 50% duty cycle
                                          .hpoint = 0};
    ledc_channel_config(&ledc_channel);
}

void set_stepper_frequency(int freq, float pitch) {

    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, freq);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 1024);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    if (freq < 50)
        freq = 50;
    if (freq > 1000)
        freq = 1000;

    freq = (freq / 25) * 25;
    float steps = 1.8;
    float us;
    if (freq == 0) {
        us = 1;
    } else {
        us = 1.00 / freq;
    }

    int angle_to_move = pitch * STEPS_PER_REVOLUTION / 360;
    for (int i = 0; i < angle_to_move; i++) {
        if (phi >= 360.0) {
            phi = 0;
        }
        phi += steps;
        esp_rom_delay_us(us * 1000000);
    }
    vTaskDelay(10);
    ledc_stop(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);
}

float get_phi_angle() { return phi; }

void set_stepper_direction(int direction) {
    // Set the GPIO level for the DIR pin (0 or 1)
    gpio_set_level(GPIO_DIR, direction);

    ESP_LOGI(TAG, "Stepper direction set to %d", direction);
}

void stepper_idle() {
    ledc_stop(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);

    // Set the direction pin to low
    gpio_set_level(GPIO_DIR, 0);

    ESP_LOGI(TAG, "Stepper motor set to idle state");
}