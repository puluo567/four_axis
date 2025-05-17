#ifndef __PID_BSP_H_
#define __PID_BSP_H_

#include "scheduler.h"

#define LIMIT_MIN_MAX(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

typedef struct
{
    float kp, ki, kd;
    float mesure;//����ֵ
    float error;//���
    float last_error;//��һ�����
    float last_last_error;//���ϴ����
    float integral;//����
    float derivative;//΢��
    float output;//���
    float max_output;//������
    float min_output;//��С���
    float integral_max;//�����޷�
    float integral_min;
}pid_t;

void pid_init(pid_t *pid, float kp, float ki, float kd, float max_output, float min_output, float integral_max, float integral_min);
void pid_update_position(pid_t *pid, float measure, float desire, float dt);
void pid_update_increment(pid_t *pid, float measure, float desire, float dt);

#endif
