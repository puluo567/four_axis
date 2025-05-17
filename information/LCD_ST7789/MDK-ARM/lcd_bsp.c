#include "lcd_bsp.h"
#include "lcd_font.h"

uint16_t BACK_COLOR = WHITE;
uint16_t POINT_COLOR = BLACK;   //背景色，画笔色

void ReadWrite(uint8_t ByteSend)
{
    HAL_SPI_Transmit(&hspi1, &ByteSend, 1, 1000);
}

static void ST7789_WR_DATA8(uint16_t da){ //发送数据-8位参数
	#if ENABLE_CS
		OLED_CS(0);
	#endif
	#if Fast_Mode
		GPIOB->BSRR = DC_Pin(10);//为了极大的加快速度，这里我直接使用寄存器操作,这条代码指PB10(1)
	#else
		OLED_DC(1);
	#endif
		ReadWrite(da);
	#if ENABLE_CS
		OLED_CS(1)
	#endif
}
static void ST7789_WR_DATA(int da){
	#if ENABLE_CS
		OLED_CS(0);
	#endif
    #if Fast_Mode
		GPIOB->BSRR = DC_Pin(10);//为了极大的加快速度，这里我直接使用寄存器操作,这条代码指PB10(1)
	#else
		OLED_DC(1);
	#endif
	ReadWrite(da>>8);
	ReadWrite(da);
	#if ENABLE_CS
		OLED_CS(1);
	#endif
}
static void ST7789_WR_REG(uint16_t da)	 {
	#if ENABLE_CS
		OLED_CS(0);
	#endif
    #if Fast_Mode
		GPIOB->BRR = DC_Pin(10);//为了极大的加快速度，这里我直接使用寄存器操作,这条代码指PB10(0)
	#else
		OLED_DC(0);
	#endif
	ReadWrite(da);
	#if ENABLE_CS
		OLED_CS(1);
	#endif
}

void ST7789_Cursor(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2){
   ST7789_WR_REG(0x2a);
   ST7789_WR_DATA(x1);
   ST7789_WR_DATA(x2);
  
   ST7789_WR_REG(0x2b);
   ST7789_WR_DATA(y1);
   ST7789_WR_DATA(y2);

   ST7789_WR_REG(0x2C);					 						 
}

void ST7789_Init(uint16_t Back_color,uint16_t Pen_color)
{
	BACK_COLOR = Back_color;
	POINT_COLOR = Pen_color;
	#if ENABLE_CS
		OLED_CS(0);//片选使能
	#endif
	OLED_RST(0);
	for(int i =0;i<2000;i++)__NOP();//延时
	OLED_RST(1);
	for(int i =0;i<2000;i++)__NOP();//延时

	ST7789_WR_REG(0x36); 
	ST7789_WR_DATA8(0x00);

	ST7789_WR_REG(0x3A); 
	ST7789_WR_DATA8(0x05);

	ST7789_WR_REG(0xB2);
	ST7789_WR_DATA8(0x0C);
	ST7789_WR_DATA8(0x0C);
	ST7789_WR_DATA8(0x00);
	ST7789_WR_DATA8(0x33);
	ST7789_WR_DATA8(0x33);

	ST7789_WR_REG(0xB7); 
	ST7789_WR_DATA8(0x35);  

	ST7789_WR_REG(0xBB);
	ST7789_WR_DATA8(0x19);

	ST7789_WR_REG(0xC0);
	ST7789_WR_DATA8(0x2C);

	ST7789_WR_REG(0xC2);
	ST7789_WR_DATA8(0x01);

	ST7789_WR_REG(0xC3);
	ST7789_WR_DATA8(0x12);   

	ST7789_WR_REG(0xC4);
	ST7789_WR_DATA8(0x20);  

	ST7789_WR_REG(0xC6);//刷新率
	ST7789_WR_DATA8(0x0F);    

	ST7789_WR_REG(0xD0); 
	ST7789_WR_DATA8(0xA4);
	ST7789_WR_DATA8(0xA1);

	ST7789_WR_REG(0xE0);
	ST7789_WR_DATA8(0xD0);
	ST7789_WR_DATA8(0x04);
	ST7789_WR_DATA8(0x0D);
	ST7789_WR_DATA8(0x11);
	ST7789_WR_DATA8(0x13);
	ST7789_WR_DATA8(0x2B);
	ST7789_WR_DATA8(0x3F);
	ST7789_WR_DATA8(0x54);
	ST7789_WR_DATA8(0x4C);
	ST7789_WR_DATA8(0x18);
	ST7789_WR_DATA8(0x0D);
	ST7789_WR_DATA8(0x0B);
	ST7789_WR_DATA8(0x1F);
	ST7789_WR_DATA8(0x23);

	ST7789_WR_REG(0xE1);
	ST7789_WR_DATA8(0xD0);
	ST7789_WR_DATA8(0x04);
	ST7789_WR_DATA8(0x0C);
	ST7789_WR_DATA8(0x11);
	ST7789_WR_DATA8(0x13);
	ST7789_WR_DATA8(0x2C);
	ST7789_WR_DATA8(0x3F);
	ST7789_WR_DATA8(0x44);
	ST7789_WR_DATA8(0x51);
	ST7789_WR_DATA8(0x2F);
	ST7789_WR_DATA8(0x1F);
	ST7789_WR_DATA8(0x1F);
	ST7789_WR_DATA8(0x20);
	ST7789_WR_DATA8(0x23);

	ST7789_WR_REG(0x21); 

	ST7789_WR_REG(0x11); 
	for(int i =0;i<5000;i++)__NOP();//延时

	ST7789_WR_REG(0x29); 
}

