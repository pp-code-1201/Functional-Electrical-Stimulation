#ifndef __SYS_H
#define __SYS_H 			   


#define u32 unsigned long
#define u8  unsigned char 
#define u16 unsigned int 


#define _BC_FES_       //FES打开这个开关，默认为VNS
#define _BC_FES_SWITCH //单通道切换
#define _BC_FES_ACTION  //异步连续动作

//电路测试，PWM1一直拉高，PWM2一直拉低
//#define _TEST_

//////////////////////////////////////////////////////////////////////////////////	 
//********************************************************************************
//V1.2修改说明
//修正了中断中调用出现死循环的错误
//防止延时不准确,采用do while结构!
////////////////////////////////////////////////////////////////////////////////// 
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























