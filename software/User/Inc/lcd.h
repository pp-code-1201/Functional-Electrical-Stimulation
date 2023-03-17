#ifndef __LCD_H__
#define __LCD_H__		
#include "main.h"
#include "sys.h"

/////////////////////////////////////用户配置区///////////////////////////////////	 
//以下2个宏定义，定义屏幕的显示方式及IO速度
#define USE_HORIZONTAL  0	//定义是否使用横屏 		0,不使用.1,使用.
#define LCD_FAST_IO     1 	//定义是否使用快速IO	0,不实用.1,使用
//////////////////////////////////////////////////////////////////////////////////	 


 
//TFTLCD部分外要调用的函数		   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色
extern u16  POINT_COLOR_EX;
extern u16  BACK_COLOR_EX;
//定义LCD的尺寸
#if USE_HORIZONTAL==1	//使用横屏
#define LCD_W 320
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 320
#endif


//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	 wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	

////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 
//如果使用快速IO，则定义下句，如果不使用，则去掉即可！
//使用快速IO，刷屏速率可以达到28帧每秒！
//普通IO，只能14帧每秒！   

//-----------------LCD端口定义---------------- 
#define	LCD_LED_SET   HAL_GPIO_WritePin(LCD_BL_GPIO_Port,LCD_BL_Pin,GPIO_PIN_SET)
#define	LCD_LED_CLR   HAL_GPIO_WritePin(LCD_BL_GPIO_Port,LCD_BL_Pin,GPIO_PIN_RESET)

#define	LCD_CS_SET	  HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_SET)
#define	LCD_CS_CLR    HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET)

#define	LCD_RS_SET		HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_SET)		
#define	LCD_RS_CLR		HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_RESET)

#define	LCD_WR_SET		HAL_GPIO_WritePin(LCD_WR_GPIO_Port,LCD_WR_Pin,GPIO_PIN_SET)
#define	LCD_WR_CLR		HAL_GPIO_WritePin(LCD_WR_GPIO_Port,LCD_WR_Pin,GPIO_PIN_RESET)

#define	LCD_RD_SET		HAL_GPIO_WritePin(LCD_RD_GPIO_Port,LCD_RD_Pin,GPIO_PIN_SET)
#define	LCD_RD_CLR		HAL_GPIO_WritePin(LCD_RD_GPIO_Port,LCD_RD_Pin,GPIO_PIN_RESET)

#define LCD_RST_SET		HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET)
#define LCD_RST_CLR		HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET)

//PC0~15,作为数据线
#define DATAOUT(x) GPIOD->ODR=x; //数据输出
#define DATAIN     GPIOD->IDR;   //数据输入

#define	DATA_INPUT   {GPIOD->CRL=0X88888888; GPIOD->CRH=0X88888888;GPIOD->ODR=0XFFFF;}    
#define	DATA_OUTPUT  {GPIOD->CRL=0X33333333; GPIOD->CRH=0X33333333;GPIOD->ODR=0XFFFF;}

//////////////////////////////////////////////////////////////////////
//画笔颜色
#define WHITE         	0xFFFF
#define BLACK         	0x0000	  
#define BLUE         	 	0x001F  
#define BRED            0XF81F
#define GRED 			 			0XFFE0
#define GBLUE			 			0X867D//0X07FF
#define RED           	0xF800
#define MAGENTA       	0xF81F
#define GREEN         	0x07E0
#define CYAN          	0x7FFF
#define YELLOW        	0xFFE0
#define BROWN 			 		0XBC40 //棕色
#define BRRED 			 		0XFC07 //棕红色
#define GRAY  			 		0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#define CUSTOM_BG      GREEN

	    															  
extern _lcd_dev lcddev;	//管理LCD重要参数  


void LCD_Init(void);
void LCD_Clear(u16 Color);	 
void LCD_SetCursor(u16 Xpos, u16 Ypos);						    
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_DrawPoint_16Bit(u16 color);
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
void LCD_DrawPoint(u16 x,u16 y);
void LCD_Clear_Fill(u16 Color,u16 x1,u16 y1,u16 x2,u16 y2);


//写8位数据函数
//用宏定义,提高速度.
#if LCD_FAST_IO //快速IO

#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} 	

#endif	

#if 0
#define LCD_WR_TDATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
DATAOUT(data & 0xFF00);\
LCD_WR_CLR;\
LCD_WR_SET;\
DATAOUT(data << 8);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
}		

#else
#define LCD_WR_TDATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
DATAOUT(data & 0xFF00);\
LCD_WR_CLR;\
LCD_WR_SET;\
DATAOUT(data << 8);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
}		
#endif


#endif


