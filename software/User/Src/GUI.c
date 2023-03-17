#include <stdio.h>
#include "lcd.h"
#include "sys.h"
#include "string.h"
#include "font.h" 
#include "gui.h"
#include "gpio.h"
#include "main.h"
#include "picture.h"
#include "app.h"
#include "adc.h"

#if 1//defined(_BC_FES_)
#include "image_fes.h"
#else
//#include "image.h"
#endif



#if 1//defined(_BC_FES_)
GUI_PARA gui_fes_struct_para;
//#else
GUI_PARA gui_struct_para;
#endif


//////////////////////////////////////////////////////////////////////////////////	 
//******************************************************************
//函数名：  GUI_DrawPoint
//功能：    GUI描绘一个点
//输入参数：x:光标位置x坐标
//        	y:光标位置y坐标
//			color:要填充的颜色
//返回值：  无
//修改记录：无
//******************************************************************
void GUI_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_SetCursor(x,y);//设置光标位置 
	LCD_DrawPoint_16Bit(color); 
}

//******************************************************************
//函数名：  LCD_Fill
//功能：    在指定区域内填充颜色
//输入参数：sx:指定区域开始点x坐标
//        	sy:指定区域开始点y坐标
//			ex:指定区域结束点x坐标
//			ey:指定区域结束点y坐标
//        	color:要填充的颜色
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{  	

	u16 i,j;			
	u16 width=ex-sx+1; 		//得到填充的宽度
	u16 height=ey-sy+1;		//高度
	LCD_SetWindows(sx,sy,ex-1,ey-1);//设置显示窗口

	LCD_CS_CLR;	
	LCD_RS_SET;//写数据    
	for(i=0;i<width;i++)
	{
		for(j=0;j<height;j++)
		{
				LCD_WR_DATA(color>>8);
				LCD_WR_DATA(color);  
		}
	}
	LCD_CS_SET;	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口设置为全屏
}

//******************************************************************
//函数名：  LCD_DrawLine
//功能：    GUI画线
//输入参数：x1,y1:起点坐标
//        	x2,y2:终点坐标 
//返回值：  无
//修改记录：无
//****************************************************************** 
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
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
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
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

//******************************************************************
//函数名：  LCD_DrawRectangle
//功能：    GUI画矩形(非填充)
//输入参数：(x1,y1),(x2,y2):矩形的对角坐标
//返回值：  无
//修改记录：无
//******************************************************************  
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
#if 0
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
#else
	Gui_DrawLine(x1,  y1,  x2,y1, GRAY2);  //H
	Gui_DrawLine(x1+1,y1+1,x2,y1+1, GRAY1);  //H
	Gui_DrawLine(x1,  y1,  x1,y2, GRAY2);  //V
	Gui_DrawLine(x1+1,y1+1,x1+1,y2, GRAY1);  //V
	Gui_DrawLine(x1,  y2,  x2,y2, GRAY2);  //H
	Gui_DrawLine(x2,  y1,  x2,y2, GRAY1);  //V
#endif
}  

//******************************************************************
//函数名：  LCD_DrawFillRectangle
//功能：    GUI画矩形(填充)
//输入参数：(x1,y1),(x2,y2):矩形的对角坐标
//返回值：  无
//修改记录：无
//******************************************************************   
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_Fill(x1,y1,x2,y2,POINT_COLOR);
	//LCD_Fill(x1,y1,x2,y2,GBLUE);

}
 
//******************************************************************
//函数名：  _draw_circle_8
//功能：    8对称性画圆算法(内部调用)
//输入参数：(xc,yc) :圆中心坐标
// 			(x,y):光标相对于圆心的坐标
//         	c:填充的颜色
//返回值：  无
//修改记录：无
//******************************************************************  
void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
{
	GUI_DrawPoint(xc + x, yc + y, c);

	GUI_DrawPoint(xc - x, yc + y, c);

	GUI_DrawPoint(xc + x, yc - y, c);

	GUI_DrawPoint(xc - x, yc - y, c);

	GUI_DrawPoint(xc + y, yc + x, c);

	GUI_DrawPoint(xc - y, yc + x, c);

	GUI_DrawPoint(xc + y, yc - x, c);

	GUI_DrawPoint(xc - y, yc - x, c);
}

//******************************************************************
//函数名：  gui_circle
//功能：    在指定位置画一个指定大小的圆(填充)
//输入参数：(xc,yc) :圆中心坐标
//         	c:填充的颜色
//		 	r:圆半径
//		 	fill:填充判断标志，1-填充，0-不填充
//返回值：  无
//修改记录：无
//******************************************************************  
void gui_circle(int xc, int yc,u16 c,int r, int fill)
{
	int x = 0, y = r, yi, d;

	d = 3 - 2 * r;


	if (fill) 
	{
		// 如果填充（画实心圆）
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				_draw_circle_8(xc, yc, x, yi, c);

			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	} else 
	{
		// 如果不填充（画空心圆）
		while (x <= y) {
			_draw_circle_8(xc, yc, x, y, c);
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}

//******************************************************************
//函数名：  LCD_ShowChar
//功能：    显示单个英文字符
//输入参数：(x,y):字符显示位置起始坐标
//        	fc:前置画笔颜色
//			bc:背景颜色
//			num:数值（0-94）
//			size:字体大小
//			mode:模式  0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************  
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
{  
    u8 temp;
    u8 pos,t;
	u16 colortemp=POINT_COLOR;      
		   
	num=num-' ';//得到偏移后的值
	LCD_SetWindows(x,y,x+size/2-1,y+size-1);//设置单个文字显示窗口
	if(!mode) //非叠加方式
	{
		
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//调用1206字体
			else temp=asc2_1608[num][pos];		 //调用1608字体
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint_16Bit(fc); 
				else LCD_DrawPoint_16Bit(bc); 
				temp>>=1; 
				
		    }
			
		}	
	}else//叠加方式
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//调用1206字体
			else temp=asc2_1608[num][pos];		 //调用1608字体
			for(t=0;t<size/2;t++)
		    {   
				POINT_COLOR=fc;              
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//画一个点    
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏    	   	 	  
} 
//******************************************************************
//函数名：  LCD_ShowString
//功能：    显示英文字符串
//输入参数：x,y :起点坐标	 
//			size:字体大小
//			*p:字符串起始地址
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************  	  
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
{         
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {   
		if(x>(lcddev.width-1)||y>(lcddev.height-1)) 
		return;     
        LCD_ShowChar(x,y,POINT_COLOR,BACK_COLOR,*p,size,mode);
        x+=size/2;
        p++;
    }  
} 


void LCD_ShowString_Ex(u16 x,u16 y,u8 size,u8 *p,u8 mode)
{         
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {   
		if(x>(lcddev.width-1)||y>(lcddev.height-1)) 
		return;     
        LCD_ShowChar(x,y,POINT_COLOR_EX,BACK_COLOR,*p,size,mode);
        x+=size/2;
        p++;
    }  
} 


//******************************************************************
//函数名：  mypow
//功能：    求m的n次方(gui内部调用)
//输入参数：m:乘数
//	        n:幂
//返回值：  m的n次方
//修改记录：无
//******************************************************************  
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

//******************************************************************
//函数名：  LCD_ShowNum
//功能：    显示单个数字变量值
//输入参数：x,y :起点坐标	 
//			len :指定显示数字的位数
//			size:字体大小(12,16)
//			color:颜色
//			num:数值(0~4294967295)
//返回值：  无
//修改记录：无
//******************************************************************  			 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
			  if((10 > num) && (0 < num))
			  {
            LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,'0',size,0);
				}
				else
				{
				    LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,' ',size,0);
				}
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,temp+'0',size,0); 
	}
} 


void LCD_ShowNum_Ex(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
			  if((10 > num) && (0 < num))
			  {
            LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR_EX,BACK_COLOR,'0',size,0);
				}
				else
				{
				    LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR_EX,BACK_COLOR,' ',size,0);
				}
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR_EX,BACK_COLOR,temp+'0',size,0); 
	}
} 


void LCD_ShowNum_Time(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
			  if(10 > num)
			  {
            LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,'0',size,0);
				}
				else
				{
				    LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,' ',size,0);
				}
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,temp+'0',size,0); 
	}
} 