//窗口旋转函数
void ST7789_SetRotation(uint8_t Diraction){
	ST7789_WR_REG(0x36);	// MADCTL
	switch (Diraction) {
	case 0:ST7789_WR_DATA8(0x00);break;//0000 0000
	case 1:ST7789_WR_DATA8(0xA0);break;//1010 0000
	case 2:ST7789_WR_DATA8(0xC0);break;//1100 0000
	case 3:ST7789_WR_DATA8(0x60);break;//0110 0000 
	default://这里有问题，屏幕是发生了旋转，但是坐标出现了问题，应该需要旋转后重新设置坐标，存在问题(待修改...........)
		break;
	}
}

void ST7789_Clear(uint16_t Color){
	uint16_t i,j;  	
	ST7789_Cursor(0,0,ST7789_W-1,ST7789_H-1);
    for(i=0;i<ST7789_W;i++){
	  for (j=0;j<ST7789_H;j++){
        	ST7789_WR_DATA(Color);	 			 
	    }
	}
}

void ST7789_DrawPoint(uint16_t x,uint16_t y){
	ST7789_Cursor(x,y,x,y);//设置光标位置 
	ST7789_WR_DATA(POINT_COLOR);
}

// void ST7789_DrawPoint_big(uint16_t x,uint16_t y)
// {
// 	ST7789_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
// } 

// void ST7789_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color){
// 	uint16_t i;
// 	ST7789_Cursor(xsta,ysta,xend,yend);      //设置光标位置 
// 	/*
// 	for(i=ysta;i<=yend;i++){								   	 	
// 		for(j=xsta;j<=xend;j++)
// 		ST7789_WR_DATA(color);    
// 	} 
// 	*/				  	
// 	for (i=0;i<480;i+=2){//ST7789一行要240*2个字节,one line in ST7789 needs one 480 byte
// 		SendBuff[i] = color>>8;
// 		SendBuff[i+1] = color;
// 	}
// 	for(i = 0;i<240;i++){
// 		if(ST7789_HSPI == SPI1)
// 			DMA_Transfer(DMA1_Channel3,480);
// 		else
// 			DMA_Transfer(DMA1_Channel5,480);
// 	}
// }  

void ST7789_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ ){//画线输出
		ST7789_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

void ST7789_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	ST7789_DrawLine(x1,y1,x2,y1);
	ST7789_DrawLine(x1,y1,x1,y2);
	ST7789_DrawLine(x1,y2,x2,y2);
	ST7789_DrawLine(x2,y1,x2,y2);
}

void ST7789_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b){
		ST7789_DrawPoint(x0-b,y0-a);             //3           
		ST7789_DrawPoint(x0+b,y0-a);             //0           
		ST7789_DrawPoint(x0-a,y0+b);             //1       
		ST7789_DrawPoint(x0-b,y0-a);             //7           
		ST7789_DrawPoint(x0-a,y0-b);             //2             
		ST7789_DrawPoint(x0+b,y0+a);             //4               
		ST7789_DrawPoint(x0+a,y0-b);             //5
		ST7789_DrawPoint(x0+a,y0+b);             //6 
		ST7789_DrawPoint(x0-b,y0+a);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		ST7789_DrawPoint(x0+a,y0+b);
	}
} 

//mode:叠加方式(1)还是非叠加方式(0)
void ST7789_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
{
    uint8_t temp;
    uint8_t pos,t;
	uint16_t x0=x;
	uint16_t colortemp=POINT_COLOR;      
    if(x>ST7789_W-16||y>ST7789_H-16)return;	    
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	ST7789_Cursor(x,y,x+8-1,y+16-1);      //设置光标位置 
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(uint16_t)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++){
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				ST7789_WR_DATA(POINT_COLOR);
				temp>>=1;
				x++;
		    }
			x=x0;
			y++;
		}	
	}else{//叠加方式
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(uint16_t)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++){        
		        if(temp&0x01)ST7789_DrawPoint(x+t,y+pos);//画一个点     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   

uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}

void ST7789_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len){ 
	uint8_t t,temp;
	uint8_t enshow=0;
	num=(uint16_t)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1)){
			if(temp==0){
				ST7789_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1;  	 
		}
	 	ST7789_ShowChar(x+8*t,y,temp+48,0); 
	}
} 

void ST7789_ShowString(uint16_t x,uint16_t y,char *p)
{         
    while(*p!='\0'){      
        if(x>ST7789_W-16){x=0;y+=16;}
        if(y>ST7789_H-16){y=x=0;ST7789_Clear(RED);}
        ST7789_ShowChar(x,y,*p,0);
        x+=8;
        p++;
    }  
}

//void ST7789_Printf(uint16_t X, uint16_t Y,const char* format, ...){
//	char String[256];
//	va_list arg;
//	va_start(arg, format);
//	vsprintf(String, format, arg);
//	va_end(arg);
//	ST7789_ShowString(X, Y, String);
//}
