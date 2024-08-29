#include "stepper.h"
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"
static const char *TAG = "stepper";

void init_stepper(void *arg) {
    gpio_set_direction(GPIO_DIR, GPIO_MODE_OUTPUT);

    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_12_BIT,  // -bit 
        .freq_hz          = 2000,  // 500Hz frequency
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = GPIO_STEP,
        .duty           = 2048,  // 50% duty cycle 
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);
    
    
}

void set_stepper_frequency(int freq) {
    // Set the frequency of the PWM signal
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, freq);
	ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 1024);
	ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);


        
        for (int i = 0; i < STEPS_PER_REVOLUTION; i++) {
            esp_rom_delay_us(10); 
            if (i % 10 == 0) {  
                vTaskDelay(1); 
            }
        }
		ledc_stop(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);
		vTaskDelay(10);
}

void update_stepper_frequency(int freq){

}

void set_stepper_direction(int direction) {
    // Set the GPIO level for the DIR pin (0 or 1)
    gpio_set_level(GPIO_DIR, direction);

    ESP_LOGI(TAG, "Stepper direction set to %d", direction);
}
