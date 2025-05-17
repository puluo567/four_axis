#ifndef __LCD_BSP_H_
#define __LCD_BSP_H_

#include "stdio.h"
#include "gpio.h"
#include "spi.h"

#define ST7789_W 240
#define ST7789_H 320

#define RES_GPIO            GPIOB
#define RES_GPIO_PIN        GPIO_PIN_12
#define DC_GPIO             GPIOB
#define DC_GPIO_PIN         GPIO_PIN_13
#define CS_GPIO             GPIOB
#define CS_GPIO_PIN         GPIO_PIN_14
#define BLK_GPIO            GPIOB
#define BLK_GPIO_PIN        GPIO_PIN_15

//BLK可以不接
//CS可以不接,大部分ST7789默认为0,即默认为选中状态

#define ENABLE_CS 0          //如果你有片选端，请勾选这里
#define Fast_Mode 0//1:使用寄存器操作,0:使用库函数操作

#define OLED_RST(x) HAL_GPIO_WritePin(RES_GPIO, RES_GPIO_PIN, (GPIO_PinState)x) //RST
#define OLED_CS(x)  HAL_GPIO_WritePin(CS_GPIO, CS_GPIO_PIN, (GPIO_PinState)x) //CS
#define OLED_DC(x)  HAL_GPIO_WritePin(DC_GPIO, DC_GPIO_PIN, (GPIO_PinState)x) //DC

#define DC_Pin(x) 0x01<<x//寄存器操作，提升速度

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 
#define BRRED 			 0XFC07 
#define GRAY  			 0X8430 

extern uint16_t BACK_COLOR, POINT_COLOR;//背景色，画笔色
//默认Deault: White Black
////////////////////////////////User////////////////////////////////////////////
void ST7789_Init(uint16_t Back_color,uint16_t Pen_color);
void ST7789_SetRotation(uint8_t Diraction);                         //bug
void ST7789_Clear(uint16_t Color);
void ST7789_Clear_DMA(uint16_t Color);
void ST7789_Cursor(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void ST7789_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void ST7789_Printf(uint16_t X, uint16_t Y,const char* format, ...);
void ST7789_SlowPrint(uint16_t x,uint16_t y,const char* string);
void ST7789_PrintChinese(unsigned int x,unsigned int y,unsigned char index);
void ST7789_ShowImage_Flash(uint16_t startX, uint16_t startY,const uint8_t *Picture_ptr);
void ST7789_ShowImage_DMA(uint16_t startX, uint16_t startY,const uint8_t *Picture_ptr,uint32_t sizof_pic);

//Add 增添的不常用基础操作
void ST7789_DrawPoint(uint16_t x,uint16_t y);//画点
void ST7789_DrawPoint_big(uint16_t x,uint16_t y);//画一个大点
uint16_t  ST7789_ReadPoint(uint16_t x,uint16_t y);//读点
void ST7789_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);//画圆
void ST7789_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);//画线
void ST7789_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);//画矩形
void ST7789_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode);//char
void ST7789_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len);//Num
void ST7789_ShowString(uint16_t x,uint16_t y,char *p);//String

#endif
