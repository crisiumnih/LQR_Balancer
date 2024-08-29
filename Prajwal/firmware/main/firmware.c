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

static const char *TAG = "mpu_test";

#include "sra_board.h"

const float K[] = {-0.5924, -3.2264, -0.1000};
const float final_state[] = {0, 0, 0};
float base_freq = 500;
float K_u = 0.5;
float step_count = 0;
float step_to_rad = 0.0314;
float phi_dot = 0.001;

void state_eqn(float err[]) {
  float euler_angle[2], mpu_offset[2] = {0.0f, 0.0f};
  if (enable_mpu6050() == ESP_OK) {
    if (read_mpu6050(euler_angle, mpu_offset) == ESP_OK) {

      float current_1 = (euler_angle[1] + mpu_offset[1]) * 0.0175;

      vTaskDelay(pdMS_TO_TICKS(10));
      float current_2 = (euler_angle[1] + mpu_offset[1]) * 0.0175;
      ESP_LOGI(TAG, "Roll: %0.2f | Pitch: %0.2f", euler_angle[0],
               euler_angle[1]);

      err[0] = final_state[0] - current_2;
      float d_theta1 = (current_2 - current_1) / 0.01;
      err[1] = final_state[1] - d_theta1;
    }
  }

  // float dd_theta = (d_theta2-d_theta1)/0.01;
  // err[2]=final_state[2]-dd_theta;
}

float get_phid(__int64_t time_req) {
  float angle1 = time_req * base_freq * 0.0314;
  vTaskDelay(pdMS_TO_TICKS(10));
  float angle2 = (time_req + 10) * base_freq * 0.0314;
  phi_dot = (angle2 - angle1) / 0.01;
  return phi_dot;
}

float set_ang_acc(float theta, float theta_dot, float phi_dot) {
  return -(K[0] * theta + K[1] * theta_dot + K[2] * phi_dot);
}

void set_stepper_freq(float phi_ddot) {
  int freq;
  if (phi_ddot == 0)
    printf("problem");
  freq = phi_ddot * K_u;
  if (base_freq == 0) {
    base_freq = 1; // Prevent division by zero
  }

  base_freq = freq;
  // function to setup freq of motor to the desired freq
  //set_stepper_frequency(freq);

  // function over
}

// feedback_acc is phi_ddot

void control_loop() {
  printf("control loop started");
  float err[3];
  __int64_t start_time = esp_timer_get_time();
  state_eqn(err);
  err[2] = phi_dot;
  float feedback_acc = set_ang_acc(err[0], err[1], err[2]);
  printf("%f\n",feedback_acc);
  if (feedback_acc >= 0) {
    // set motor cw
    set_stepper_direction(0);
  } else {
    // set motor acw
    set_stepper_direction(1);
  }
  if (feedback_acc == 0)
    feedback_acc = 20;
	if(feedback_acc<0){
		feedback_acc = -(feedback_acc);
	}
  
  __int64_t end_time = esp_timer_get_time();
  __int64_t time_req = end_time - start_time;
  phi_dot = get_phid(time_req * 1000);
  
  set_stepper_freq(feedback_acc);
}

void stepper_task(void *pvParameters) {
  init_stepper(NULL);
  printf("motro ok\n");

  while (1) {
    set_stepper_direction(0);
    set_stepper_frequency(base_freq);
    vTaskDelay(10);

    
  }
}

void control_task(void *pvParameters) {
  printf("control task initiated\n");

  while (1) {
    printf("control task loop\n");
    vTaskDelay(10);
    control_loop();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void app_main(void) {

  xTaskCreatePinnedToCore(&stepper_task, "stepper_task", 8192, NULL, 1, NULL,
                          0);
  printf("Stepper task created\n");

  xTaskCreatePinnedToCore(&control_task, "control_task", 8192, NULL, 2, NULL,
                          1);
  printf("Control task created\n");

  while (1) {
    printf("Main loop running\n");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}