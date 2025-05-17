#include "receive_task.h"

void receive_init(void)
{
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart3, uart3_inf.rx_buffer, sizeof(uart3_inf.rx_buffer));
    __HAL_UART_CLEAR_IDLEFLAG(&huart3);

    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, uart1_inf.rx_buffer, sizeof(uart1_inf.rx_buffer));
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
}

/*************************************** ESP32 UART3 START  ************************************************/
TaskHandle_t esp32_receive_task_handle = NULL;

uart_t uart3_inf;

void esp32_receive_IT(void)
{
    if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE))
    {
        uart3_inf.flag = 1;
        HAL_UART_DMAStop(&huart3);
        memcpy(uart3_inf.rx_data, uart3_inf.rx_buffer, sizeof(uart3_inf.rx_buffer));
        uint8_t len = sizeof(uart3_inf.rx_buffer) - __HAL_DMA_GET_COUNTER(huart3.hdmarx);
        uart3_inf.rx_data[len] = '\0';
        memset(uart3_inf.rx_buffer, '0', sizeof(uart3_inf.rx_buffer));
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);
        HAL_UART_Receive_DMA(&huart3, uart3_inf.rx_buffer, sizeof(uart3_inf.rx_buffer));
    }
}

void esp32_receive_task(void)
{
    while(1)
    {
        if (uart3_inf.flag)
        {
            uart3_inf.flag = 0;
            
        }
        
        vTaskDelay(10);
    }
}
/*************************************** ESP32 UART3 END  ************************************************/



/*************************************** MASTER UART1 START  ************************************************/
TaskHandle_t master_receive_task_handle = NULL;
uart_t uart1_inf;

void master_receive_IT(void)
{
    if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
    {
        uart1_inf.flag = 1;
        HAL_UART_DMAStop(&huart1);
        memcpy(uart1_inf.rx_data, uart1_inf.rx_buffer, sizeof(uart1_inf.rx_buffer));
        uint8_t len = sizeof(uart1_inf.rx_buffer) - __HAL_DMA_GET_COUNTER(huart1.hdmarx);
        uart1_inf.rx_data[len] = '\0';
        memset(uart1_inf.rx_buffer, '0', sizeof(uart1_inf.rx_buffer));
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        HAL_UART_Receive_DMA(&huart1, uart1_inf.rx_buffer, sizeof(uart1_inf.rx_buffer));
    }
}

void master_receive_task(void)
{
    while(1)
    {
        if (uart1_inf.flag)
        {
            uart1_inf.flag = 0;
        }
        
        vTaskDelay(10);
    }
}
/*************************************** MASTER UART1 END  ************************************************/

/*************************************** UART START  ************************************************/

//发送格式 帧头+数据长度+数据+校验和
void uart_send_pack(UART_HandleTypeDef *huart, uint8_t *data, uint8_t len)
{
    uint8_t frame[100] = {0};
    frame[0] = 0xAA; //帧头
    frame[1] = len; //数据长度
    memcpy(&frame[2], data, len); //数据
    uint16_t check_sum = 0;
    for(uint8_t i = 0; i < len + 2; i++)
    {
        check_sum += frame[i];
    }
    frame[len + 2] = (uint8_t)(check_sum & 0xFF); //校验和仅保留低八位
    HAL_UART_Transmit_DMA(huart, frame, len + 3); //发送数据
}

void uart_receive_pack(uint8_t *data, uint8_t len)
{
    if(len > 3 && data[0] == 0xAA)
    {
        
    }
}

/*************************************** UART END  ************************************************/
