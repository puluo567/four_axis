#include "remote_task.h"

#define REMOTE_ADC_NUM 4

typedef struct
{
    uint32_t adc_buffer[REMOTE_ADC_NUM];
    uint8_t key[5];
    uint8_t sw[4];
}remote_t;

remote_t remote_inf;

TaskHandle_t remote_task_handle = NULL;

/***************************************** ADC 采集 START ******************************************/


/*             min   mid    max      从下到上 从左到右
** left_du      0     X     4000     ADC0
** left_lr      50   1930   3600     ADC1 -->十分不稳  最小值在 0~350 之间
**right_du      0    1999   4000     ADC2
**right_lr      0    1960   4000     ADC3
*/

uint32_t ADC_Read(uint32_t Channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = Channel;                                         /* 通道 */
	sConfig.Rank = ADC_REGULAR_RANK_1;                              
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;                  /* 采样时间 */
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)             
	{
		Error_Handler();
	}
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 10);
	return HAL_ADC_GetValue(&hadc1);
}

void adc_get(void)
{
    remote_inf.adc_buffer[0] = ADC_Read(ADC_CHANNEL_0);               /* 读取 ADC0 */
    remote_inf.adc_buffer[1] = ADC_Read(ADC_CHANNEL_1);               /* 读取 ADC1 */
    remote_inf.adc_buffer[2] = ADC_Read(ADC_CHANNEL_2);               /* 读取 ADC2 */
    remote_inf.adc_buffer[3] = ADC_Read(ADC_CHANNEL_3);               /* 读取 ADC3 */
    HAL_ADC_Stop(&hadc1);                                             /* 停止 ADC */
}

/***************************************** ADC 采集 END ******************************************/

/***************************************** KEY START ******************************************/
#define KEY_NUM 5

typedef struct{
    GPIO_TypeDef *GPIOX;
    uint16_t GPIO_Pin;
    uint8_t value;//值
    uint8_t flag;//标志位
    uint8_t count;//短按次数
    uint32_t releas_time;//松开时间
    uint32_t press_time;//按下时间
}Key_t;

Key_t KEY[KEY_NUM] = {
    {GPIOB, GPIO_PIN_4, 0, 0, 0, 0, 0},
    {GPIOB, GPIO_PIN_5, 0, 0, 0, 0, 0},
    {GPIOB, GPIO_PIN_6, 0, 0, 0, 0, 0},
    {GPIOB, GPIO_PIN_7, 0, 0, 0, 0, 0},
    {GPIOA, GPIO_PIN_6, 0, 0, 0, 0, 0},
};

void key_get(void)
{
    for(uint8_t i = 0; i < KEY_NUM; i++)
    {
        uint32_t now_time = HAL_GetTick();
        if(HAL_GPIO_ReadPin(KEY[i].GPIOX, KEY[i].GPIO_Pin) == RESET)
        {
            switch (KEY[i].flag)
            {
                case 0:
                    KEY[i].press_time = now_time;
                    KEY[i].flag = 1;
                    break;
                case 1:
                    if(now_time - KEY[i].press_time >= 10)
                    {
                        KEY[i].press_time = now_time;
                        KEY[i].flag = 2;
                    }
                    break;
                case 2:
                    if(now_time - KEY[i].press_time >= 500)
                    {
                        KEY[i].flag = 3;
                        KEY[i].value = 2;
                        KEY[i].count = 0;
                    }
                    break;
                default:
                    break;
            }
        }
        else
        {
            if(KEY[i].flag == 2)
            {
                KEY[i].releas_time = now_time;
                KEY[i].count++;
            }
            
            KEY[i].flag = 0;
        }
        if((KEY[i].count > 0) && (now_time - KEY[i].releas_time >= 50))
        {
            switch(KEY[i].count)
            {
                case 1:
                    KEY[i].value = 1;break;
                case 2:
                    KEY[i].value = 3;break;
                case 3:
                    KEY[i].value = 4;break;
            }
            KEY[i].count = 0;
            KEY[i].flag = 0;
        }
        remote_inf.key[i] = KEY[i].value;
    }
}
/***************************************** KEY END ******************************************/

/***************************************** SW START ******************************************/
//往上拨动为低电平，往下拨动为高电平
typedef struct{
    GPIO_TypeDef *GPIOX;
    uint16_t GPIO_Pin;
    uint8_t value;//值
}sw_t;

sw_t SW[4] = {
    {GPIOA, GPIO_PIN_11, 0},
    {GPIOA, GPIO_PIN_12, 0},
    {GPIOA, GPIO_PIN_15, 0},
    {GPIOB, GPIO_PIN_3, 0},
};
void sw_get(void)
{
    for(uint8_t i = 0; i < 4; i++)
    {
        //低电平设为1，高电平设为0
        HAL_GPIO_ReadPin(SW[i].GPIOX, SW[i].GPIO_Pin) == RESET ? (SW[i].value = 1) : (SW[i].value = 0);
        remote_inf.sw[i] = SW[i].value;
    }
}

/***************************************** SW END ******************************************/

void remote_init(void)
{

}

void remote_task(void)
{
    char tx3_buffer[50] = {0};
    char tx1_buffer[100] = {0};
    while (1)
    {
        adc_get();
        key_get();
        sw_get();
        // sprintf(tx3_buffer,"ADC0: %d, ADC1: %d, ADC2: %d, ADC3: %d", remote_inf.adc_buffer[0], remote_inf.adc_buffer[1], remote_inf.adc_buffer[2], remote_inf.adc_buffer[3]);
        // HAL_UART_Transmit_DMA(&huart3, (uint8_t *)tx3_buffer, strlen(tx3_buffer));
        // memset(tx3_buffer, 0, sizeof(tx3_buffer));
        // // for(uint8_t i = 0; i < KEY_NUM; i++)
        // // {
        // //     sprintf(tx3_buffer," KEY%d: %d ", i, remote_inf.key[i]);
        // //     HAL_UART_Transmit_DMA(&huart3, (uint8_t *)tx3_buffer, strlen(tx3_buffer));
        // //     memset(tx3_buffer, 0, sizeof(tx3_buffer));
        // // }
        // for(uint8_t i = 0; i < 4; i++)
        // {
        //    sprintf(tx3_buffer," SW%d: %d ", i, remote_inf.sw[i]);
        //    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)tx3_buffer, strlen(tx3_buffer));
        //    memset(tx3_buffer, 0, sizeof(tx3_buffer));
        // }
        // HAL_UART_Transmit_DMA(&huart3, (uint8_t *)"\r\n", 2);
        
        vTaskDelay(10);
    }
}

