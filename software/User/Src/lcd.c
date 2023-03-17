#include "lcd.h"
#include "stdlib.h"
#include "gpio.h" 
#include "usart.h"
#include "sys.h"
#include "gui.h"

					 
//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000, BACK_COLOR = 0xFFFF;
u16 POINT_COLOR_EX = 0x10;//(海军蓝) 
u16 BACK_COLOR_EX = GREEN;  
u16 DeviceCode;	 


//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;



#if LCD_FAST_IO //快速IO
//写寄存器函数
void LCD_RESET(void)
{ 
	LCD_RST_SET;//高电平  
 	HAL_Delay(1);
	LCD_RST_CLR;//低电平
	HAL_Delay(20);
	LCD_RST_SET;//高电平  
	HAL_Delay(20);
}
#endif


#if LCD_FAST_IO //快速IO
//写寄存器函数
void LCD_WR_REG(u8 data)
{ 
	LCD_RS_CLR;//写地址  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}
#endif	

//写寄存器
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	


//读寄存器
u16 LCD_ReadReg(u8 LCD_Reg)
{										   
	u16 t;
	//LCD_WR_REG(LCD_Reg);  //写入要读的寄存器号  
	//GPIOD->CRL=0X88888888; //PC0-7  上拉输入
	//GPIOD->CRH=0X88888888; //PC8-15 上拉输入
	//GPIOD->ODR=0XFFFF;    //全部输出高
	//LCD_RS_SET;
	//LCD_CS_CLR;
	//读取数据(读寄存器时,并不需要读2次)
	//LCD_RD_CLR;					   
	LCD_RD_SET;
	//t=DATAIN;  
	//LCD_CS_SET;   
	//GPIOD->CRL=0X33333333; //PC0-7  上拉输出
	//GPIOD->CRH=0X33333333; //PC8-15 上拉输出
	//GPIOD->ODR=0XFFFF;    //全部输出高
	return t; 
}   


//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 



//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(0x00FF&Xpos);		

	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(0x00FF&Ypos);		

	LCD_WriteRAM_Prepare();	//开始写入GRAM	
}  


//设置LCD参数
void LCD_SetParam(void)
{ 	
#if	defined(_BC_FES_)
    lcddev.wramcmd=0x2C;
		lcddev.dir=1;//横屏				 	 		
		lcddev.width=320;
		lcddev.height=240;
		lcddev.setxcmd=0x2A;
		lcddev.setycmd=0x2B;	
#else
		lcddev.wramcmd=0x2C;
		lcddev.dir=0;//竖屏				 	 		
		lcddev.width=240;
		lcddev.height=320;
		lcddev.setxcmd=0x2A;
		lcddev.setycmd=0x2B;	
#endif
}	


//画点
//x:0~239
//y:0~319
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	LCD_WR_REG(0x2c);//开始写入GRAM
	//LCD_WR_TDATA(POINT_COLOR); 
	LCD_WR_DATA(POINT_COLOR);
} 

	 
//初始化lcd
void LCD_Init(void)
{ 
	LCD_RESET();//复位
	  					 
	HAL_Delay(50);               // delay 50 ms 
 	//LCD_WriteReg(0x0000,0x0001);
	HAL_Delay(50);               // delay 50 ms 
  //lcddev.id = LCD_ReadReg(0x0000);   
  LCD_RD_SET;

#if 0	
	  LCD_WR_REG(0xCB);  
		LCD_WR_DATA(0x39); 
		LCD_WR_DATA(0x2C); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x34); 
		LCD_WR_DATA(0x02); 
		
		LCD_WR_REG(0xF7);  
		LCD_WR_DATA(0x20); 
		
		LCD_WR_REG(0xEA);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		
		LCD_WR_REG(0xC0); 	 //Power control 
		LCD_WR_DATA(0x1B);	 //VRH[5:0] 
		
		LCD_WR_REG(0xC1); 	 //Power control 
		LCD_WR_DATA(0x01);	 //SAP[2:0];BT[3:0] 
		
		LCD_WR_REG(0xC5); 	 //VCM control 
		LCD_WR_DATA(0x30);	 //3F
		LCD_WR_DATA(0x30);	 //3C
		
		LCD_WR_REG(0xC7); 	 //VCM control2 
		LCD_WR_DATA(0XB7); 
		
		LCD_WR_REG(0x36); 	 // Memory Access Control 显示方向设置
