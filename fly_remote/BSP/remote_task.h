#ifndef __REMOTE_TASK_H_
#define __REMOTE_TASK_H_

#include "adc.h"
#include "freeRTOS.h"
#include "task.h"
#include "receive_task.h"

extern TaskHandle_t remote_task_handle;

void remote_task(void);

#endif