void LCD_ShowNum_Time_Ex(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
			  if(10 > num)
			  {
            LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR_EX,BACK_COLOR,'0',size,0);
				}
				else
				{
				    LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR_EX,BACK_COLOR,' ',size,0);
				}
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR_EX,BACK_COLOR,temp+'0',size,0); 
	}
} 



void LCD_ShowNum_BackColor_Ex(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
			  if((10 > num) && (0 < num))
			  {
            LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR_EX,'0',size,0);
				}
				else
				{
				    LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR_EX,' ',size,0);
				}
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR_EX,temp+'0',size,0); 
	}
} 



//******************************************************************
//函数名：  GUI_DrawFont16
//功能：    显示单个16X16中文字体
//输入参数：x,y :起点坐标
//			fc:前置画笔颜色
//			bc:背景颜色	 
//			s:字符串地址
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
{
	u8 i,j;
	
	u16 k;
	u16 HZnum;
	u16 x0=x;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//自动统计汉字数目
	
			
	for (k=0;k<HZnum;k++) 
	{
	  if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
	  { 	LCD_SetWindows(x,y,x+16-1,y+16-1);
		    for(i=0;i<16*2;i++)
		    {
				for(j=0;j<8;j++)
		    	{	
					if(!mode) //非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x80>>j))	LCD_DrawPoint_16Bit(fc);
						else LCD_DrawPoint_16Bit(bc);
					}
					else
					{
						POINT_COLOR=fc;
						if(tfont16[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//画一个点
						x++;
						if((x-x0)==16)
						{
							x=x0;
							y++;
							break;
						}
					}

				}
				
			}
			
			
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}

	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
} 

//******************************************************************
//函数名：  GUI_DrawFont24
//功能：    显示单个24X24中文字体
//输入参数：x,y :起点坐标
//			fc:前置画笔颜色
//			bc:背景颜色	 
//			s:字符串地址
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;
	u16 x0=x;
	HZnum=sizeof(hz24)/sizeof(typFNT_GB24);	//自动统计汉字数目
		
			for (k=0;k<HZnum;k++) 
			{
			  if ((hz24[k].Index[0]==*(s))&&(hz24[k].Index[1]==*(s+1)))
			  { 	LCD_SetWindows(x,y,x+24-1,y+24-1);
				    for(i=0;i<24*3;i++)
				    {
							for(j=0;j<8;j++)
							{
								if(!mode) //非叠加方式
								{
									if(hz24[k].Msk[i]&(0x80>>j))	LCD_DrawPoint_16Bit(fc);
									else LCD_DrawPoint_16Bit(bc);
								}
							else
							{
								POINT_COLOR=fc;
								if(hz24[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//画一个点
								x++;
								if((x-x0)==24)
								{
									x=x0;
									y++;
									break;
								}
							}
						}
					}
					
					
				}				  	
				continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
			}

	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
}

//******************************************************************
//函数名：  GUI_DrawFont32
//功能：    显示单个32X32中文字体
//输入参数：x,y :起点坐标
//			fc:前置画笔颜色
//			bc:背景颜色	 
//			s:字符串地址
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//****************************************************************** 
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;
	u16 x0=x;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//自动统计汉字数目
	for (k=0;k<HZnum;k++) 
			{
			  if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
			  { 	LCD_SetWindows(x,y,x+32-1,y+32-1);
				    for(i=0;i<32*4;i++)
				    {
						for(j=0;j<8;j++)
				    	{
							if(!mode) //非叠加方式
							{
								if(tfont32[k].Msk[i]&(0x80>>j))	LCD_DrawPoint_16Bit(fc);
								else LCD_DrawPoint_16Bit(bc);
							}
							else
							{
								POINT_COLOR=fc;
								if(tfont32[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//画一个点
								x++;
								if((x-x0)==32)
								{
									x=x0;
									y++;
									break;
								}
							}
						}
					}
					
					
				}				  	
				continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
			}
	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
} 

//******************************************************************
//函数名：  Show_Str
//功能：    显示一个字符串,包含中英文显示
//输入参数：x,y :起点坐标
// 			fc:前置画笔颜色
//			bc:背景颜色
//			str :字符串	 
//			size:字体大小
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************    	   		   
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{					
	u16 x0=x;							  	  
  	u8 bHz=0;     //字符或者中文 
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
			if(x>(lcddev.width-size/2)||y>(lcddev.height-size)) 
			return; 
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {          
		        if(*str==0x0D)//换行符号
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else
				{
					if(size>16)//字库中没有集成12X24 16X32的英文字体,用8X16代替
					{  
					LCD_ShowChar(x,y,fc,bc,*str,16,mode);
					x+=8; //字符,为全字的一半 
					}
					else
					{
					LCD_ShowChar(x,y,fc,bc,*str,size,mode);
					x+=size/2; //字符,为全字的一半 
					}
				} 
				str++; 
		        
	        }
        }else//中文 
        {   
			if(x>(lcddev.width-size)||y>(lcddev.height-size)) 
			return;  
            bHz=0;//有汉字库    
			if(size==32)
			GUI_DrawFont32(x,y,fc,bc,str,mode);	 	
			else if(size==24)
			GUI_DrawFont24(x,y,fc,bc,str,mode);	
			else
			GUI_DrawFont16(x,y,fc,bc,str,mode);
				
	        str+=2; 
	        x+=size;//下一个汉字偏移	    
        }						 
    }   
}

//******************************************************************
//函数名：  Gui_StrCenter
//功能：    居中显示一个字符串,包含中英文显示
//输入参数：x,y :起点坐标
// 			fc:前置画笔颜色
//			bc:背景颜色
//			str :字符串	 
//			size:字体大小
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************   
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{
	u16 len=strlen((const char *)str);
	u16 x1=(lcddev.width-len*8)/2;
	Show_Str(x+x1,y,fc,bc,str,size,mode);
} 
 
//******************************************************************
//函数名：  Gui_Drawbmp16
//功能：    显示一副16位BMP图像
//输入参数：x,y :起点坐标
// 			*p :图像数组起始地址
//返回值：  无
//修改记录：无
//******************************************************************  
void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p) //显示40*40 QQ图片
{
  	int i; 
	unsigned char picH,picL; 
	LCD_SetWindows(x,y,x+240-1,y+320-1);//窗口设置
  for(i=0;i<240*320;i++)
	{	
	 	picL=*(p+i*2);	//数据低位在前
		picH=*(p+i*2+1);				
		LCD_DrawPoint_16Bit(picH<<8|picL);  						
	}	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复显示窗口为全屏	

}


//******************************************************************
//函数名：  Gui_Drawbmp16
//功能：    显示一副16位BMP图像
//输入参数：x,y :起点坐标
// 			*p :图像数组起始地址
//返回值：  无
//修改记录：无
//****************************************************************** 
void Gui_Drawbmp16X(u16 x,u16 y,const unsigned char *p) 
{
  	int i; 
	unsigned char picH,picL; 
	LCD_SetWindows(x,y,x+BATTERY_X-1,y+BATTERY_Y-1);//窗口设置1
  for(i=0;i<BATTERY_X*BATTERY_Y;i++)
	{	
	 	picL=*(p+i*2);	//数据低位在前
		picH=*(p+i*2+1);				
		LCD_DrawPoint_16Bit(picH<<8|picL);  						
	}	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复显示窗口为全屏	

}


/**************************************************************************************
功能: 16bit 画任何坐标和尺寸的图片
输入: 坐标(x,y), 图片大小(x_offset,y_offset),图片数据(*p)
输  出: 无
**************************************************************************************/
void Gui_DrawImage(u16 x,u16 y,u16 x_offset,u16 y_offset,const unsigned char *p) //
{
  	int i; 
		unsigned char picH,picL; 
		LCD_SetWindows(x,y,x+x_offset-1,y+y_offset-1);//窗口设置
	  for(i=0;i<x_offset*y_offset;i++)
		{	
		 	picL=*(p+i*2);	//数据低位在前
			picH=*(p+i*2+1);				
			LCD_DrawPoint_16Bit(picH<<8|picL);  						
		}	
		LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复显示窗口为全屏	

}



//画线函数，使用Bresenham 画线算法
void Gui_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color)   
{
int dx,             // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2, 
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping	


	LCD_SetCursor(x0,y0);
	dx = x1-x0;//计算x距离
	dy = y1-y0;//计算y距离

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
	{//且线的点数等于x距离，以x轴递增画点
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)//要画的点数不会超过x距离
		{
			//画点
			GUI_DrawPoint(x0,y0,Color);
			
			// test if error has overflowed
			if (error >= 0) //是否需要增加y坐标值
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//增加y坐标值
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//x坐标值每次画点后都递增1
		} // end for
	} // end if |slope| <= 1
	else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
	{//以y轴为递增画点
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			GUI_DrawPoint(x0,y0,Color);

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}

