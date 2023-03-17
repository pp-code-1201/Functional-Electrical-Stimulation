//#include "core_cm3.h"
#include "sys.h"

//延时nus
//nus为要延时的us数.	
//注意:nus的值不要大于1000us    3234

void delay_us(u32 nus)
{
	#if 0
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;						//LOAD的值
	ticks=nus*72; 											//需要的节拍数
	told=SysTick->VAL;        					//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)
				tcnt+=told-tnow;							//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else 
				tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;						//时间超过/等于要延迟的时间,则退出.
		}
	};
	#endif
}
 
//延时nms
//nms:要延时的ms数
void delay_ms(u16 nms)
{
	u32 i;
	for(i=0;i<nms;i++) 
		delay_us(1000);
}



