#if defined(_BC_FES_)
    LCD_WR_DATA(0x00);
#else
		LCD_WR_DATA(0x60);   //0xA0 
#endif
		
		LCD_WR_REG(0x3A); 	
		LCD_WR_DATA(0x05); //RGB 565
		
		LCD_WR_REG(0xB1); 	
		LCD_WR_DATA(0x00);	 
		LCD_WR_DATA(0x1A); 
		
		LCD_WR_REG(0xB6); 	 // Display Function Control 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0xA2); 
		
		LCD_WR_REG(0xF2); 	 // 3Gamma Function Disable 
		LCD_WR_DATA(0x00); 
		
		LCD_WR_REG(0x26); 	 //Gamma curve selected 
		LCD_WR_DATA(0x01); 
		
		LCD_WR_REG(0xE0); 	 //Set Gamma 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x2A); 
		LCD_WR_DATA(0x28); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x0E); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x54); 
		LCD_WR_DATA(0XA9); 
		LCD_WR_DATA(0x43); 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00);	
		
		LCD_WR_REG(0XE1); 	 //Set Gamma 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x15); 
		LCD_WR_DATA(0x17); 
		LCD_WR_DATA(0x07); 
		LCD_WR_DATA(0x11); 
		LCD_WR_DATA(0x06); 
		LCD_WR_DATA(0x2B); 
		LCD_WR_DATA(0x56); 
		LCD_WR_DATA(0x3C); 
		LCD_WR_DATA(0x05); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x0F); 
		
		LCD_WR_REG(0x2B); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x3f);
		
		LCD_WR_REG(0x2A); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xef);	
	
		LCD_WR_REG(0x21); //颜色反向
		LCD_WR_REG(0x2C); 
		
		LCD_WR_REG(0x11); //Exit Sleep
		HAL_Delay(120);
		
		LCD_WR_REG(0x29); //display on	
#else
	  LCD_WR_REG(0xCF);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x81); 
		LCD_WR_DATA(0X30); 
	 
		LCD_WR_REG(0xED);  
		LCD_WR_DATA(0x64); 
		LCD_WR_DATA(0x03); 
		LCD_WR_DATA(0X12); 
		LCD_WR_DATA(0X81); 
	 
		LCD_WR_REG(0xE8);  
		LCD_WR_DATA(0x85); 
		LCD_WR_DATA(0x11); 
		LCD_WR_DATA(0x78); 
	
	
		LCD_WR_REG(0xCB);  
		LCD_WR_DATA(0x39); 
		LCD_WR_DATA(0x2C); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x34); 
		LCD_WR_DATA(0x02); 
	 
		LCD_WR_REG(0xF7);  
		LCD_WR_DATA(0x20); 
	 
		LCD_WR_REG(0xEA);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
	 
	
		LCD_WR_REG(0xb6);  
		LCD_WR_DATA(0x0a); 
		LCD_WR_DATA(0xa2); 
	
		LCD_WR_REG(0xC0); 			//Power control 
		LCD_WR_DATA(0x21); 		//VRH[5:0] 
	 
		LCD_WR_REG(0xC1); 			//Power control 
		LCD_WR_DATA(0x11); 		//SAP[2:0];BT[3:0] 
	 
		LCD_WR_REG(0xC5); 			//VCM control 
		LCD_WR_DATA(0x33); 
		LCD_WR_DATA(0x3C); 
	 
		LCD_WR_REG(0xC7); 			//VCM control2 
		LCD_WR_DATA(0xA0); 
	 
		LCD_WR_REG(0x36); 			// Memory Access Control 设置横屏竖屏
