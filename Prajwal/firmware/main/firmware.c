#include <stdint.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_rom_sys.h"  
#include "esp_timer.h"
#include "mpu.h"
#include "stepper.h"



const float K[] ={-0.5924 ,-3.2264, -0.1000};
const float final_state[] = {0, 0 ,0};
float base_freq = 500;
float K_u= 200;
float step_count = 0;
float step_to_rad = 0.0314;
float phi_do = 0;





void state_eqn(float err[]){
    
    float current_1 = read_mpu();
    
    vTaskDelay(pdMS_TO_TICKS(10));
    float current_2 = read_mpu();
    
    err[0]=final_state[0]-current_2;
    float d_theta1 = (current_2-current_1)/0.01;
    err[1]=final_state[1]-d_theta1;
    
    


    // float dd_theta = (d_theta2-d_theta1)/0.01;
    // err[2]=final_state[2]-dd_theta;



}

// supposing MPU reading for upright is 90 hence 50 steps available in each direction as of now
float get_phid(__int64_t time_req){
    float angle1 = time_req*base_freq*0.0314;
    vTaskDelay(pdMS_TO_TICKS(10));
    float angle2 = (time_req+10)*base_freq*0.0314;
    phi_do=(angle2-angle1)/0.01;
    return phi_do;
}

float set_ang_acc(float theta, float theta_dot, float phi_dot){
    return -(K[0]*theta + K[1]*theta_dot + K[2]*phi_dot);
}


void set_stepper_freq(float phi_ddot){
    int freq;
    freq=phi_ddot*K_u;
    base_freq=freq;
    // function to setup freq of motor to the desired freq
    set_stepper_frequency(freq);

    // function over
    


}

void control_loop(){
    float err[3];
    __int64_t start_time = esp_timer_get_time(); 
    state_eqn(err);
    err[2]=phi_do;
    float feedback_acc= set_ang_acc(err[0], err[1], err[2]);
    __int64_t end_time = esp_timer_get_time();
    __int64_t time_req = end_time-start_time;
    phi_do=get_phid(time_req*1000);
    if(feedback_acc>=0){
        //set motor cw
        set_stepper_direction(0);
    }
    else{
        //set motor acw
        set_stepper_direction(1);
    }
    set_stepper_freq(feedback_acc);

}

void control_task(void *pvParameters) {
    while(1) {
        control_loop();
        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}




void app_main(void)
{
    xTaskCreate(&control_task, "control_task", 4096, NULL, 5, NULL);


}
