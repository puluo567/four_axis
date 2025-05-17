#ifndef __RECEIVE_TASK_H_
#define __RECEIVE_TASK_H_

#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "axis_Task.h"
#include "freeRTOS.h"
#include "task.h"
#include "queue.h"

typedef struct
{
    uint8_t flag;
}uart_t;

extern uart_t uart_inf;

extern uint8_t rx_buffer[30];
extern char rx_data[30];

extern TaskHandle_t remote_receive_task_handle;
extern QueueHandle_t remote_queue_handle;

void receive_init(void);
void remote_receive_IT(void);
void remote_receive_Task(void);

#endif