/**************************************************************************************
功能: 显示一个凸按钮
输入: u16 x1,y1,x2,y2
输  出: 无
**************************************************************************************/
void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2)
{
#if 0
	Gui_DrawLine(x1,  y1,  x2,y1, GRAY2);  //H
	Gui_DrawLine(x1+1,y1+1,x2,y1+1, GRAY1);  //H
	Gui_DrawLine(x1,  y1,  x1,y2, GRAY2);  //V
	Gui_DrawLine(x1+1,y1+1,x1+1,y2, GRAY1);  //V
	Gui_DrawLine(x1,  y2,  x2,y2, GRAY2);  //H
	Gui_DrawLine(x2,  y1,  x2,y2, GRAY1);  //V
#else
  Gui_DrawLine(x1,  y1,  x2,y1, WHITE);  //H
	Gui_DrawLine(x1+1,y1+1,x2,y1+1, WHITE);  //H
	Gui_DrawLine(x1,  y1,  x1,y2, WHITE);  //V
	Gui_DrawLine(x1+1,y1+1,x1+1,y2, WHITE);  //V
	Gui_DrawLine(x1,  y2,  x2,y2, WHITE);  //H
	Gui_DrawLine(x2,  y1,  x2,y2, WHITE);  //V
#endif
	
}


/**************************************************************************************
功能: 除了电池图标的满屏刷屏
输入: 无
输  出: 无
**************************************************************************************/
void GUI_Display_Clear_No_Icon(void)
{
#if defined(_BC_FES_)
    LCD_Clear_Fill(WHITE,0,14,320,240);
#else
    LCD_Clear_Fill(WHITE,0,14,240,320);
#endif
}


/**************************************************************************************
功能: 强度数字显示
输入: 坐标(x,y)，强度值(strength)
输  出: 无
**************************************************************************************/
void GUI_Display_Strength_Adjust_Val(u16 x,u16 y,u8 strength)
{
    /*if(VNS_STRENGTH_MAX == strength)
    {
				LCD_ShowNum(x,y,strength,3,16);
		}
		else*/
		{
        LCD_ShowNum(x,y,strength,2,16);
		}
}


/**************************************************************************************
功能: 显示倒计时时间
输入: 坐标(x,y)，时间(分：秒)
输  出: 无
**************************************************************************************/
void GUI_Display_Countdown_Time(u16 x,u16 y,u8 min,u8 sec)
{
    LCD_ShowNum_Time(x,y,min,2,16);
		LCD_ShowString(x+20,y,16,(u8 *)":",0);
		LCD_ShowNum_Time(x+30,y,sec,2,16);
}

/**************************************************************************************
功能: 显示倒计时时间EX
输入: 坐标(x,y)，时间(分：秒)
输  出: 无
**************************************************************************************/
void GUI_Display_Countdown_Time_Ex(u16 x,u16 y,u8 min,u8 sec)
{
    LCD_ShowNum_Time_Ex(x,y,min,2,16);
		LCD_ShowString(x+20,y,16,(u8 *)":",0);
		LCD_ShowNum_Time_Ex(x+30,y,sec,2,16);
}


/**************************************************************************************
功能: 电池图标电量显示
输入: 无
输  出: 无
**************************************************************************************/
void GUI_Display_Ui_Battery_Level_Icon(void)
{
#if defined(_BC_FES_)
		GUI_FES_Display_Ui_Battery_Level_Icon();
#else
		GUI_VNS_Display_Ui_Battery_Level_Icon();
#endif
}


/**************************************************************************************
功能: 电池图标充电中显示
输入: 无
输  出: 无
**************************************************************************************/
void GUI_Display_Ui_Charging_Battery_Icon(void)
{
#if defined(_BC_FES_)
		GUI_FES_Display_Ui_Charging_Battery_Icon();
#else
		GUI_VNS_Display_Ui_Charging_Battery_Icon();
#endif
}


/**************************************************************************************
功能: 0-9数字图片显示
输入: x,y,num,high_light
输  出: 无
**************************************************************************************/
void GUI_Display_Num(u8 x, u8 y, u8 num,u8 high_light)
{
    if(high_light)
    {
        //Gui_DrawImage(118,y+8,16,13,image_fes_num_up);
        //Gui_DrawImage(190,y+8,16,13,image_fes_num_down);
  			switch(num)
		    {
		        case 0:Gui_DrawImage(x,y,19,29,image_fes_num_h_0);
							     break;
						case 1:Gui_DrawImage(x,y,19,29,image_fes_num_h_1);
							     break;
					  case 2:Gui_DrawImage(x,y,19,29,image_fes_num_h_2);
							     break;
					  case 3:Gui_DrawImage(x,y,19,29,image_fes_num_h_3);
							     break;
						case 4:Gui_DrawImage(x,y,19,29,image_fes_num_h_4);
							     break;
					  case 5:Gui_DrawImage(x,y,19,29,image_fes_num_h_5);
							     break;
						case 6:Gui_DrawImage(x,y,19,29,image_fes_num_h_6);
							     break;
					  case 7:Gui_DrawImage(x,y,19,29,image_fes_num_h_7);
							     break;
					  case 8:Gui_DrawImage(x,y,19,29,image_fes_num_h_8);
							     break;
						case 9:Gui_DrawImage(x,y,19,29,image_fes_num_h_9);

						default : break;
				}
		}
    else
    {
				switch(num)
		    {
		        case 0:Gui_DrawImage(x,y,19,29,image_fes_num_0);
							     break;
						case 1:Gui_DrawImage(x,y,19,29,image_fes_num_1);
							     break;
					  case 2:Gui_DrawImage(x,y,19,29,image_fes_num_2);
							     break;
					  case 3:Gui_DrawImage(x,y,19,29,image_fes_num_3);
							     break;
						case 4:Gui_DrawImage(x,y,19,29,image_fes_num_4);
							     break;
					  case 5:Gui_DrawImage(x,y,19,29,image_fes_num_5);
							     break;
						case 6:Gui_DrawImage(x,y,19,29,image_fes_num_6);
							     break;
					  case 7:Gui_DrawImage(x,y,19,29,image_fes_num_7);
							     break;
					  case 8:Gui_DrawImage(x,y,19,29,image_fes_num_8);
							     break;
						case 9:Gui_DrawImage(x,y,19,29,image_fes_num_9);

						default : break;
				}
		}
    
}


/**************************************************************************************
功能: 画数字图片
输入: 无
输  出: 无
**************************************************************************************/
void GUI_DrawImageNum_Ex(u16 x,u16 y,u8 strength,u8 high_light,u8 index_ok)
{
    u8 temp_h = 0;
		u8 temp_l = 0;

		temp_h = strength/10;
		temp_l = strength%10;

		if(0 == temp_h)
		{
				LCD_Clear_Fill(WHITE,x-10,y,x+30,y+34);
				GUI_Display_Num(x,y,temp_l,high_light);
		}
		else
		{
		    LCD_Clear_Fill(WHITE,x,y,x+30,y+34);
        GUI_Display_Num(x-10,y,temp_h,high_light);
				GUI_Display_Num(x+12,y,temp_l,high_light);
		}			

		if(index_ok)
    {
				Gui_DrawImage(x-50,y+8,16,13,image_fes_num_up);
		    Gui_DrawImage(x+50,y+8,16,13,image_fes_num_down);
    }
		else
		{
				LCD_Clear_Fill(WHITE,x-50,y+8,x-12,y+24);
		}
}



