#ifndef __RECEIVE_TASK_H_
#define __RECEIVE_TASK_H_

#include "usart.h"
#include "string.h"
#include "stdio.h"

#include "freeRTOS.h"
#include "task.h"

typedef struct
{
    uint8_t rx_buffer[30];
    char rx_data[30];
    uint8_t flag;
}uart_t;

void receive_init(void);

/*************************************** ESP32 UART3 START  ************************************************/
extern TaskHandle_t esp32_receive_task_handle;
extern uart_t uart3_inf;

void esp32_receive_IT(void);
void esp32_receive_task(void);
/*************************************** ESP32 UART3 END  ************************************************/


/*************************************** MASTER UART1 START  ************************************************/
extern TaskHandle_t master_receive_task_handle;
extern uart_t uart1_inf;
void master_receive_IT(void);
void master_receive_task(void);
/*************************************** MASTER UART1 END  ************************************************/
#endif
