#ifndef __SYS_H
#define __SYS_H 			   


#define u32 unsigned long
#define u8  unsigned char 
#define u16 unsigned int 


#define _BC_FES_       //FES��������أ�Ĭ��ΪVNS
#define _BC_FES_SWITCH //��ͨ���л�
#define _BC_FES_ACTION  //�첽��������

//��·���ԣ�PWM1һֱ���ߣ�PWM2һֱ����
//#define _TEST_

//////////////////////////////////////////////////////////////////////////////////	 
//********************************************************************************
//V1.2�޸�˵��
//�������ж��е��ó�����ѭ���Ĵ���
//��ֹ��ʱ��׼ȷ,����do while�ṹ!
////////////////////////////////////////////////////////////////////////////////// 
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























