#include "receive_task.h"

TaskHandle_t remote_receive_task_handle = NULL;
QueueHandle_t remote_queue_handle = NULL;

uint8_t rx_buffer[30];
char rx_data[30];

uart_t uart_inf;

void receive_init(void)
{
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, rx_buffer, sizeof(rx_buffer));
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);    
}

void remote_receive_IT(void)
{
    if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
    {
        uart_inf.flag = 1;
        HAL_UART_DMAStop(&huart1);
        memcpy(rx_data, rx_buffer, sizeof(rx_buffer));
        uint8_t len = sizeof(rx_buffer) - __HAL_DMA_GET_COUNTER(huart1.hdmarx);
        rx_data[len] = '\0';
        memset(rx_buffer, '0', sizeof(rx_buffer));
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        HAL_UART_Receive_DMA(&huart1, rx_buffer, sizeof(rx_buffer));
    }
}
/*油门传输格式 a=%.0f!  占空比 大于 50% */
void remote_receive_Task(void)
{
    uint32_t queue_send = 0;
    remote_queue_handle = xQueueCreate(10, sizeof(uint32_t));
    while (1)
    {
        if(uart_inf.flag)
        {
            uart_inf.flag = 0;
            sscanf(rx_data, "a=%d!", &queue_send);
            xQueueSendToBack(remote_queue_handle, &queue_send, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
