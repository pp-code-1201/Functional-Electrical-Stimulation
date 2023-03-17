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
		PARA_OFF_ON,    //刺激开关
		PARA_MODE,      //刺激模式
		PARA_STRENGTH,  //刺激强度
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
    uint8_t is_pc;             //PC控制设备
    volatile uint8_t hr_cycle; //发送串口心跳周期
    uint16_t pwm1_cycle;        //pwm1周期
    uint16_t pwm2_cycle;        //pwm2周期
    uint8_t  start_stop_flag;   //暂停/开始刺激
    uint8_t  power_off_flag;    //关机标志
    uint8_t  power_off_key_count; //关机长按时间
    uint8_t  cha_start_stop_flag;//FES A通道 暂停/开始刺激标志
		uint8_t  chb_start_stop_flag;//FES B通道 暂停/开始刺激标志
		uint8_t  cha_start_stop_status;//FES A通道 刺激时间暂停/开始状态
		uint8_t  chb_start_stop_status;//FES B通道 刺激时间暂停/开始状态
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
//PWM 周期
#define   VNS_PWM1_MODE_A_CYC    (400)

#define   FES_CH_A_UPPER_CYC     (290) //默认上肢模式,100*290us=29ms 
#define   FES_CH_B_UPPER_CYC     (290) //默认上肢模式,100*290us=29ms 
#define   FES_CH_A_SWALL_CYC     (170) //吞咽模式,100*170us=17ms 
#define   FES_CH_B_SWALL_CYC     (170) //吞咽模式,100*170us=17ms 




//设备类型
#define   BC_DEVICE_TYPE          DEVICE_FES

//脉宽(微秒us)
#define   VNS_PULSE_WIDTH_MIN    (200)    
#define   VNS_PULSE_WIDTH_MAX    (1000)   
#define   VNS_PULSE_WIDTH_STEP   (50)   

//频率(Hz)
#define   VNS_FREQUENCY_MIN      (5)    
#define   VNS_FREQUENCY_MAX      (30)   
#define   VNS_FREQUENCY_STEP     (5)   

//强度
#define   FES_STRENGTH_MIN              (0)    
#define   FES_STRENGTH_MAX              (60)
#define   FES_STRENGTH_MAX_OFFSET       (60)
#define   FES_LOWER_STRENGTH_MAX_OFFSET (20)
#define   FES_STRENGTH_STEP             (1)   


//刺激时间(s)
#define   VNS_START_TIME_MIN     (30)    
#define   VNS_START_TIME_MAX     (1800)   
#define   VNS_START_TIME_STEP    (30)   

//暂停时间(s)
#define   VNS_STOP_TIME_MIN     (0)    
#define   VNS_STOP_TIME_MAX     (270)   
#define   VNS_STOP_TIME_STEP    (30)   


//总时长(min)
#define   FES_TOTAL_TIME_MIN     (5)    
#define   FES_TOTAL_TIME_MAX     (30)   
#define   FES_TOTAL_TIME_STEP    (5)   

#else
/*----------------------------------------------------------------------
 Macros          										
-----------------------------------------------------------------------*/

//PWM 周期
#define   VNS_PWM1_MODE_A_CYC    (400) //默认模式A,125*400us=50ms 
#define   VNS_PWM1_MODE_B_CYC    (320) //模式B,125*320us=40ms
#define   VNS_PWM1_MODE_C_CYC    (320) //模式C,125*320us=40ms

#define   VNS_MODE_C_STRENGTH    (5)


//设备类型
#define   BC_DEVICE_TYPE          DEVICE_VNS

//脉宽(微秒us)
#define   VNS_PULSE_WIDTH_MIN    (200)    
#define   VNS_PULSE_WIDTH_MAX    (1000)   
#define   VNS_PULSE_WIDTH_STEP   (50)   

//频率(Hz)
#define   VNS_FREQUENCY_MIN      (5)    
#define   VNS_FREQUENCY_MAX      (30)   
#define   VNS_FREQUENCY_STEP     (5)  

//强度
#define   VNS_STRENGTH_MIN         (0)    
#define   VNS_STRENGTH_MAX         (60)   
#define   VNS_STRENGTH_MAX_OFFSET  (20) 
#define   VNS_STRENGTH_STEP        (1)   


//刺激时间(s)
#define   VNS_START_TIME_MIN     (30)    
#define   VNS_START_TIME_MAX     (1800)   
#define   VNS_START_TIME_STEP    (30)   

//暂停时间(s)
#define   VNS_STOP_TIME_MIN     (0)    
#define   VNS_STOP_TIME_MAX     (270)   
#define   VNS_STOP_TIME_STEP    (30)   


//总时长(min)
#define   VNS_TOTAL_TIME_MIN     (5)    
#define   VNS_TOTAL_TIME_MAX     (30)   
#define   VNS_TOTAL_TIME_STEP    (5)   

#endif


//向上位机发送串口心跳周期(s)
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

