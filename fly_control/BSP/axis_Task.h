#ifndef __AXIS_TASK_H_
#define __AXIS_TASK_H_

#include "tim.h"
#include "freeRTOS.h"
#include "task.h"
#include "pid_bsp.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#define MOTOR1_PWM TIM2->CCR1
#define MOTOR2_PWM TIM2->CCR2
#define MOTOR3_PWM TIM2->CCR3
#define MOTOR4_PWM TIM2->CCR4

enum
{
    ROLL = 0,
    PITCH,
    YAW,
    ATTITUDE_NUM,
    LEFT_DU = 0,//左摇杆上下
    LEFT_LR,//左摇杆左右
    RIGHT_DU,
    RIGHT_LR,
    ADC_NUM,
    SW1 = 0,
    SW2,
    SW3,
    SW4,
    SW_NUM,
};

typedef struct
{
    float measure;//测量值
    float desire;//期望值
}attitude_t;

typedef struct //遥控器
{
    uint32_t adc_value[ADC_NUM];//ADC值
    uint8_t key_value[5];//按键值,这里用不上
    uint8_t sw_value[SW_NUM];//开关值
}remote_t;


typedef struct
{
    uint32_t a_value;//油门大小控制
    uint32_t time_out;//超时控制
    remote_t remote;//遥控器数据
    attitude_t attitude[ATTITUDE_NUM];//姿态控制
}axis_t;

extern axis_t axis_inf;

extern TaskHandle_t axis_control_task_handle;

void axis_init(void);
void axis_control_task(void);

#endif