#if defined(_BC_FES_)
/**************************************************************************************
功能: 画可调节的数字图片(1通道界面)
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_DrawImageNum_Adjust(u8 strength,u8 high_light,u8 index_ok)
{
		GUI_DrawImageNum_Ex(152,100,strength,high_light,index_ok);
}


/**************************************************************************************
功能: 画可调节的数字图片(治疗时间设置界面)
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_DrawImageNum_Adjust_Ex(u8 time,u8 high_light,u8 index_ok)
{
		GUI_DrawImageNum_Ex(152,118,time,high_light,index_ok);
}


/**************************************************************************************
功能: 画可调节的数字图片(2通道界面A)
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_DrawImageNum_Adjust_Two_Cha(u8 strength,u8 high_light,u8 index_ok)
{
		GUI_DrawImageNum_Ex(68,100,strength,high_light,index_ok);
}


/**************************************************************************************
功能: 画可调节的数字图片(2通道界面B)
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_DrawImageNum_Adjust_Two_Chb(u8 strength,u8 high_light,u8 index_ok)
{
		GUI_DrawImageNum_Ex(228,100,strength,high_light,index_ok);
}


/**************************************************************************************
功能: FES主界面通道选择框
输入: 通道A/B/
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Main_Menu_Ch_Selec(uint8_t ch_selec,uint8_t ch_a_flag,uint8_t ch_b_flag)
{
#if defined(_BC_FES_SWITCH)
    //A select
		if(CH_FES_A == ch_selec) 
		{
				Gui_DrawImage(60,100,37,46,image_fes_ch_a_s);
		}
		else
		{
			  Gui_DrawImage(60,100,37,46,image_fes_ch_a);
		}

		//B select
		if(CH_FES_B == ch_selec)
		{
				Gui_DrawImage(223,100,37,46,image_fes_ch_b_s);
		}	
		else
		{
				Gui_DrawImage(223,100,37,46,image_fes_ch_b);
		}
#else
		//A select
    if(CH_FES_A == ch_selec) 
    {
				if(CH_A_ON == ch_a_flag) 
		    {
				    Gui_DrawImage(60,90,37,46,image_fes_ch_a_s);
				    Gui_DrawImage(72,152,17,17,image_fes_point_s); 
		    }
				else
				{
				    Gui_DrawImage(60,90,37,46,image_fes_ch_a_s);
		    		Gui_DrawImage(72,152,17,17,image_fes_point);
				}
    }
		else
		{
    		if(CH_A_ON == ch_a_flag) 
		    {
				    Gui_DrawImage(60,90,37,46,image_fes_ch_a);
				    Gui_DrawImage(72,152,17,17,image_fes_point_s); 
		    }
				else
				{
				    Gui_DrawImage(60,90,37,46,image_fes_ch_a);
		    		Gui_DrawImage(72,152,17,17,image_fes_point);
				}
		}  

		
    //B select
		if(CH_FES_B == ch_selec)
		{
				if(CH_B_ON == ch_b_flag)
				{ 
						Gui_DrawImage(223,90,37,46,image_fes_ch_b_s);
		     		Gui_DrawImage(232,152,17,17,image_fes_point_s);
				}
				else
				{
        		Gui_DrawImage(223,90,37,46,image_fes_ch_b_s);
		     		Gui_DrawImage(232,152,17,17,image_fes_point);
				}
		}	
		else
		{
				if(CH_B_ON == ch_b_flag)
				{ 
						Gui_DrawImage(223,90,37,46,image_fes_ch_b);
		     		Gui_DrawImage(232,152,17,17,image_fes_point_s);
				}
				else
				{
        		Gui_DrawImage(223,90,37,46,image_fes_ch_b);
		     		Gui_DrawImage(232,152,17,17,image_fes_point);
				}
		}

		//ok
		if(CH_FES_OK == ch_selec)
		{
				Gui_DrawImage(128,194,64,31,image_fes_ok_s);
		}
		else
		{
    		Gui_DrawImage(128,194,64,31,image_fes_ok);
		}
#endif
}


/**************************************************************************************
功能: 模式选择界面背景
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Main_Mode_Background(uint8_t ch_a_flag,uint8_t ch_b_flag,uint8_t two_ch_flag)
{
		Gui_DrawLine(60,26,120,26,BLACK); //左边线条
		if((CH_A_ON == ch_a_flag) && !two_ch_flag)
		{
    		Gui_DrawImage(148,16,25,28,image_fes_a);
		}
		else if((CH_B_ON == ch_b_flag) && !two_ch_flag)
		{
				Gui_DrawImage(148,16,25,28,image_fes_b);
		}
		else if(two_ch_flag)
		{
				Gui_DrawImage(148,16,25,28,image_fes_b);
		}
		Gui_DrawLine(200,26,260,26,BLACK);//右边线条

		Gui_DrawLine(36,130,126,130,BLACK);//左边横
		Gui_DrawLine(194,130,284,130,BLACK);//右边横
		
		Gui_DrawLine(160,68,160,120,BLACK);//中间竖线
		Gui_DrawLine(160,148,160,200,BLACK);		 
}


/**************************************************************************************
功能: FES通道A模式选择界面
输入: 上肢/下肢/吞咽/自定义
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Main_Ch_Mode_Back(uint8_t mode)
{
    //if(CH_A_ON == ch_a_flag)
    {
        //mode upper
        if(MODE_FES_UPPER == mode)
        {
		        Gui_DrawImage(46,80,63,32,image_fes_upper_s);
        }
        else
        {
		        Gui_DrawImage(46,80,62,32,image_fes_upper);
        }

        //lower
        if(MODE_FES_LOWER == mode)
        {
						Gui_DrawImage(212,80,63,32,image_fes_lower_s);
        }
				else
				{
				    Gui_DrawImage(212,80,62,32,image_fes_lower);
				}

				//swallow
				if(MODE_FES_SWALLOW == mode)
				{
				    if(gui_fes_struct_para.menu_id == MENU_ID_FES_MODE_CHA)
				    {
						    Gui_DrawImage(46,152,65,33,gImage_fes_combination_s);
				    }
						else
						{
                Gui_DrawImage(46,152,63,32,image_fes_swallow_s);
						}
				}
				else
				{
						if(gui_fes_struct_para.menu_id == MENU_ID_FES_MODE_CHA)
				    {
						    Gui_DrawImage(46,152,65,33,gImage_fes_combination);
				    }
						else
						{
                Gui_DrawImage(46,152,62,32,image_fes_swallow);
						}
				}
			
				//custom
				if(MODE_FES_CUSTOM == mode)
				{
						Gui_DrawImage(212,152,91,32,image_fes_custom_s);
				}
				else
				{
						Gui_DrawImage(212,152,92,31,image_fes_custom);
				}
    }
}


/**************************************************************************************
功能: FES刺激界面背景 CHA
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Main_Cha_Stimulate_Background(void)
{
    Gui_DrawLine(60,26,120,26,BLACK); //左边线条
    Gui_DrawImage(148,16,20,22,image_fes_cha_title);
		Gui_DrawLine(200,26,260,26,BLACK);//右边线条
		
		Gui_DrawImage(140,54,41,20,image_fes_str);

		GUI_FES_DrawImageNum_Adjust(gui_fes_struct_para.cha_current_strength,0,0);
		
		Gui_DrawImage(104,158,117,60,image_fes_remaining_time);
    GUI_Display_Countdown_Time(140,188,gui_fes_struct_para.cha_time_min,gui_fes_struct_para.cha_time_sec);
			
}


/**************************************************************************************
功能: FES刺激界面背景 CHB
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Main_Chb_Stimulate_Background(void)
{
    Gui_DrawLine(60,26,120,26,BLACK); //左边线条
    Gui_DrawImage(148,16,20,22,image_fes_chb_title);
		Gui_DrawLine(200,26,260,26,BLACK);//右边线条
		
		Gui_DrawImage(140,54,41,20,image_fes_str);

#if defined(_BC_FES_SWITCH)	
		GUI_FES_DrawImageNum_Adjust(gui_fes_struct_para.cha_current_strength,0,0);
#else
		GUI_FES_DrawImageNum_Adjust(gui_fes_struct_para.chb_current_strength,0,0);
#endif
		Gui_DrawImage(104,158,117,60,image_fes_remaining_time);
		GUI_Display_Countdown_Time(140,188,gui_fes_struct_para.chb_time_min,gui_fes_struct_para.chb_time_sec);
}


void GUI_FES_Display_Ui_Main_Cha_Chb_Stimulate_Select(uint8_t chab_stimu_index)
{
    if(MODE_FES_STRENGTH_CHA == chab_stimu_index)
    {
				GUI_FES_DrawImageNum_Adjust_Two_Cha(gui_fes_struct_para.cha_current_strength,1,gui_fes_struct_para.chab_stimu_cha_strength_on_flag);
				//Gui_DrawLine(56,132,100,132,GBLUE); //画线
				GUI_Display_Countdown_Time(56,188,gui_fes_struct_para.cha_time_min,gui_fes_struct_para.cha_time_sec);
				GUI_FES_DrawImageNum_Adjust_Two_Chb(gui_fes_struct_para.chb_current_strength,0,0);
		    GUI_Display_Countdown_Time(216,188,gui_fes_struct_para.chb_time_min,gui_fes_struct_para.chb_time_sec);
    }
		else if(MODE_FES_STOP_START_CHA == chab_stimu_index)
		{
				GUI_FES_DrawImageNum_Adjust_Two_Cha(gui_fes_struct_para.cha_current_strength,0,0);
				GUI_Display_Countdown_Time_Ex(56,188,gui_fes_struct_para.cha_time_min,gui_fes_struct_para.cha_time_sec);//high light
				GUI_FES_DrawImageNum_Adjust_Two_Chb(gui_fes_struct_para.chb_current_strength,0,0);
		    GUI_Display_Countdown_Time(216,188,gui_fes_struct_para.chb_time_min,gui_fes_struct_para.chb_time_sec);
		}
		else if(MODE_FES_STRENGTH_CHB == chab_stimu_index)
		{
				GUI_FES_DrawImageNum_Adjust_Two_Cha(gui_fes_struct_para.cha_current_strength,0,0);
				GUI_Display_Countdown_Time(56,188,gui_fes_struct_para.cha_time_min,gui_fes_struct_para.cha_time_sec);
				GUI_FES_DrawImageNum_Adjust_Two_Chb(gui_fes_struct_para.chb_current_strength,1,gui_fes_struct_para.chab_stimu_chb_strength_on_flag);//high light
		    GUI_Display_Countdown_Time(216,188,gui_fes_struct_para.chb_time_min,gui_fes_struct_para.chb_time_sec);
		}
		else if(MODE_FES_STOP_START_CHB == chab_stimu_index)
		{
				GUI_FES_DrawImageNum_Adjust_Two_Cha(gui_fes_struct_para.cha_current_strength,0,0);
				GUI_Display_Countdown_Time(56,188,gui_fes_struct_para.cha_time_min,gui_fes_struct_para.cha_time_sec);
				GUI_FES_DrawImageNum_Adjust_Two_Chb(gui_fes_struct_para.chb_current_strength,0,0);
		    GUI_Display_Countdown_Time_Ex(216,188,gui_fes_struct_para.chb_time_min,gui_fes_struct_para.chb_time_sec);//high light
		}	
		else
		{
       
		}
}


/**************************************************************************************
功能: FES刺激界面背景 CHA CHB 
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Main_Cha_Chb_Stimulate_Background(uint8_t chab_stimu_index)
{
    //A 通道
		Gui_DrawLine(20,26,60,26,BLACK); //左边线条
    Gui_DrawImage(70,16,20,22,image_fes_cha_title);
		Gui_DrawLine(100,26,140,26,BLACK);//右边线条
		
		Gui_DrawImage(60,54,41,20,image_fes_str);	
		Gui_DrawImage(20,158,117,60,image_fes_remaining_time);

		Gui_DrawLine(160,50,160,210,BLACK);//中间竖线
		
		//B 通道
		Gui_DrawLine(180,26,220,26,BLACK); //左边线条
		Gui_DrawImage(230,16,20,22,image_fes_chb_title);
		Gui_DrawLine(260,26,300,26,BLACK);//右边线条

		Gui_DrawImage(220,54,41,20,image_fes_str);
		Gui_DrawImage(180,158,117,60,image_fes_remaining_time);

		GUI_FES_Display_Ui_Main_Cha_Chb_Stimulate_Select(chab_stimu_index);
		
}


/**************************************************************************************
功能: FES刺激界面
输入: 通道号/模式
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Main_Ch_Stimulate(uint8_t ch_a_flag,uint8_t ch_b_flag,uint8_t a_mode_index,uint8_t b_mode_index,uint8_t chab_stimu_index)
{
    //LCD_Clear(WHITE);
    GUI_Display_Clear_No_Icon();
		
    if((CH_A_ON == ch_a_flag) && (CH_B_OFF == ch_b_flag))
    {
		    gui_fes_struct_para.menu_id = MENU_ID_FES_STIMULATE_CHA;	
			  GUI_FES_Display_Ui_Main_Cha_Stimulate_Background();
    }
		
		if((CH_A_OFF == ch_a_flag) && (CH_B_ON == ch_b_flag))
		{
				gui_fes_struct_para.menu_id = MENU_ID_FES_STIMULATE_CHB;	
				GUI_FES_Display_Ui_Main_Chb_Stimulate_Background();
		}
	
    if((CH_A_ON == ch_a_flag) && (CH_B_ON == ch_b_flag))
		{
				gui_fes_struct_para.menu_id = MENU_ID_FES_STIMULATE_CHA_CHB;	
				GUI_FES_Display_Ui_Main_Cha_Chb_Stimulate_Background(chab_stimu_index);	
		}		
}


/**************************************************************************************
功能: SWITCH 模式选择
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Main_Ch_Stimulate_Switch(uint8_t selec_index)
{
    //LCD_Clear(WHITE);
    GUI_Display_Clear_No_Icon();
		
    if(CH_FES_A == selec_index)
    {
		    gui_fes_struct_para.menu_id = MENU_ID_FES_STIMULATE_CHA;	
			  GUI_FES_Display_Ui_Main_Cha_Stimulate_Background();
    }
		
		if(CH_FES_B == selec_index)
		{
				gui_fes_struct_para.menu_id = MENU_ID_FES_STIMULATE_CHB;	
				GUI_FES_Display_Ui_Main_Chb_Stimulate_Background();
		}
}



/**************************************************************************************
功能: 模式选择界面
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Mode_Selec(uint8_t ch_a_flag,uint8_t ch_b_flag,uint8_t mode_index,uint8_t two_ch_flag)
{
		//LCD_Clear(WHITE);
		GUI_Display_Clear_No_Icon();

		if((CH_A_ON == ch_a_flag) && (CH_B_OFF == ch_b_flag))
    {
		    gui_fes_struct_para.menu_id = MENU_ID_FES_MODE_CHA;
    }
		
		if((CH_A_OFF == ch_a_flag) && (CH_B_ON == ch_b_flag))
		{
				gui_fes_struct_para.menu_id = MENU_ID_FES_MODE_CHB;
		}
	
    if((CH_A_ON == ch_a_flag) && (CH_B_ON == ch_b_flag) && !two_ch_flag)
		{
				gui_fes_struct_para.menu_id = MENU_ID_FES_MODE_CHA;		
		}		

		if((CH_A_ON == ch_a_flag) && (CH_B_ON == ch_b_flag) && two_ch_flag)
		{
				gui_fes_struct_para.menu_id = MENU_ID_FES_MODE_CHB;		
		}		
	
		GUI_FES_Display_Ui_Main_Mode_Background(ch_a_flag,ch_b_flag,two_ch_flag);
		GUI_FES_Display_Ui_Main_Ch_Mode_Back(mode_index);  
}


/**************************************************************************************
功能: SWITCH模式选择 背景
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Main_Mode_Background_Swith(uint8_t selec_index)
{
		Gui_DrawLine(60,26,120,26,BLACK); //左边线条
		if(CH_FES_A == selec_index)
		{
    		Gui_DrawImage(148,16,25,28,image_fes_a);
		}
		else if(CH_FES_B == selec_index)
		{
				Gui_DrawImage(148,16,25,28,image_fes_b);
		}
		
		Gui_DrawLine(200,26,260,26,BLACK);//右边线条

		Gui_DrawLine(36,130,126,130,BLACK);//左边横
		Gui_DrawLine(194,130,284,130,BLACK);//右边横
		
		Gui_DrawLine(160,68,160,120,BLACK);//中间竖线
		Gui_DrawLine(160,148,160,200,BLACK);		 
}


/**************************************************************************************
功能: SWITCH模式选择界面
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Mode_Selec_Switch(uint8_t selec_index,uint8_t mode_index)
{
		//LCD_Clear(WHITE);
		GUI_Display_Clear_No_Icon();

		if(CH_FES_A == selec_index)
    {
		    gui_fes_struct_para.menu_id = MENU_ID_FES_MODE_CHA;
    }
		else if(CH_FES_B == selec_index)
		{
        gui_fes_struct_para.menu_id = MENU_ID_FES_MODE_CHB;
		}
		
		GUI_FES_Display_Ui_Main_Mode_Background_Swith(selec_index);
		GUI_FES_Display_Ui_Main_Ch_Mode_Back(mode_index);  
}


/**************************************************************************************
功能: 治疗时间设置界面
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Stimu_Time(uint8_t selec_index,uint8_t stimu_time)
{
		//LCD_Clear(WHITE);
		GUI_Display_Clear_No_Icon();
		
		gui_fes_struct_para.menu_id = MENU_ID_FES_STIMULATE_TIME;

		Gui_DrawLine(60,26,120,26,BLACK); //左边线条
		if(CH_FES_A == selec_index)
		{
    		Gui_DrawImage(148,16,25,28,image_fes_a);
		}
		else if(CH_FES_B == selec_index)
		{
				Gui_DrawImage(148,16,25,28,image_fes_b);
		}
		Gui_DrawLine(200,26,260,26,BLACK);//右边线条

		Gui_DrawImage(80,86,161,82,image_treatment_time);
    LCD_ShowString(190,130,16,(u8 *)"min",1);
		GUI_FES_DrawImageNum_Adjust_Ex(gui_fes_struct_para.stimu_time,0,0);
}


/**************************************************************************************
功能: 连续动作训练界面
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Stimu_Combina_Main(uint8_t is_stop)
{
		GUI_Display_Clear_No_Icon();
		
		gui_fes_struct_para.menu_id = MENU_ID_FES_MODE_COMBINA;

		//training	
		Gui_DrawImage(98,68,139,53,gImage_fes_training);

	  if(is_stop)
	  {
				Gui_DrawImage(150,150,36,37,gImage_fes_doing); 
	  }
		else
		{
				Gui_DrawImage(150,150,36,37,gImage_fes_stop);
		}
}


/**************************************************************************************
功能: 连续动作训练暂停开始
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Stimu_Combina(uint8_t is_stop)
{
	  if(is_stop)
	  {
				Gui_DrawImage(150,150,36,37,gImage_fes_doing); 
	  }
		else
		{
				Gui_DrawImage(150,150,36,37,gImage_fes_stop);
		}
}




/**************************************************************************************
功能: FES电池图标电量显示
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Battery_Level_Icon(void)
{
    if(BATTERY_ICON_3 == Get_Adc_Battery_Level_Status())
    {
        Gui_Drawbmp16X(294,5,gImage_image_battery3);
    }
		else if(BATTERY_ICON_2 == Get_Adc_Battery_Level_Status())
		{
        Gui_Drawbmp16X(294,5,gImage_image_battery2);
		}
		else if(BATTERY_ICON_1 == Get_Adc_Battery_Level_Status())
		{
        Gui_Drawbmp16X(294,5,gImage_image_battery1);
		}
		else
		{
        Gui_Drawbmp16X(294,5,gImage_image_battery0);
		}
}


/**************************************************************************************
功能: FES电池图标充电中显示
输入: 无
输  出: 无
**************************************************************************************/
void GUI_FES_Display_Ui_Charging_Battery_Icon(void)
{
    static u8 count = 0;

    if(3 > count)
    {
				count++;

				if(1 == count)
				{
						Gui_Drawbmp16X(294,5,gImage_image_battery1);
				}
				else if(2 == count)
				{
						Gui_Drawbmp16X(294,5,gImage_image_battery2);
				}
				else if(3 == count)
				{
						count = 0;
						Gui_Drawbmp16X(294,5,gImage_image_battery3);
				}
    }
}



