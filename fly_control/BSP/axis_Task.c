#include "axis_Task.h"
// PWM1 -> TIM2_CH1 PWM2 -> TIM2_CH2 PWM3 -> TIM2_CH3 PWM4 -> TIM2_CH4
// PA0 PA1 PB10 PB11
//ROLL ?X??? PITCH ?Y??? YAW ?Z???

/*
      M1   M2
       \   /
        \ /
        / \
      M3   M4
*/

TaskHandle_t axis_control_task_handle = NULL;

axis_t axis_inf = {0};
pid_t roll_pid, pitch_pid, yaw_pid;

void axis_init(void)
{
    MPU_Init();			//MPU6050???
    mpu_dmp_init();		//dmp???
    while(mpu_dmp_get_data(&axis_inf.attitude[PITCH].desire, &axis_inf.attitude[ROLL].desire, &axis_inf.attitude[YAW].desire));
    TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = 0;
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_ALL);
}

void axis_data_get(void)
{
    if(pdPASS == xQueueReceive(remote_queue_handle, &axis_inf.a_value, 0))
    {
        while(mpu_dmp_get_data(&axis_inf.attitude[PITCH].measure, &axis_inf.attitude[ROLL].measure, &axis_inf.attitude[YAW].measure));
        axis_inf.time_out = HAL_GetTick();
    }
    else
    {
        uint32_t now_time = HAL_GetTick();
        if(now_time - axis_inf.time_out >= 1000)
        {
            axis_inf.a_value = 0;
            axis_inf.time_out = now_time;
        }
    }
}

void axis_adjust(void)
{
    pid_update_position(&roll_pid, axis_inf.attitude[ROLL].measure, axis_inf.attitude[ROLL].desire, 0.01f);
    pid_update_position(&pitch_pid, axis_inf.attitude[PITCH].measure, axis_inf.attitude[PITCH].desire, 0.01f);
    pid_update_position(&yaw_pid, axis_inf.attitude[YAW].measure, axis_inf.attitude[YAW].desire, 0.01f);
    MOTOR1_PWM = axis_inf.a_value - pitch_pid.output + roll_pid.output + yaw_pid.output;
    MOTOR2_PWM = axis_inf.a_value - pitch_pid.output - roll_pid.output - yaw_pid.output;
    MOTOR3_PWM = axis_inf.a_value + pitch_pid.output + roll_pid.output - yaw_pid.output;
    MOTOR4_PWM = axis_inf.a_value + pitch_pid.output - roll_pid.output + yaw_pid.output;
}

void axis_control_task(void)
{
    while(1)
    {
        axis_data_get();
        axis_adjust();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

