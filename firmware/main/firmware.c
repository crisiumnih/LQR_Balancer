#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mpu.h"
#include "sdkconfig.h"
#include "stepper.h"
#include <stdint.h>
#include <stdio.h>

static const char *TAG = "State Variables: ";
static const char *TAG3 = "System: ";
static const char *TAG2 = "Frequencies provided: ";
int direction_step = 0;
#include "sra_board.h"

const float K[] = {-0.5924, -3.2264, -0.1000};
const float final_state[] = {0, 0, 0};
float base_freq = 300;
float K_u = 4000;
float step_count = 0;
float step_to_rad = 0.0314;
float phi_dot = 0.001;
int base_max = 1200;
int base_min = 25;
float deg_to_rad=0.0175;

/*state_eqn sets up the theta and theta_dot in state space variables*/

void state_eqn(float err[]) {
    float euler_angle[2], mpu_offset[2] = {0.0f, 0.0f};
    if (enable_mpu6050() == ESP_OK) {
        if (read_mpu6050(euler_angle, mpu_offset) == ESP_OK) {

            float current_angle_1 = (euler_angle[1]) * deg_to_rad;

            vTaskDelay(pdMS_TO_TICKS(10));
            float current_angle_2 = (euler_angle[1]) * deg_to_rad;
            ESP_LOGI(TAG, "Pitch: %0.2f", euler_angle[1]);

            err[0] = final_state[0] - current_angle_2;
            float d_theta1 = (current_angle_2 - current_angle_1) / 0.01;
            err[1] = final_state[1] - d_theta1;
            if (current_angle_2 >= 0)
                direction_step = 1;
            else
                direction_step = 0;
        }
    }

}




float get_phi_dot() {
    float angle1 = get_phi_angle();
    vTaskDelay(pdMS_TO_TICKS(30));
    float angle2 = get_phi_angle();
    ESP_LOGI(TAG,"\nphi:%f\n", angle2);
    phi_dot = (angle2 - angle1) / 0.03;
    return phi_dot;
}

/*To obtain required feedback angular acceleration of motor*/

float set_ang_acc(float theta, float theta_dot, float phi_dot) {
    return -(K[0] * theta + K[1] * theta_dot + K[2] * phi_dot);
}



void set_stepper_freq(float phi_ddot, float pitch) {
    int freq;
    if (phi_ddot == 0)
        ESP_LOGE(TAG3,"problem in freq");

    freq = phi_ddot * K_u;
    if (base_freq == 0) {
        base_freq = 1; // Prevent division by zero
    }
    if (freq >= base_max)
        freq = base_max;
    if (freq <= 25)
        freq = base_min;

    base_freq = freq;
    
    // function to setup freq of motor to the desired freq
    set_stepper_frequency(freq, pitch, direction_step);

    ESP_LOGI(TAG2,"Base freq: %f\n", base_freq);
}



void control_loop() {
    ESP_LOGI(TAG,"Control loop started\n");
    float err[3];
    
    state_eqn(err);
    err[2] = phi_dot;
    float feedback_acc = set_ang_acc(err[0], err[1], err[2]);
    ESP_LOGI(TAG2,"Feedback Accn: %f\n", feedback_acc);

    // set motor cw
    set_stepper_direction(direction_step);

    if (feedback_acc < 0) {
        feedback_acc = -(feedback_acc);
    }

    
    phi_dot = get_phi_dot();
    float euler_angle[2], mpu_offset[2] = {0.0f, 0.0f};
    if (read_mpu6050(euler_angle, mpu_offset) == ESP_OK) {
        set_stepper_freq(feedback_acc, euler_angle[1]);
    }
}

void stepper_task(void *pvParameters) {
    init_stepper(NULL);
    ESP_LOGI(TAG,"MOTOR INITIALISED\n");

    while (1) {
        set_stepper_direction(direction_step);
        set_stepper_frequency(base_freq, 1, direction_step);
        vTaskDelay(10);
    }
}

void control_task(void *pvParameters){

    while (1) {
        
        vTaskDelay(10);
        control_loop();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main(void) {

    xTaskCreatePinnedToCore(&stepper_task, "stepper_task", 8192, NULL, 1, NULL,
                            0);
    ESP_LOGI(TAG3,"Stepper task created\n");

    xTaskCreatePinnedToCore(&control_task, "control_task", 8192, NULL, 2, NULL,
                            1);
    ESP_LOGI(TAG3,"Control task created\n");

    while (1) {
        ESP_LOGI(TAG3,"Main loop running\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}