#else
/**************************************************************************************
功能: 画可调节的数字图片(1通道界面)
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_DrawImageNum_Adjust(u8 strength,u8 high_light,u8 index_ok)
{
		GUI_DrawImageNum_Ex(106,142,strength,high_light,index_ok);
}


/**************************************************************************************
功能: 画可调节的数字图片(强度测试界面界面)
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_DrawImageNum_Test_Adjust(u8 strength,u8 high_light,u8 index_ok)
{
		GUI_DrawImageNum_Ex(104,172,strength,high_light,index_ok);
}


/**************************************************************************************
功能: 画可调节的数字图片(治疗时间设置界面)
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_DrawImageNum_Adjust_Ex(u8 time,u8 high_light,u8 index_ok)
{
		GUI_DrawImageNum_Ex(152,118,time,high_light,index_ok);
}

/**************************************************************************************
功能: VNS电池图标电量显示
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Battery_Level_Icon(void)
{
    if(BATTERY_ICON_3 == Get_Adc_Battery_Level_Status())
    {
        Gui_Drawbmp16X(216,1,gImage_image_battery3);
    }
		else if(BATTERY_ICON_2 == Get_Adc_Battery_Level_Status())
		{
        Gui_Drawbmp16X(216,1,gImage_image_battery2);
		}
		else if(BATTERY_ICON_1 == Get_Adc_Battery_Level_Status())
		{
        Gui_Drawbmp16X(216,1,gImage_image_battery1);
		}
		else
		{
        Gui_Drawbmp16X(216,1,gImage_image_battery0);
		}
}


/**************************************************************************************
功能: VNS电池图标充电中显示
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Charging_Battery_Icon(void)
{
    static u8 count = 0;

    if(3 > count)
    {
				count++;

				if(1 == count)
				{
						Gui_Drawbmp16X(216,1,gImage_image_battery1);
				}
				else if(2 == count)
				{
						Gui_Drawbmp16X(216,1,gImage_image_battery2);
				}
				else if(3 == count)
				{
						count = 0;
						Gui_Drawbmp16X(216,1,gImage_image_battery3);
				}
    }
}

/**************************************************************************************
功能: 显示每个模式界面的标题
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Title(void)
{
    if(MODE_A == gui_struct_para.menu_mode_index)
		{
		    Gui_DrawImage(100,26,29,33,gImage_icon_mode_a);
		}
		else if(MODE_B == gui_struct_para.menu_mode_index)
		{
		    Gui_DrawImage(100,26,29,33,gImage_icon_mode_b);
		}
		else if(MODE_C == gui_struct_para.menu_mode_index)
		{
		    Gui_DrawImage(100,26,29,33,gImage_icon_mode_c);
		}
		else if(MODE_CUSTOM == gui_struct_para.menu_mode_index)
		{
		    Gui_DrawImage(74,26,85,31,gImage_icon_mode_custom);
		}
		
}


/**************************************************************************************
功能: 显示每个模式强度测试界面的的标题
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Test_Title(void)
{
    Gui_DrawLine(30,32,50,32,BLACK); //左边线条
		Gui_DrawImage(60,20,117,28,gImage_icon_test_str);
		Gui_DrawLine(188,32,210,32,BLACK);//右边线条
    //Gui_DrawImage(46,30,117,28,);
    if(MODE_A == gui_struct_para.menu_mode_index)
		{
		    Gui_DrawImage(100,56,29,33,gImage_icon_mode_a);
		}
		else if(MODE_B == gui_struct_para.menu_mode_index)
		{
		    Gui_DrawImage(100,56,29,33,gImage_icon_mode_b);
		}
		else if(MODE_C == gui_struct_para.menu_mode_index)
		{
		    Gui_DrawImage(100,56,29,33,gImage_icon_mode_c);
		}
		else if(MODE_CUSTOM == gui_struct_para.menu_mode_index)
		{
		    //Gui_DrawImage(90,30,85,31,gImage_icon_mode_custom);
		}
		
}



/**************************************************************************************
功能: 刺激工作界面
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Stimulate(void)
{    
    gui_struct_para.menu_id = MENU_ID_STIMULATE;
		GUI_Display_Clear_No_Icon();	

    Gui_DrawLine(36,42,66,42,BLACK); //左边线条
    GUI_VNS_Display_Ui_Title();
		Gui_DrawLine(160,42,190,42,BLACK);//右边线条
		
		Gui_DrawImage(88,80,59,30,gImage_icon_stim_strength);

		GUI_VNS_DrawImageNum_Adjust(gui_struct_para.current_strength,0,0);
		
		Gui_DrawImage(40,220,160,81,gImage_icon_stim_time);

		GUI_Display_Ui_Electrode_Connect();
    GUI_Display_Countdown_Time(96,260,gui_struct_para.time_min,gui_struct_para.time_sec);
}



/**************************************************************************************
功能: 刺激强度调节界面
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Adjust_Strength(void)
{
    gui_struct_para.menu_id = MENU_ID_STRENGTH;
		GUI_Display_Clear_No_Icon();

		GUI_VNS_Display_Ui_Test_Title();
		
    Gui_DrawImage(46,114,149,142,gImage_icon_curr_stren);
		GUI_VNS_DrawImageNum_Test_Adjust(gui_struct_para.current_strength,1,0);
}



/**************************************************************************************
功能: 自定义界面-脉宽
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Pluse_Width(void)
{
    if(gui_struct_para.us_val < VNS_PULSE_WIDTH_MAX)
    {
        LCD_Clear_Fill(WHITE,36,92,76,112);
    		LCD_ShowNum(38,94,gui_struct_para.us_val,3,16);
		}
		else
		{
		    LCD_Clear_Fill(WHITE,36,92,76,112);
		    LCD_ShowNum(38,94,gui_struct_para.us_val,4,16);
		}

		LCD_ShowString(74,94,16,(u8 *)"us",1);
}


/**************************************************************************************
功能: 自定义界面-脉宽(选中)
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Pluse_Width_Ex(void)
{
    if(gui_struct_para.us_val < VNS_PULSE_WIDTH_MAX)
    {
        LCD_Clear_Fill(WHITE,36,92,76,112);
    		LCD_ShowNum_BackColor_Ex(38,94,gui_struct_para.us_val,3,16);
		}
		else
		{
		    LCD_Clear_Fill(WHITE,36,92,76,112);
		    LCD_ShowNum_BackColor_Ex(38,94,gui_struct_para.us_val,4,16);
		}

		LCD_ShowString(74,94,16,(u8 *)"us",1);
}



/**************************************************************************************
功能: 自定义界面-频率
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Frequency(void)
{
		LCD_ShowNum(154,94,gui_struct_para.hz_val,2,16);
    LCD_ShowString(178,94,16,(u8 *)"Hz",1);
}


/**************************************************************************************
功能: 自定义界面-频率（选中）
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Frequency_Ex(void)
{
		LCD_ShowNum_BackColor_Ex(154,94,gui_struct_para.hz_val,2,16);
    LCD_ShowString(178,94,16,(u8 *)"Hz",1);
}


/**************************************************************************************
功能: 自定义界面-强度
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Strength(void)
{
		LCD_ShowNum(44,162,gui_struct_para.current_strength,2,16);
}


/**************************************************************************************
功能: 自定义界面-强度
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Strength_Ex(void)
{
		LCD_ShowNum_BackColor_Ex(44,162,gui_struct_para.current_strength,2,16);
}


/**************************************************************************************
功能: 自定义界面-总时长
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Total_Time(void)
{
		LCD_ShowNum(150,162,gui_struct_para.total_time,2,16);
		//Gui_DrawImage(180,146,34,24,gImage_unit_time);
		LCD_ShowString(178,162,16,(u8 *)"min",1);
}


/**************************************************************************************
功能: 自定义界面-总时长(选中)
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Total_Time_Ex(void)
{
		LCD_ShowNum_BackColor_Ex(150,162,gui_struct_para.total_time,2,16);
		//Gui_DrawImage(180,146,34,24,gImage_unit_time);
		LCD_ShowString(178,162,16,(u8 *)"min",1);
}


/**************************************************************************************
功能: 自定义界面-刺激时间
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Start_Time(void)
{
    u8 num_len = 0;
    if((0 < gui_struct_para.start_time) && (100 > gui_struct_para.start_time))
    {
		    num_len = 2;
    }
		else if((100 <= gui_struct_para.start_time) && (1000 > gui_struct_para.start_time))
		{
		    num_len = 3;
    }
		else if(1000 <= gui_struct_para.start_time)
		{
		    num_len = 4;
    }
		LCD_Clear_Fill(WHITE,36,234,86,254);
		LCD_ShowNum(40,236,gui_struct_para.start_time,num_len,16);
		LCD_ShowString(76,236,16,(u8 *)"s",1);
}


/**************************************************************************************
功能: 自定义界面-刺激时间(选中)
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Start_Time_Ex(void)
{
    u8 num_len = 0;
		
    if((0 < gui_struct_para.start_time) && (100 > gui_struct_para.start_time))
    {
		    num_len = 2;
    }
		else if((100 <= gui_struct_para.start_time) && (1000 > gui_struct_para.start_time))
		{
		    num_len = 3;
    }
		else if(1000 <= gui_struct_para.start_time)
		{
		    num_len = 4;
    }
		LCD_Clear_Fill(WHITE,36,234,86,254);
		LCD_ShowNum_BackColor_Ex(40,236,gui_struct_para.start_time,num_len,16);
		LCD_ShowString(76,236,16,(u8 *)"s",1);
}


/**************************************************************************************
功能: 自定义界面-休息时间
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Stop_Time(void)
{
		LCD_ShowNum(150,236,gui_struct_para.stop_time,3,16);
		//Gui_DrawImage(174,222,22,24,gImage_unit_time_s);
		LCD_ShowString(184,236,16,(u8 *)"s",1);
}


/**************************************************************************************
功能: 自定义界面-休息时间(选中)
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Stop_Time_Ex(void)
{
		LCD_ShowNum_BackColor_Ex(150,236,gui_struct_para.stop_time,3,16);
		//Gui_DrawImage(174,222,22,24,gImage_unit_time_s);
		LCD_ShowString(184,236,16,(u8 *)"s",1);
}


/**************************************************************************************
功能: 自定义界面
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Menu(void)
{    
    gui_struct_para.menu_id = MENU_ID_CUSTOM;
		GUI_Display_Clear_No_Icon();
    GUI_VNS_Display_Ui_Custom_Selec(gui_struct_para.custom_index);
}



/**************************************************************************************
功能: 自定义界面-背景画线
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Bg(void)
{
    Gui_DrawLine(36,28,66,28,BLACK); //左边线条
    Gui_DrawImage(74,14,85,31,gImage_icon_mode_custom);
		Gui_DrawLine(160,28,190,28,BLACK);//右边线条	

    Gui_DrawLine(120,64,120,270,BLACK);//中间竖线
		Gui_DrawLine(30,120,210,120,BLACK);
		Gui_DrawLine(30,186,210,186,BLACK);

}



/**************************************************************************************
功能: 自定义界面-选择
输入: 无
输  出: 无
**************************************************************************************/
void GUI_VNS_Display_Ui_Custom_Selec(uint8_t custon_index)
{    
    GUI_VNS_Display_Ui_Custom_Bg();

    /*脉宽*/
		Gui_DrawImage(10,68,84,22,gImage_custom_us); 
		GUI_VNS_Display_Ui_Custom_Pluse_Width();

    /*频率*/
		Gui_DrawImage(126,68,84,22,gImage_custom_hz);
    GUI_VNS_Display_Ui_Custom_Frequency();

    /*强度*/
		Gui_DrawImage(12,136,84,22,gImage_custom_ma); 
		GUI_VNS_Display_Ui_Custom_Strength();

    /*总时长*/
		Gui_DrawImage(126,136,84,22,gImage_custom_time); 
		GUI_VNS_Display_Ui_Custom_Total_Time();

    /*刺激时间*/
		Gui_DrawImage(16,204,84,22,gImage_custom_start);
		GUI_VNS_Display_Ui_Custom_Start_Time();

    /*休息时间*/
		Gui_DrawImage(130,204,84,22,gImage_custom_stop); 
    GUI_VNS_Display_Ui_Custom_Stop_Time();

    /*确定*/
		Gui_DrawImage(86,280,68,33,gImage_custom_ok); 

		if(CUSTOM_ID_PLUSE == custon_index)
    {
        Gui_DrawImage(10,68,84,22,gImage_custom_us_h); 
        if(gui_struct_para.ok_flag) //选中
        {
						GUI_VNS_Display_Ui_Custom_Pluse_Width_Ex();
				}
		}
		else if(CUSTOM_ID_FREQU == custon_index)
    {
        Gui_DrawImage(126,68,84,22,gImage_custom_hz_h); 
        if(gui_struct_para.ok_flag)
        {
						GUI_VNS_Display_Ui_Custom_Frequency_Ex();
        }
		}
    else if(CUSTOM_ID_STREN == custon_index)
		{
		    Gui_DrawImage(12,136,84,22,gImage_custom_ma_h); 
		    if(gui_struct_para.ok_flag)
		    {
						GUI_VNS_Display_Ui_Custom_Strength_Ex();
				}
		}
    else if(CUSTOM_ID_TOTIM == custon_index)
		{
		    Gui_DrawImage(126,136,84,22,gImage_custom_time_h); 
		    if(gui_struct_para.ok_flag)
		    {
						GUI_VNS_Display_Ui_Custom_Total_Time_Ex();
				}
		}
		else if(CUSTOM_ID_STATIM == custon_index)	
		{
		    Gui_DrawImage(16,204,84,22,gImage_custom_start_h);
		    if(gui_struct_para.ok_flag)
		    {
						GUI_VNS_Display_Ui_Custom_Start_Time_Ex();
		    }
		}
		else if(CUSTOM_ID_STOTIM == custon_index)
		{
		    Gui_DrawImage(130,204,84,22,gImage_custom_stop_h); 
		    if(gui_struct_para.ok_flag)
		    {
						GUI_VNS_Display_Ui_Custom_Stop_Time_Ex();
				}
		}
    else if(CUSTOM_ID_OK == custon_index)	
		{
				Gui_DrawImage(86,280,68,33,gImage_custom_ok_h); 
		}	
}



