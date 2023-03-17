#ifndef __APP_MAIN_H
#define __APP_MAIN_H

/*----------------------------------------------------------------------
 Include										
-----------------------------------------------------------------------*/
#include "main.h"
#include "sys.h"


/*----------------------------------------------------------------------
 Structure and enum           										
-----------------------------------------------------------------------*/
typedef enum
{
    DEVICE_START = 0,
		DEVICE_VNS,
		DEVICE_FES,
		DEVICE_EEG,
		DEVICE_FNIRS,
		DEVICE_CHECK = 0xFE,
		DEVICE_END
}DEVICE_TYPE;

typedef enum
{
    PARA_START = 0,
		PARA_OFF_ON,    //�̼�����
		PARA_MODE,      //�̼�ģʽ
		PARA_STRENGTH,  //�̼�ǿ��
		PARA_END
}PARA_TYPE;


typedef enum
{
    CHANNEL_START = 0,
		CHANNEL_A,    
		CHANNEL_B,      
		CHANNEL_END
}CHANNEL_TYPE;
		

typedef struct
{
    uint8_t is_pc;             //PC�����豸
    volatile uint8_t hr_cycle; //���ʹ�����������
    uint16_t pwm1_cycle;        //pwm1����
    uint16_t pwm2_cycle;        //pwm2����
    uint8_t  start_stop_flag;   //��ͣ/��ʼ�̼�
    uint8_t  power_off_flag;    //�ػ���־
    uint8_t  power_off_key_count; //�ػ�����ʱ��
    uint8_t  cha_start_stop_flag;//FES Aͨ�� ��ͣ/��ʼ�̼���־
		uint8_t  chb_start_stop_flag;//FES Bͨ�� ��ͣ/��ʼ�̼���־
		uint8_t  cha_start_stop_status;//FES Aͨ�� �̼�ʱ����ͣ/��ʼ״̬
		uint8_t  chb_start_stop_status;//FES Bͨ�� �̼�ʱ����ͣ/��ʼ״̬
}GLOBAL_DATA;


#define  BC_TRUE   1
#define  BC_FALSE   0


#if defined(_BC_FES_)
/*----------------------------------------------------------------------
 Structure and enum           										
-----------------------------------------------------------------------*/
#if defined(_BC_FES_ACTION)

typedef struct
{
		uint8_t action_index;
		uint8_t action_cha_str;
		uint8_t action_chb_str;
		uint8_t g_action_flag;
    uint8_t g_action_start_flag;
}CONTI_ACTION;

#endif

/*----------------------------------------------------------------------
 Macros          										
-----------------------------------------------------------------------*/
//PWM ����
#define   VNS_PWM1_MODE_A_CYC    (400)

#define   FES_CH_A_UPPER_CYC     (290) //Ĭ����֫ģʽ,100*290us=29ms 
#define   FES_CH_B_UPPER_CYC     (290) //Ĭ����֫ģʽ,100*290us=29ms 
#define   FES_CH_A_SWALL_CYC     (170) //����ģʽ,100*170us=17ms 
#define   FES_CH_B_SWALL_CYC     (170) //����ģʽ,100*170us=17ms 




//�豸����
#define   BC_DEVICE_TYPE          DEVICE_FES

//����(΢��us)
#define   VNS_PULSE_WIDTH_MIN    (200)    
#define   VNS_PULSE_WIDTH_MAX    (1000)   
#define   VNS_PULSE_WIDTH_STEP   (50)   

//Ƶ��(Hz)
#define   VNS_FREQUENCY_MIN      (5)    
#define   VNS_FREQUENCY_MAX      (30)   
#define   VNS_FREQUENCY_STEP     (5)   

//ǿ��
#define   FES_STRENGTH_MIN              (0)    
#define   FES_STRENGTH_MAX              (60)
#define   FES_STRENGTH_MAX_OFFSET       (60)
#define   FES_LOWER_STRENGTH_MAX_OFFSET (20)
#define   FES_STRENGTH_STEP             (1)   


//�̼�ʱ��(s)
#define   VNS_START_TIME_MIN     (30)    
#define   VNS_START_TIME_MAX     (1800)   
#define   VNS_START_TIME_STEP    (30)   

//��ͣʱ��(s)
#define   VNS_STOP_TIME_MIN     (0)    
#define   VNS_STOP_TIME_MAX     (270)   
#define   VNS_STOP_TIME_STEP    (30)   


//��ʱ��(min)
#define   FES_TOTAL_TIME_MIN     (5)    
#define   FES_TOTAL_TIME_MAX     (30)   
#define   FES_TOTAL_TIME_STEP    (5)   

#else
/*----------------------------------------------------------------------
 Macros          										
-----------------------------------------------------------------------*/

//PWM ����
#define   VNS_PWM1_MODE_A_CYC    (400) //Ĭ��ģʽA,125*400us=50ms 
#define   VNS_PWM1_MODE_B_CYC    (320) //ģʽB,125*320us=40ms
#define   VNS_PWM1_MODE_C_CYC    (320) //ģʽC,125*320us=40ms

#define   VNS_MODE_C_STRENGTH    (5)


//�豸����
#define   BC_DEVICE_TYPE          DEVICE_VNS

//����(΢��us)
#define   VNS_PULSE_WIDTH_MIN    (200)    
#define   VNS_PULSE_WIDTH_MAX    (1000)   
#define   VNS_PULSE_WIDTH_STEP   (50)   

//Ƶ��(Hz)
#define   VNS_FREQUENCY_MIN      (5)    
#define   VNS_FREQUENCY_MAX      (30)   
#define   VNS_FREQUENCY_STEP     (5)  

//ǿ��
#define   VNS_STRENGTH_MIN         (0)    
#define   VNS_STRENGTH_MAX         (60)   
#define   VNS_STRENGTH_MAX_OFFSET  (20) 
#define   VNS_STRENGTH_STEP        (1)   


//�̼�ʱ��(s)
#define   VNS_START_TIME_MIN     (30)    
#define   VNS_START_TIME_MAX     (1800)   
#define   VNS_START_TIME_STEP    (30)   

//��ͣʱ��(s)
#define   VNS_STOP_TIME_MIN     (0)    
#define   VNS_STOP_TIME_MAX     (270)   
#define   VNS_STOP_TIME_STEP    (30)   


//��ʱ��(min)
#define   VNS_TOTAL_TIME_MIN     (5)    
#define   VNS_TOTAL_TIME_MAX     (30)   
#define   VNS_TOTAL_TIME_STEP    (5)   

#endif


//����λ�����ʹ�����������(s)
#define   HR_CYCLE  (5)




/*----------------------------------------------------------------------
 Extern           										
-----------------------------------------------------------------------*/
extern GLOBAL_DATA g_data;



/*----------------------------------------------------------------------
 Functions           										
-----------------------------------------------------------------------*/
void BC_task(void);
void BC_mcu_init(void);
void BC_countdown_time(void);
void BC_send_hr_cycle(void);
void BC_long_key_power_off(void);
void BC_VNS_set_stimulation_time(uint16_t total_time);
void BC_detecting_battery_voltage(void);
void BC_continuous_action_pwm_dac(uint16_t tim_count);
void BC_continuous_action_pwm_dac_half(uint16_t tim_count);
void BC_FES_key_back_chb_stimulate(void);
void BC_FES_key_back_cha_stimulate(void);


#endif