#if defined(_BC_FES_)
		LCD_WR_DATA(0x60); //横屏
#else
    LCD_WR_DATA(0x00); //竖屏
#endif
	
		LCD_WR_REG(0x3A); 			
		LCD_WR_DATA(0x55); 
	
		LCD_WR_REG(0xB1); 			//VCM control 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x18); 
	
	 
		LCD_WR_REG(0xF2); 			// 3Gamma Function Disable 
		LCD_WR_DATA(0x00); 
	 
		LCD_WR_REG(0x26); 			//Gamma curve selected 
		LCD_WR_DATA(0x01); 
	 
		LCD_WR_REG(0xE0); 			//Set Gamma 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x1a); 
		LCD_WR_DATA(0x18); 
		LCD_WR_DATA(0x0c); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x45); 
		LCD_WR_DATA(0xba); 
		LCD_WR_DATA(0x30); 
		LCD_WR_DATA(0x02); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
	 
		LCD_WR_REG(0XE1); 			//Set Gamma 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x24); 
		LCD_WR_DATA(0x27); 
		LCD_WR_DATA(0x03); 
		LCD_WR_DATA(0x0e); 
		LCD_WR_DATA(0x06); 
		LCD_WR_DATA(0x3a); 
		LCD_WR_DATA(0x45); 
		LCD_WR_DATA(0x4f); 
		LCD_WR_DATA(0x0d); 
		LCD_WR_DATA(0x1f); 
		LCD_WR_DATA(0x0f); 
		LCD_WR_DATA(0x3f); 
		LCD_WR_DATA(0x3f); 
		LCD_WR_DATA(0x0F); 

		//LCD_WR_REG(0x2C);
	
	//	delay(50);
		LCD_WR_REG(0x11); 			//Exit Sleep 
		HAL_Delay(20); 
		LCD_WR_REG(0x29); 			//Display on
	//	LCD_WR_REG(0x21);//颜色反向

#endif
  
	LCD_SetParam();//设置LCD参数	 
	
	GUI_Display_Logo_Picture();
	HAL_Delay(1);
	//LCD_LED_SET;//高电平打开液晶屏背光
	HAL_Delay(500);
}  		  



/*************************************************
函数名：LCD_SetWindows
功能：设置lcd显示窗口，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);	

	LCD_WriteRAM_Prepare();	//开始写入GRAM				
}   

//******************************************************************
//函数名：  LCD_Clear
//功能：    LCD全屏填充清屏函数
//输入参数：Color:要清屏的填充色
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Clear(u16 Color)
{
		u32 index=0;      
		LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	
		LCD_CS_CLR;	
		LCD_RS_SET;//写数据    
		for(index=0;index<lcddev.width*lcddev.height;index++)
		{
				//LCD_WR_DATA(Color>>8);
				//LCD_WR_DATA(Color);
				//LCD_WR_TDATA(Color);
				LCD_WR_DATA(Color);
		}
		LCD_CS_SET;			
}   


void LCD_Clear_Fill(u16 Color,u16 x1,u16 y1,u16 x2,u16 y2)
{
		u32 index=0;      
		LCD_SetWindows(x1,y1,x2,y2);	
		LCD_CS_CLR;	
		LCD_RS_SET;//写数据    
		for(index=0;index<((x2-x1)*(y2-y1));index++)
		{
				//LCD_WR_DATA(Color>>8);
				//LCD_WR_DATA(Color);
				//LCD_WR_TDATA(Color);
				LCD_WR_DATA(Color);
		}
		LCD_CS_SET;			
}   



//******************************************************************
//函数名：  LCD_DrawPoint_16Bit
//功能：    8位总线下如何写入一个16位数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_DrawPoint_16Bit(u16 color)
{
    //LCD_WR_TDATA(color);
    LCD_WR_DATA(color);
}