#endif



/**************************************************************************************
功能: 显示jpg logo 图片
输入: 无
输  出: 无
**************************************************************************************/
void GUI_Display_Logo_Picture(void)
{
    LCD_Clear(WHITE);
    #if defined(_BC_FES_)
		Gui_DrawImage(77,104,165,32,image_fes_logo);
		#else
		Gui_DrawImage(36,124,165,32,image_fes_logo);
		#endif
		
		//LCD_Clear(WHITE);
}


/**************************************************************************************
功能: 主界面背景
输入: 无
输  出: 无
**************************************************************************************/
void GUI_Display_Ui_Main_Back(void)
{      
    Get_Adc_Battery_Val(ADC_CHANNEL_15);
    GUI_Display_Ui_Battery_Level_Icon();
		
#if defined(_BC_FES_)
    Gui_DrawLine(60,26,90,26,BLACK); //左边线条
    Gui_DrawImage(110,16,97,23,image_fes_title);
		Gui_DrawLine(224,26,254,26,BLACK);//右边线条
		
		Gui_DrawLine(160,62,160,200,BLACK);//中间竖线  
#else
		Gui_DrawLine(30,32,50,32,BLACK); //左边线条
		Gui_DrawImage(60,20,118,29,gImage_icon_mode);
		Gui_DrawLine(188,32,210,32,BLACK);//右边线条

		Gui_DrawLine(120,60,120,300,BLACK);//中间竖线
		Gui_DrawLine(30,170,210,170,BLACK);
		
		//Gui_DrawImage(20,82,86,86,gImage_icon_a);
		//Gui_DrawImage(142,80,86,86,gImage_icon_b);
		//Gui_DrawImage(22,190,86,86,gImage_icon_c);
		//Gui_DrawImage(142,190,86,86,gImage_icon_custom);	
#endif
}


