#ifndef __LCD_H__
#define __LCD_H__		
#include "main.h"
#include "sys.h"

/////////////////////////////////////�û�������///////////////////////////////////	 
//����2���궨�壬������Ļ����ʾ��ʽ��IO�ٶ�
#define USE_HORIZONTAL  0	//�����Ƿ�ʹ�ú��� 		0,��ʹ��.1,ʹ��.
#define LCD_FAST_IO     1 	//�����Ƿ�ʹ�ÿ���IO	0,��ʵ��.1,ʹ��
//////////////////////////////////////////////////////////////////////////////////	 


 
//TFTLCD������Ҫ���õĺ���		   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
extern u16  POINT_COLOR_EX;
extern u16  BACK_COLOR_EX;
//����LCD�ĳߴ�
#if USE_HORIZONTAL==1	//ʹ�ú���
#define LCD_W 320
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 320
#endif


//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	 wramcmd;		//��ʼдgramָ��
	u16  setxcmd;		//����x����ָ��
	u16  setycmd;		//����y����ָ��	 
}_lcd_dev; 	

////////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���---------------- 
//���ʹ�ÿ���IO�������¾䣬�����ʹ�ã���ȥ�����ɣ�
//ʹ�ÿ���IO��ˢ�����ʿ��Դﵽ28֡ÿ�룡
//��ͨIO��ֻ��14֡ÿ�룡   

//-----------------LCD�˿ڶ���---------------- 
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

//PC0~15,��Ϊ������
#define DATAOUT(x) GPIOD->ODR=x; //�������
#define DATAIN     GPIOD->IDR;   //��������

#define	DATA_INPUT   {GPIOD->CRL=0X88888888; GPIOD->CRH=0X88888888;GPIOD->ODR=0XFFFF;}    
#define	DATA_OUTPUT  {GPIOD->CRL=0X33333333; GPIOD->CRH=0X33333333;GPIOD->ODR=0XFFFF;}

//////////////////////////////////////////////////////////////////////
//������ɫ
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
#define BROWN 			 		0XBC40 //��ɫ
#define BRRED 			 		0XFC07 //�غ�ɫ
#define GRAY  			 		0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

#define CUSTOM_BG      GREEN

	    															  
extern _lcd_dev lcddev;	//����LCD��Ҫ����  


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


//д8λ���ݺ���
//�ú궨��,����ٶ�.
#if LCD_FAST_IO //����IO

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


