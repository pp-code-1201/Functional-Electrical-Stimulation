//#include "core_cm3.h"
#include "sys.h"

//��ʱnus
//nusΪҪ��ʱ��us��.	
//ע��:nus��ֵ��Ҫ����1000us    3234

void delay_us(u32 nus)
{
	#if 0
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;						//LOAD��ֵ
	ticks=nus*72; 											//��Ҫ�Ľ�����
	told=SysTick->VAL;        					//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)
				tcnt+=told-tnow;							//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else 
				tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;						//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}
	};
	#endif
}
 
//��ʱnms
//nms:Ҫ��ʱ��ms��
void delay_ms(u16 nms)
{
	u32 i;
	for(i=0;i<nms;i++) 
		delay_us(1000);
}



