/**************************************************************************************
功能: 主界面模式选择框
输入: 模式A/B/C/自定义(mode)
输  出: 无
**************************************************************************************/
void GUI_Display_Ui_Main_Menu_Selec(uint8_t mode)
{
		GUI_Display_Ui_Main_Back();

		if(MODE_A == mode)
		{
				//DisplayButtonDown(136,78,230,170);
				//LCD_DrawRectangle(16,78,110,170);
				Gui_DrawImage(40,90,39,42,gImage_icon_a_s);
				Gui_DrawImage(160,90,39,42,gImage_icon_b);
				Gui_DrawImage(40,226,39,42,gImage_icon_c);
				Gui_DrawImage(160,226,48,43,gImage_icon_custom);
		}
		else if(MODE_B == mode)
		{
        Gui_DrawImage(40,90,39,42,gImage_icon_a);
				Gui_DrawImage(160,90,39,42,gImage_icon_b_s);
				Gui_DrawImage(40,226,39,42,gImage_icon_c);
				Gui_DrawImage(160,226,48,43,gImage_icon_custom);
		}
		else if(MODE_C == mode)
		{
        Gui_DrawImage(40,90,39,42,gImage_icon_a);
				Gui_DrawImage(160,90,39,42,gImage_icon_b);
				Gui_DrawImage(40,226,39,42,gImage_icon_c_s);
				Gui_DrawImage(160,226,48,43,gImage_icon_custom);
		}
		else if(MODE_CUSTOM == mode)
		{
				Gui_DrawImage(40,90,39,42,gImage_icon_a);
				Gui_DrawImage(160,90,39,42,gImage_icon_b);
				Gui_DrawImage(40,226,39,42,gImage_icon_c);
				Gui_DrawImage(160,226,48,43,gImage_icon_custom_s);
		}

}









/**************************************************************************************
功能: 电极连接状态界面
输入: 无
输  出: 无
**************************************************************************************/
void GUI_Display_Ui_Electrode_Connect(void)
{
		if(Get_Adc_Electrode_Connect_Status())
    {
        //LCD_Clear_Fill(WHITE,88,266,170,288);
		    //LCD_ShowString(100,270,16,(u8 *)"Connect",0);
    }
		else
		{
		    //LCD_Clear_Fill(WHITE,88,266,174,288);
        //LCD_ShowString(90,270,16,(u8 *)"Disconnect",0);
		}
}


/**************************************************************************************
功能: 模式选择界面
输入: 无
输  出: 无
**************************************************************************************/
void GUI_Display_Ui_Main_Menu(void)
{
#if defined(_BC_FES_)
		gui_fes_struct_para.menu_id = MENU_ID_FES_MAIN;
#else
    gui_struct_para.menu_id = MENU_ID_MAIN;
#endif

		GUI_Display_Clear_No_Icon();
		GUI_Display_Ui_Main_Back();
		
#if defined(_BC_FES_)
    //Get_Adc_Battery_Val(ADC_CHANNEL_15);
		//GUI_FES_Display_Ui_Battery_Level_Icon();
		
    GUI_FES_Display_Ui_Main_Menu_Ch_Selec(gui_fes_struct_para.selec_index,gui_fes_struct_para.ch_a_flag,gui_fes_struct_para.ch_b_flag);
#else
		GUI_Display_Ui_Main_Menu_Selec(gui_struct_para.menu_mode_index);
#endif
}




/*test function start*/
void GUI_Display_Ui_Main_Picture1(void)
{
    //LCD_Clear(WHITE);
		//Gui_Drawbmp16X(194,0,gImage_image_battery);
		//LCD_ShowString(0,0,16,"Picture Test",1);
		//LCD_Clear(BLACK);
		//LCD_DrawRectangle(20,80,88,88);
		//LCD_DrawFillRectangle(20,120,100,100);
		//DisplayButtonDown(20,120,100,100);
		//Gui_StrCenter(20,40,BLACK,BLACK,"模字",24,1);
		//Gui_StrCenter(20,100,BLACK,BLACK,"模",32,1);

		//GUI_Display_Ui_Main_Back();

    //DisplayButtonDown(20,80,108,168);
		//LCD_DrawRectangle(140,80,228,168);
}


void GUI_Display_Test(uint8_t num)
{
    if(1 == num)
    {
        //LCD_Clear(BLACK); 
        //GUI_Display_Ui_Mode_Strength();
		}
		else if(2 == num)
		{
        //LCD_Clear(WHITE);
		}
		else if(3 == num)
		{
        //LCD_Clear(YELLOW);
				//LCD_ShowString(20,20,16,"BrainCLOS Test",1);
				//Gui_DrawImage(20,160,81,81,gImage_image2);
				//GUI_Display_Ui_Main_Menu_Selec(MODE_B);
		}
		else if(4 == num)
		{
        //LCD_Clear(GREEN);
        //GUI_Display_Logo_Picture();
				//GUI_Display_Ui_Main_Menu();
		}
}
/*test function end*/

