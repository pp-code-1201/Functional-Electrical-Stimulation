/******************************************************************************
  BC 
  project:VNS F103
 ******************************************************************************
  File Name     : app.c
  Version       : 
  Author        : Zengz
  Created       : 2022/12/12
  Description   : user app file
  Function List :
  History       :
 
******************************************************************************/

/*----------------------------------------------------------------------
 Include										
-----------------------------------------------------------------------*/
#include <string.h>
#include "app.h"
#include "main.h"
#include "dac.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "lcd.h"
#include "gui.h"
#include "usart.h"
#include "tim.h"
#include "sys.h"
#include "iwdg.h"



/*---------------------------------------------------------------------
Macros       				  				
-----------------------------------------------------------------------*/


/*---------------------------------------------------------------------
Global variables         				  				
-----------------------------------------------------------------------*/
#if defined(_BC_FES_)
uint8_t g_device_name[] = {'F','E','S','\0'};
uint8_t g_version[] = {0x01,0x04};

uint8_t g_device_id_default[6] = {0x02,0x22,0x11,0x08,0x00,0x01};

uint16_t g_total_time = 30;
uint16_t g_total1_time = 0;

uint16_t g_start_time = 30;
uint16_t g_stop_time = 30;

uint8_t g_start_falg = 0;
uint8_t g_pwm_off_falg = 0;


#else
uint8_t g_device_name[] = {'V','N','S','\0'};
uint8_t g_version[] = {0x01,0x03};

uint8_t g_device_id_default[6] = {0x01,0x22,0x11,0x08,0x00,0x01};

uint16_t g_total_time = 30;
uint16_t g_total1_time = 0;

uint16_t g_start_time = 30;
uint16_t g_stop_time = 30;

uint8_t g_start_falg = 0;
uint8_t g_pwm_off_falg = 0;


#endif



/*----------------------------------------------------------------------
 Structure and enum           										
-----------------------------------------------------------------------*/

GLOBAL_DATA g_data;

#if defined(_BC_FES_ACTION)
CONTI_ACTION g_action;
#endif


/*----------------------------------------------------------------------
 Extern           										
-----------------------------------------------------------------------*/
extern GUI_PARA gui_struct_para;
extern GUI_PARA gui_fes_struct_para;
extern u16 g_adcx;
/*----------------------------------------------------------------------
 Local functions           										
-----------------------------------------------------------------------*/
void BC_continuous_action(void);


/*****************************************************************************
  * Function:	  
  * 		 delay_us
  * Description: 
  * 		 延时nus
  * Parameters:  
  * 		 [nus]
  * Return: 	 
  * 		 void
*****************************************************************************/



/*****************************************************************************
  * Function:	  
  * 		 delay_ms
  * Description: 
  * 		 delay ms
  * Parameters:  
  * 		 [nms]
  * Return: 	 
  * 		 void
*****************************************************************************/



/*****************************************************************************
  * Function:	  
  * 		 SoftReset
  * Description: 
  * 		 MCU 软件重启
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void SoftReset(void)
{
    __set_FAULTMASK(1); // 关闭所有中断
    NVIC_SystemReset(); // 复位
}


/*****************************************************************************
  * Function:	  
  * 		 BC_VNS_led_init
  * Description: 
  * 		 water lamp
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_water_led_init(void)
{ 
		LED_BLUE_ON;
		HAL_Delay(200);
	  LED_YELLOW_ON;
		HAL_Delay(200);
	  LED_RED_ON;
		HAL_Delay(200);
		GUI_Display_Ui_Main_Menu();
		HAL_Delay(200);
		LED_BLUE_OFF;
		LED_YELLOW_OFF;
		LED_RED_OFF;
		HAL_Delay(200);
		LED_BLUE_ON;
}



/*****************************************************************************
  * Function:	  
  * 		 BC_crc_cal_value(uint8_t* data_value, uint8_t data_length)
  * Description: 
  * 		 CRC-16 / MODBUS 
  * Parameters:  
  * 		 [ptr count]
  * Return: 	 
  * 		 void
*****************************************************************************/
uint32_t BC_crc_cal_value(uint8_t* data_value, uint8_t data_length)
{
		uint8_t i = 0;
		uint16_t crc_value = 0xffff;
		
		while(data_length--) 
		{
				crc_value ^= *data_value++;
				for (i = 0; i < 8; i++)
				{
						if(crc_value & 0x0001)
						{
						  	crc_value = (crc_value >> 1) ^ 0xA001;
						}
						else
						{
							  crc_value = crc_value >> 1;
						}
				}
		}
		return(crc_value);
}


/*****************************************************************************
  * Function:BC_para_init
  * 		 
  * Description: Initialize all parameters
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_para_init(void)
{
#if defined(_BC_FES_)
    gui_fes_struct_para.cha_on_flag = 0;
		gui_fes_struct_para.chb_on_flag = 0;

    g_data.cha_start_stop_flag = 0;
		g_data.chb_start_stop_flag = 0;

		gui_fes_struct_para.ch_a_flag = CH_A_OFF; //默认A通道未选中
		gui_fes_struct_para.ch_b_flag = CH_B_OFF; //默认B通道未选中
    gui_fes_struct_para.selec_index = CH_FES_A;//默认高亮选中A通道
    gui_fes_struct_para.cha_mode_index = MODE_FES_UPPER; //默认A通道模式为上肢    
    gui_fes_struct_para.chb_mode_index = MODE_FES_UPPER; //默认B通道模式为上肢
		gui_fes_struct_para.cha_current_strength = 0;     

		gui_fes_struct_para.chab_stimulate_index = MODE_FES_STRENGTH_CHA; //默认双通道刺激界面高亮
		gui_fes_struct_para.chab_stimu_cha_strength_flag = 0; //默认双通道刺激界面高亮不选择
		gui_fes_struct_para.chab_stimu_cha_strength_on_flag = CH_B_OFF;
		gui_fes_struct_para.chab_stimu_chb_strength_flag = 0; //默认双通道刺激界面高亮不选择
		gui_fes_struct_para.chab_stimu_chb_strength_on_flag = CH_B_OFF;

		gui_fes_struct_para.stimu_time = FES_TOTAL_TIME_MIN;

#if defined(_BC_FES_ACTION)
		g_action.action_index = 0;
		g_action.g_action_flag = 1;
		g_action.g_action_start_flag = 0;
    g_action.action_cha_str = 0;
		g_action.action_chb_str = 0;
#endif
			
#else   
    g_data.start_stop_flag = 0;

    gui_struct_para.menu_mode_index = MODE_A; //默认模式A
		gui_struct_para.current_strength = 0;     

    gui_struct_para.us_val = 250;//PERIOD_VALUE;
    gui_struct_para.hz_val = 25;
		gui_struct_para.current_strength = 0;//默认DAC值为0
		gui_struct_para.start_time = 30; //30s
		gui_struct_para.stop_time = 30; //30s
		gui_struct_para.total_time = 30; //30min
		gui_struct_para.custom_index = CUSTOM_ID_PLUSE;
		gui_struct_para.ok_flag = 0;
#endif
}


/*****************************************************************************
  * Function:BC_device_init_check_voltage
  * 		 
  * Description: Device Initialize check voltage value
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_device_init_check_voltage(void)
{
    BC_detecting_battery_voltage();
		HAL_Delay(500);
		BC_detecting_battery_voltage();
		HAL_Delay(500);
		BC_detecting_battery_voltage();
}


/*****************************************************************************
  * Function:BC_mcu_init
  * 		 
  * Description: Initialize all configured peripherals
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_mcu_init(void)
{
		MX_GPIO_Init();
	  MX_ADC1_Init();
	  MX_DAC_Init();
	  MX_UART4_Init(); //语音模块
	  MX_USART1_UART_Init(); //PC
	  #ifdef __DEBUG__
	  MX_USART3_UART_Init(); //debug
	  #endif
	  MX_IWDG_Init();
	  MX_TIM2_Init();
	  MX_TIM5_Init();

		LCD_Init(); 
		
#if defined(_TEST_)
	  PWM1_GPIO1_SET;
		PWM1_GPIO2_RESET;
#else
    PWM1_GPIO1_RESET;
		PWM1_GPIO2_RESET;

		#if defined(_BC_FES_)
    SWITCH_CHA;
		#endif

		#if defined(_BC_FES_)
		PWM2_GPIO1_RESET;
		PWM2_GPIO2_RESET;
		
    Get_Adc_Battery_Val(ADC_CHANNEL_15);
		#endif
#endif

		BC_para_init();
		BC_DEBUG("\r\n[BC %s Version_%d_%d]-(" __DATE__ " - " __TIME__ ")\r\n",g_device_name,g_version[0],g_version[1]);  	

		BC_VNS_water_led_init();
		BC_device_init_check_voltage();
}


/*****************************************************************************
  * Function:BC_play_audio_1
  * 		 
  * Description: 播放语音1-开始刺激         
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_play_audio_1(void)
{
    uint8_t send_data[7] = {0x7E, 0x05, 0xA0, 0x00, 0x01, 0xA6, 0xEF};
    HAL_UART_Transmit(&huart4, (uint8_t *)send_data, 7, 10);	
		//BC_DEBUG("BC_play_audio_1\r\n");
}


/*****************************************************************************
  * Function:BC_play_audio_2
  * 		 
  * Description: 播放语音2-暂停刺激         
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_play_audio_2(void)
{
    uint8_t send_data[7] = {0x7E, 0x05, 0xA0, 0x00, 0x02, 0xA7, 0xEF};
    HAL_UART_Transmit(&huart4, (uint8_t *)send_data, 7, 10);	
}


/*****************************************************************************
  * Function:BC_play_audio_3
  * 		 
  * Description: 播放语音3-继续刺激         
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_play_audio_3(void)
{
    uint8_t send_data[7] = {0x7E, 0x05, 0xA0, 0x00, 0x03, 0xA8, 0xEF};
    HAL_UART_Transmit(&huart4, (uint8_t *)send_data, 7, 10);	
}


/*****************************************************************************
  * Function:BC_play_audio_4
  * 		 
  * Description: 播放语音4-结束刺激         
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_play_audio_4(void)
{
    uint8_t send_data[7] = {0x7E, 0x05, 0xA0, 0x00, 0x04, 0xA9, 0xEF};
    HAL_UART_Transmit(&huart4, (uint8_t *)send_data, 7, 10);	
}


/*****************************************************************************
  * Function:BC_detecting battery voltage
  * 		 
  * Description: 检测电压           
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_detecting_battery_voltage(void)
{
		Get_Adc_Battery_Val(ADC_CHANNEL_15);

		//if(MENU_ID_FES_MAIN == gui_fes_struct_para.menu_id)
		{
				GUI_Display_Ui_Battery_Level_Icon();
		}
		
		if(BATTERY_ICON_0 == Get_Adc_Battery_Poweroff_Status())
		{
		    BC_DEBUG("power off\r\n");
				POWER_OFF;
		}
}


/*****************************************************************************
  * Function:BC_charging_get_status
  * 		 
  * Description: 检测充电中状态           
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_charging_get_status(void)
{
		//Get_Adc_Charging_Val(ADC_CHANNEL_14);

		BC_DEBUG("charging = %d\r\n",CHARGING_STATUS);
		if(1 == CHARGING_STATUS)
		{
				GUI_Display_Ui_Charging_Battery_Icon();
		}
		else
		{
				//电池电量显示
		    GUI_Display_Ui_Battery_Level_Icon();
		}
}



/*****************************************************************************
  * Function:BC_long_key_power_off
  * 		 
  * Description: 长按关机           
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_long_key_power_off(void)
{
    static uint8_t power_on = 1;
		if(0 == HAL_GPIO_ReadPin(POWER_KEY_GPIO_Port, POWER_KEY_Pin))
		{
				g_data.power_off_key_count++;//关机按键计数
				//BC_DEBUG("power off key c = %d\r\n",g_data.power_off_key_count++);

				if(g_data.power_off_key_count > 1)
				{
				    if(power_on)
				    {
								POWER_ON;
								LCD_LED_SET;
								power_on = 0;
				    }
						else
						{
						    LCD_LED_CLR;	
								LED_BLUE_OFF;
				        LED_YELLOW_OFF;
				        LED_RED_OFF;
								POWER_OFF;
						}
				}
		}
}

/*****************************************************************************
  * Function:BC_check_response_data
  * 		 
  * Description: VNS respones check data 
  * 0x55 0xDA 0x01 0x00 0x03 0x01 0x00 0x00 0x00 0x00 0x00 0x01 0x00 0x48 0xC5	 
  * Parameters:  
  * 		 [uint8_t sn,uint8_t code]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_check_response_data(uint8_t sn,uint8_t code)
{
    uint8_t  send_data[30] = {0}; 
		uint8_t  i = 0;
		uint32_t crc_val = 0;
		uint8_t  device_type = BC_DEVICE_TYPE;

		send_data[0] = 0x55;
		send_data[1] = 0xDA;
		send_data[2] = device_type; //VNS
		send_data[3] = 0x00;
		send_data[4] = 0x09; //data size
		send_data[5] = sn; //sn
		send_data[6] = 0x00;
		send_data[7] = 0x00;
		send_data[8] = code; //[0x00 查询]  [0x01 心跳]

    for(i = 0; i < 6; i++)
    {
        send_data[9+i] = g_device_id_default[i];
		}
		
		send_data[15] = g_version[0]; //version
		send_data[16] = g_version[1];

		send_data[17] = Get_Adc_Electrode_Connect_Status(); //electrode connect status
		
    crc_val = BC_crc_cal_value(send_data,18);
		send_data[18] = (crc_val>>8)&0xFF;
		send_data[19] = (crc_val&0xFF);

		//HAL_UART_Transmit_IT(&huart1, send_data, 15);
		HAL_UART_Transmit(&huart1, (uint8_t *)send_data, 20, 10);
		
}


/*****************************************************************************
  * Function:BC_continuous_action_pwm_dac
  * 		 
  * Description:动作 PWM
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_continuous_action_pwm_dac(uint16_t tim_count)
{
#if defined(_BC_FES_ACTION)
    if(g_action.action_index)
    {
				if(g_action.g_action_flag)
				{
		        BC_DAC1_Set_Level(g_action.action_cha_str/tim_count);
				}
				else
				{
						BC_DAC1_Set_Level(g_action.action_chb_str/tim_count);
				}
    }
		else
#endif
		{
				if(CH_FES_A == gui_fes_struct_para.selec_index)
        {
            BC_DAC1_Set_Level(gui_fes_struct_para.cha_current_strength/tim_count);
        }
				else
				{
            BC_DAC1_Set_Level(gui_fes_struct_para.chb_current_strength/tim_count);
				}
		}
}


/*****************************************************************************
  * Function:BC_continuous_action_pwm_dac_half
  * 		 
  * Description:动作 PWM half
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_continuous_action_pwm_dac_half(uint16_t tim_count)
{
#if defined(_BC_FES_ACTION)
    if(g_action.action_index)
    {
				if(g_action.g_action_flag)
				{
		        BC_DAC1_Set_Level(g_action.action_cha_str/2/tim_count);
				}
				else
				{
						BC_DAC1_Set_Level(g_action.action_chb_str/2/tim_count);
				}
		}
		else
#endif
		{
        if(CH_FES_A == gui_fes_struct_para.selec_index)
		    {
		        BC_DAC1_Set_Level(gui_fes_struct_para.cha_current_strength/2/tim_count);
		    }
				else
				{
						BC_DAC1_Set_Level(gui_fes_struct_para.chb_current_strength/2/tim_count);
				}
		}
}


/*****************************************************************************
  * Function:BC_send_hr_cycle
  * 		 
  * Description:向上位机发送串口心跳周期
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_send_hr_cycle(void)
{
    static uint8_t count_sn= 0;
		static uint8_t dog_flag = 0;
		static uint8_t battery_flag = 0;

    //Feed the dog at 2 second intervals
    dog_flag = !dog_flag;
    if(dog_flag)
    {
        IWDG_Feed();
    }
		else
		{
		    //电池电压检测 1min检测1次
		    if(30 > battery_flag)
		    {
        		battery_flag++;
				}
				else
				{
				    BC_DEBUG("battery_flag = %d\r\n",battery_flag);
		    		BC_detecting_battery_voltage();
						battery_flag = 0;
				}
		}
 
		//电池充电状态检测
		//BC_charging_get_status();
		

		//电极连接状态检测		
		//Get_Adc_Val(ADC_CHANNEL_0);
		//Get_Adc_Val(ADC_CHANNEL_1);
    //BC_DEBUG("adc = %d\r\n",g_adcx);
    
		//长按关机检测
		BC_long_key_power_off();

	  //连续动作
	  #if defined(_BC_FES_ACTION)
	  BC_continuous_action();
		#endif
    
    	
    if(g_data.is_pc)
    {
        if(HR_CYCLE <= g_data.hr_cycle)
        {
            if(255 > count_sn)
            {
                count_sn++;
            }
						else
						{
                count_sn = 0;
						}
            //发心跳数据
            g_data.hr_cycle = 0; 
            BC_check_response_data(count_sn,0x01);          
				}
		}
		else
		{
        //BC_Timer4_Off();
		}
}



/*****************************************************************************
  * Function:BC_response_data
  * 		 
  * Description: VNS respones set data 
  * 0x55 0xDA 0x01 0x00 0x03 0x01 0x00 0x00 0x00 0x00 0x00 0x01 0x00 0x48 0xC5	 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_response_data(uint8_t *data)
{
    uint8_t  send_data[20] = {0};
		uint32_t crc_val = 0;
		uint8_t  device_type = BC_DEVICE_TYPE;

		send_data[0] = 0x55;
		send_data[1] = 0xDA;
    send_data[2] = device_type; //type
		send_data[3] = 0x00;
		send_data[4] = 0x01; //data size
		send_data[5] = data[5]; //sn
		send_data[6] = 0x00;
		send_data[7] = 0x00;
		send_data[8] = 0x00; //err code
	
    crc_val = BC_crc_cal_value(send_data,9);
		send_data[9] = (crc_val>>8)&0xFF;
		send_data[10] = (crc_val&0xFF);

		HAL_UART_Transmit(&huart1, (uint8_t *)send_data, 11, 10);	
}


#if defined(_BC_FES_)

#if defined(_BC_FES_ACTION)
/*****************************************************************************
  * Function:BC_continuous_actioan
  * 		 
  * Description:连续动作
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_continuous_action(void)
{
    static uint8_t count = 0;
		
		BC_DEBUG("cha_str = %d chb_str = %d\r\n",g_action.action_cha_str,g_action.action_chb_str);
		if(g_action.action_index)
		{
		    if(5 > count)
		    {
		        count++;
		    }
				else
				{
		        count = 0;
				}
				
				if(2 == count)
				{			
				    g_action.g_action_flag = 0;
						SWITCH_CHB;		
				}
				else if(4 == count)
				{
				    g_action.g_action_flag = 1;
						SWITCH_CHA;
						count = 0;
				}
		}
}
#endif

/*****************************************************************************
  * Function:BC_FES_pc_connect_after_process
  * 		 
  * Description:FES after pc connect process
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_pc_connect_after_process(void)
{
		//BC_VNS_mode_a_set_para();
																	
		//BC_Ch1_Pwm_Off();
		//BC_DAC1_Stop();

		//BC_Ch2_Pwm_Off();
		//BC_DAC2_Stop();
		
		//BC_Timer5_Off();	
		g_data.cha_start_stop_flag = 0;
    g_data.chb_start_stop_flag = 0;
}


/*****************************************************************************
  * Function:BC_FES_cha_stimulation_status
  * 		 
  * Description:设置通道A刺激时间的状态
  *
  * Parameters:  
  * 		 [1:开始刺激倒计时，0:结束刺激倒计时]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_cha_set_stimulation_time_status(uint8_t on_off)
{
		g_data.cha_start_stop_status = on_off;
}


/*****************************************************************************
  * Function:BC_FES_cha_get_stimulation_time_status
  * 		 
  * Description:获取通道A刺激时间的状态
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 1:开始刺激倒计时，0:结束刺激倒计时
*****************************************************************************/
uint8_t BC_FES_cha_get_stimulation_time_status(void)
{
		return g_data.cha_start_stop_status;
}


/*****************************************************************************
  * Function:BC_FES_cha_set_stimulation_time
  * 		 
  * Description:设置刺激时间长度
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_cha_set_stimulation_time(uint16_t total_time)
{
		gui_fes_struct_para.cha_time_min = total_time/60;
		gui_fes_struct_para.cha_time_sec = total_time%60;
}


/*****************************************************************************
  * Function:BC_FES_chb_stimulation_status
  * 		 
  * Description:设置通道B刺激时间的状态
  *
  * Parameters:  
  * 		 [1:开始刺激倒计时，0:结束刺激倒计时]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_chb_set_stimulation_time_status(uint8_t on_off)
{
		g_data.chb_start_stop_status = on_off;
}


/*****************************************************************************
  * Function:BC_FES_chb_get_stimulation_time_status
  * 		 
  * Description:获取通道B刺激时间的状态
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 1:开始刺激倒计时，0:结束刺激倒计时
*****************************************************************************/
uint8_t BC_FES_chb_get_stimulation_time_status(void)
{
		return g_data.chb_start_stop_status;
}


/*****************************************************************************
  * Function:BC_FES_chb_set_stimulation_time
  * 		 
  * Description:设置刺激时间长度
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_chb_set_stimulation_time(uint16_t total_time)
{
		gui_fes_struct_para.chb_time_min = total_time/60;
		gui_fes_struct_para.chb_time_sec = total_time%60;
}


/*****************************************************************************
  * Function:BC_FES_custom_time_do
  * 		 
  * Description:设置自定义刺激时间，休息时间，总时间
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_custom_time_do(void)
{

}


/*****************************************************************************
  * Function:BC_FES_cha_countdown_time
  * 		 
  * Description:FES 通道A 定时器倒计时
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_cha_countdown_time(void)
{
		if(0 == gui_fes_struct_para.cha_time_sec--)
    {
        if(gui_fes_struct_para.cha_time_min > 0)
        {
            gui_fes_struct_para.cha_time_min--;
						gui_fes_struct_para.cha_time_sec = 59;
    	  }
				else
				{
            if(0 == gui_fes_struct_para.cha_time_min)
            {
                //关闭定时器
                if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHA_CHB)
                {
	                if(((FES_TOTAL_TIME_MAX == gui_fes_struct_para.chb_time_min) || (0 == gui_fes_struct_para.chb_time_min)) && (0 == gui_fes_struct_para.chb_time_sec))
	                {
							        BC_Fes_Ch_Timer_Off();
											BC_play_audio_4();
	                }
                }
								else
								{
										BC_Fes_Ch_Timer_Off();		
										BC_play_audio_4();
								}
								BC_Ch1_Pwm_Off(); 
								BC_FES_cha_set_stimulation_time(0);
								BC_FES_cha_set_stimulation_time_status(BC_FALSE);
								g_data.cha_start_stop_flag = 0;
								BC_FES_key_back_cha_stimulate();
						}
				}
		}


    if(BC_TRUE == BC_FES_cha_get_stimulation_time_status())
    {
		    if(MODE_FES_STOP_START_CHA == gui_fes_struct_para.chab_stimulate_index)
		    {			
						GUI_Display_Countdown_Time_Ex(56,188,gui_fes_struct_para.cha_time_min,gui_fes_struct_para.cha_time_sec);
				}
				else
				{
				    if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHA)
				    {
								GUI_Display_Countdown_Time(140,188,gui_fes_struct_para.cha_time_min,gui_fes_struct_para.cha_time_sec);
						}
						else
						{
								GUI_Display_Countdown_Time(56,188,gui_fes_struct_para.cha_time_min,gui_fes_struct_para.cha_time_sec);
						}
				}		
    }
}


/*****************************************************************************
  * Function:BC_FES_chb_countdown_time
  * 		 
  * Description:FES 通道B 定时器倒计时
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_chb_countdown_time(void)
{
		if(0 == gui_fes_struct_para.chb_time_sec--)
    {
        if(gui_fes_struct_para.chb_time_min > 0)
        {
            gui_fes_struct_para.chb_time_min--;
						gui_fes_struct_para.chb_time_sec = 59;
    	  }
				else
				{
            if(0 == gui_fes_struct_para.chb_time_min)
            {    
                //关闭定时器
                if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHA_CHB)
                {
		                if(((FES_TOTAL_TIME_MAX == gui_fes_struct_para.cha_time_min) || (0 == gui_fes_struct_para.cha_time_min)) && (0 == gui_fes_struct_para.cha_time_sec))
		                {
								        BC_Fes_Ch_Timer_Off();
												BC_play_audio_4();
		                }
                }
								else
								{
										 BC_Fes_Ch_Timer_Off();
										 BC_play_audio_4();
								}
								#if defined(_BC_FES_SWITCH)	
								BC_Ch1_Pwm_Off(); 
								#else
								BC_Ch2_Pwm_Off(); 
								#endif
								BC_FES_chb_set_stimulation_time(0);
								BC_FES_chb_set_stimulation_time_status(BC_FALSE);
								g_data.chb_start_stop_flag = 0;
								BC_FES_key_back_chb_stimulate();
						}
				}
		}

    //BC_VNS_custom_time_do();
    if(BC_TRUE == BC_FES_chb_get_stimulation_time_status())
    {
				if(MODE_FES_STOP_START_CHB == gui_fes_struct_para.chab_stimulate_index)
				{
						GUI_Display_Countdown_Time_Ex(216,188,gui_fes_struct_para.chb_time_min,gui_fes_struct_para.chb_time_sec);
				}
				else
				{
				    if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHB)
		    		{	
		    				GUI_Display_Countdown_Time(140,188,gui_fes_struct_para.chb_time_min,gui_fes_struct_para.chb_time_sec);
				    }
						else
						{
								GUI_Display_Countdown_Time(216,188,gui_fes_struct_para.chb_time_min,gui_fes_struct_para.chb_time_sec);
						}
				}
    }
}



/*****************************************************************************
  * Function:BC_FES_countdown_time
  * 		 
  * Description:FES 定时器倒计时
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_countdown_time(void)
{
    static uint8_t cha_on = 0;
		static uint8_t chb_on = 0;

    BC_DEBUG("A min=%d sec=%d\r\n",gui_fes_struct_para.cha_time_min,gui_fes_struct_para.cha_time_sec);
		BC_DEBUG("B min=%d sec=%d\r\n",gui_fes_struct_para.chb_time_min,gui_fes_struct_para.chb_time_sec);

		if(BC_TRUE == BC_FES_cha_get_stimulation_time_status())
		{
				cha_on = !cha_on;
				if(gui_fes_struct_para.cha_current_strength > 0)
				{
						if(cha_on)
						{
								BC_Ch1_Pwm_On(); 
						}
						else
						{
		        		BC_Ch1_Pwm_Off();
						}	
				}
				else
				{
						BC_Ch1_Pwm_Off();
				}

				BC_FES_cha_countdown_time();
		}
		
		if(BC_TRUE == BC_FES_chb_get_stimulation_time_status())
		{	
			  chb_on = !chb_on;
				#if defined(_BC_FES_SWITCH)	
        if(gui_fes_struct_para.chb_current_strength > 0)
				#else
				if(gui_fes_struct_para.chb_current_strength > 0)
				#endif
				{
					  if(chb_on)
						{
						    #if defined(_BC_FES_SWITCH)	
								BC_Ch1_Pwm_On(); 
								#else
								BC_Ch2_Pwm_On(); 
								#endif
						}
						else
						{
						    #if defined(_BC_FES_SWITCH)	
		        		BC_Ch1_Pwm_Off();
								#else
		        		BC_Ch2_Pwm_Off();
								#endif
						}
				}
				else
				{
						#if defined(_BC_FES_SWITCH)	
        		BC_Ch1_Pwm_Off();
						#else
        		BC_Ch2_Pwm_Off();
						#endif
				}
				
				BC_FES_chb_countdown_time();
		}   	  
}


/*****************************************************************************
  * Function:BC_FES_cha_restart_countdown_time
  * 		 
  * Description:FES 通道A 刺激界面定时器倒计时重新开始
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_cha_restart_countdown_time(void)
{
		if((0 == gui_fes_struct_para.cha_time_sec) && (0 == gui_fes_struct_para.cha_time_sec))
		{
				BC_FES_cha_set_stimulation_time(gui_fes_struct_para.stimu_time*60);
				BC_Fes_Ch_Timer_On();
		}
}


/*****************************************************************************
  * Function:BC_FES_chb_restart_countdown_time
  * 		 
  * Description:FES 通道B 刺激界面定时器倒计时重新开始
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_chb_restart_countdown_time(void)
{
		if((0 == gui_fes_struct_para.chb_time_sec) && (0 == gui_fes_struct_para.chb_time_sec))
		{
				BC_FES_chb_set_stimulation_time(gui_fes_struct_para.stimu_time*60);
				BC_Fes_Ch_Timer_On();
		}
}


/*****************************************************************************
  * Function:BC_FES_cha_upper_set_para
  * 		 
  * Description:设置CH_A 上肢模式参数
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_cha_upper_set_para(void)
{
    g_data.pwm1_cycle = FES_CH_A_UPPER_CYC;
    MX_TIM3_Init(PERIOD_VALUE);

    if(!g_data.is_pc)
    {			
				BC_FES_cha_set_stimulation_time(gui_fes_struct_para.stimu_time*60);
				//BC_Fes_Ch_Timer_On();
				BC_FES_cha_set_stimulation_time_status(BC_FALSE);
    }
}


/*****************************************************************************
  * Function:BC_FES_cha_swallow_set_para
  * 		 
  * Description:设置CH_A 吞咽模式参数
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_cha_swallow_set_para(void)
{
    g_data.pwm1_cycle = FES_CH_A_SWALL_CYC;
    MX_TIM3_Init(PERIOD_VALUE);

    if(!g_data.is_pc)
    {			
				BC_FES_cha_set_stimulation_time(gui_fes_struct_para.stimu_time*60);
				//BC_Fes_Ch_Timer_On();
				BC_FES_cha_set_stimulation_time_status(BC_FALSE);
    }
}


/*****************************************************************************
  * Function:BC_FES_chb_upper_set_para
  * 		 
  * Description:设置CH_B 上肢模式参数
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_chb_upper_set_para(void)
{
		#if defined(_BC_FES_SWITCH)
		g_data.pwm1_cycle = FES_CH_B_UPPER_CYC;
    MX_TIM3_Init(PERIOD_VALUE);
		#else
		g_data.pwm2_cycle = FES_CH_B_UPPER_CYC;
    MX_TIM4_Init(PERIOD_VALUE);
		#endif

		if(!g_data.is_pc)
    {			
				BC_FES_chb_set_stimulation_time(gui_fes_struct_para.stimu_time*60);
				//BC_Fes_Ch_Timer_On();
				BC_FES_chb_set_stimulation_time_status(BC_FALSE);
    }
}


/*****************************************************************************
  * Function:BC_FES_chb_swallow_set_para
  * 		 
  * Description:设置CH_B 吞咽模式参数
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_chb_swallow_set_para(void)
{
		#if defined(_BC_FES_SWITCH)
		g_data.pwm1_cycle = FES_CH_B_SWALL_CYC;
    MX_TIM3_Init(PERIOD_VALUE);
		#else
		g_data.pwm2_cycle = FES_CH_B_SWALL_CYC;
    MX_TIM4_Init(PERIOD_VALUE);
		#endif

		if(!g_data.is_pc)
    {			
				BC_FES_chb_set_stimulation_time(gui_fes_struct_para.stimu_time*60);
				//BC_Fes_Ch_Timer_On();
				BC_FES_chb_set_stimulation_time_status(BC_FALSE);
    }
}


/*****************************************************************************
  * Function:BC_FES_key_down_cha_stimulate
  * 		 
  * Description:key down channel a stimulate
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_key_down_channel_a_stimulate(void)
{
		if((FES_STRENGTH_MAX > gui_fes_struct_para.cha_current_strength) && (FES_STRENGTH_MIN <= gui_fes_struct_para.cha_current_strength))
		{
				gui_fes_struct_para.cha_current_strength++;
						
				if(MODE_FES_UPPER == gui_fes_struct_para.cha_mode_index)
				{
						g_action.action_cha_str = gui_fes_struct_para.cha_current_strength;
				}
				
				if(1 == gui_fes_struct_para.cha_current_strength)
				{
						BC_DAC1_Start();
						#if !defined(_TEST_)
						BC_Ch1_Pwm_On();
						#endif
						BC_Fes_Ch_Timer_On();
				}
				BC_DAC1_Set_Level(gui_fes_struct_para.cha_current_strength);
				
				GUI_FES_DrawImageNum_Adjust(gui_fes_struct_para.cha_current_strength,0,0);
				
				BC_DEBUG("down cha current_strength = %d\r\n", gui_fes_struct_para.cha_current_strength);
		}
}


/*****************************************************************************
  * Function:BC_FES_key_up_cha_stimulate
  * 		 
  * Description:key up channel a stimulate
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_key_up_channel_a_stimulate(void)
{
		if((FES_STRENGTH_MAX >= gui_fes_struct_para.cha_current_strength) &&
					 (FES_STRENGTH_MIN < gui_fes_struct_para.cha_current_strength))
    {
        gui_fes_struct_para.cha_current_strength--;
				
				if(MODE_FES_UPPER == gui_fes_struct_para.cha_mode_index)
				{
						g_action.action_cha_str = gui_fes_struct_para.cha_current_strength;
				}
				
				BC_DAC1_Set_Level(gui_fes_struct_para.cha_current_strength); 
				if(0 == gui_fes_struct_para.cha_current_strength)
				{
            BC_DAC1_Stop();
						#if !defined(_TEST_)
					  BC_Ch1_Pwm_Off();
						#endif
				    BC_Fes_Ch_Timer_Off();
				}
				
				GUI_FES_DrawImageNum_Adjust(gui_fes_struct_para.cha_current_strength,0,0);

				BC_DEBUG("up cha current_strength = %d\r\n", gui_fes_struct_para.cha_current_strength);
	  }
}




/*****************************************************************************
  * Function:BC_FES_key_down_chb_stimulate
  * 		 
  * Description:key down channel b stimulate
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_key_down_channel_b_stimulate(void)
{
		if((FES_STRENGTH_MAX > gui_fes_struct_para.chb_current_strength) && (FES_STRENGTH_MIN <= gui_fes_struct_para.chb_current_strength))
		{
				gui_fes_struct_para.chb_current_strength++;
				
				if(MODE_FES_UPPER == gui_fes_struct_para.chb_mode_index)
				{
						g_action.action_chb_str = gui_fes_struct_para.chb_current_strength;
				}
				
				if(1 == gui_fes_struct_para.chb_current_strength)
				{
						BC_DAC1_Start();
						#if !defined(_TEST_)
						BC_Ch1_Pwm_On();
						#endif
						BC_Fes_Ch_Timer_On();
				}
				BC_DAC1_Set_Level(gui_fes_struct_para.chb_current_strength);
				
				GUI_FES_DrawImageNum_Adjust(gui_fes_struct_para.chb_current_strength,0,0);
				
				BC_DEBUG("down chb current_strength = %d\r\n", gui_fes_struct_para.chb_current_strength);
		}
}


/*****************************************************************************
  * Function:BC_FES_key_up_chb_stimulate
  * 		 
  * Description:key up channel b stimulate
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_key_up_channel_b_stimulate(void)
{
		if((FES_STRENGTH_MAX >= gui_fes_struct_para.chb_current_strength) &&
					 (FES_STRENGTH_MIN < gui_fes_struct_para.chb_current_strength))
    {
        gui_fes_struct_para.chb_current_strength--;
							
				if(MODE_FES_UPPER == gui_fes_struct_para.chb_mode_index)
				{
						g_action.action_chb_str = gui_fes_struct_para.chb_current_strength;
				}
				
				BC_DAC1_Set_Level(gui_fes_struct_para.chb_current_strength); 
				if(0 == gui_fes_struct_para.chb_current_strength)
				{
            BC_DAC1_Stop();
						#if !defined(_TEST_)
					  BC_Ch1_Pwm_Off();
						#endif
				    BC_Fes_Ch_Timer_Off();
				}
				
				GUI_FES_DrawImageNum_Adjust(gui_fes_struct_para.chb_current_strength,0,0);

				BC_DEBUG("up chb current_strength = %d\r\n", gui_fes_struct_para.chb_current_strength);
	  }
}


/*****************************************************************************
  * Function:BC_FES_key_back_cha_stimulate
  * 		 
  * Description:key back channel a stimulate
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_key_back_cha_stimulate(void)
{
		gui_fes_struct_para.cha_current_strength = 0;
		gui_fes_struct_para.cha_mode_index = MODE_FES_UPPER;
		gui_fes_struct_para.stimu_time = FES_TOTAL_TIME_MIN;
		
		BC_Ch1_Pwm_Off();
		BC_DAC1_Stop();
		
		BC_Fes_Ch_Timer_Off();
		BC_play_audio_4();
		BC_FES_cha_set_stimulation_time_status(BC_FALSE);
		GUI_Display_Ui_Main_Menu();	
}


/*****************************************************************************
  * Function:BC_FES_key_back_chb_stimulate
  * 		 
  * Description:key back channel b stimulate
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_key_back_chb_stimulate(void)
{
    gui_fes_struct_para.chb_current_strength = 0;
		gui_fes_struct_para.chb_mode_index = MODE_FES_UPPER;
#if defined(_BC_FES_SWITCH)	
		BC_FES_key_back_cha_stimulate();
    BC_FES_chb_set_stimulation_time_status(BC_FALSE);	
#else
    BC_Ch2_Pwm_Off();
		BC_DAC2_Stop();

		BC_Fes_Ch_Timer_Off();
		BC_play_audio_4();
		BC_FES_chb_set_stimulation_time_status(BC_FALSE);		
		GUI_Display_Ui_Main_Menu();
#endif	
}


/*****************************************************************************
  * Function:BC_FES_key_ok_select_ch
  * 		 
  * Description:key ok channel select
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_key_ok_select_ch(void)
{
		if(CH_FES_A == gui_fes_struct_para.selec_index)
		{
		    gui_fes_struct_para.cha_on_flag = !gui_fes_struct_para.cha_on_flag;
		    if(gui_fes_struct_para.cha_on_flag)
		    {
						gui_fes_struct_para.ch_a_flag = CH_A_ON;
		    }
				else
				{
            gui_fes_struct_para.ch_a_flag = CH_A_OFF;
				}
		}
		else if(CH_FES_B == gui_fes_struct_para.selec_index)
		{
		    gui_fes_struct_para.chb_on_flag = !gui_fes_struct_para.chb_on_flag;
		    if(gui_fes_struct_para.chb_on_flag)
		    {
						gui_fes_struct_para.ch_b_flag = CH_B_ON;
		    }
				else
				{
            gui_fes_struct_para.ch_b_flag = CH_B_OFF;
				}
		}

		GUI_FES_Display_Ui_Main_Menu_Ch_Selec(gui_fes_struct_para.selec_index,gui_fes_struct_para.ch_a_flag,gui_fes_struct_para.ch_b_flag);
		BC_DEBUG("selec_index=%d ch_a_flag=%d ch_b_flag=%d\r\n",gui_fes_struct_para.selec_index,gui_fes_struct_para.ch_a_flag,gui_fes_struct_para.ch_b_flag);

		if(CH_FES_OK == gui_fes_struct_para.selec_index)
		{
		    if(((CH_A_ON == gui_fes_struct_para.ch_a_flag) && (CH_B_OFF == gui_fes_struct_para.ch_b_flag)) ||
					 ((CH_A_OFF == gui_fes_struct_para.ch_a_flag) && (CH_B_ON == gui_fes_struct_para.ch_b_flag)))
		    {
						GUI_FES_Display_Ui_Mode_Selec(gui_fes_struct_para.ch_a_flag,gui_fes_struct_para.ch_b_flag,gui_fes_struct_para.cha_mode_index,0);
		    }
		}
}


/*****************************************************************************
  * Function:BC_FES_key_ok_stimulate_cha_entery_screen
  * 		 
  * Description:key ok entry channel a screen
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_key_ok_stimulate_cha_entery_screen(void)
{
#if defined(_BC_FES_SWITCH)
    if(1)
#else
		if((CH_A_ON == gui_fes_struct_para.ch_a_flag) && (CH_B_OFF == gui_fes_struct_para.ch_b_flag))
#endif
		{		    
				if(MODE_FES_UPPER == gui_fes_struct_para.cha_mode_index)//A通道 上肢参数
				{
				    BC_FES_cha_upper_set_para();
				}
				else if(MODE_FES_LOWER == gui_fes_struct_para.cha_mode_index)
				{
						BC_FES_cha_upper_set_para();
				}
				/*else if(MODE_FES_SWALLOW == gui_fes_struct_para.cha_mode_index)
				{
						BC_FES_cha_swallow_set_para();
				}
				else*/
				{
						
				}
				#if defined(_BC_FES_SWITCH)
		    SWITCH_CHA;
				GUI_FES_Display_Ui_Main_Ch_Stimulate_Switch(gui_fes_struct_para.selec_index);
				BC_play_audio_1();
				#else
				GUI_FES_Display_Ui_Main_Ch_Stimulate(gui_fes_struct_para.ch_a_flag,gui_fes_struct_para.ch_b_flag,
			                                  gui_fes_struct_para.cha_mode_index, gui_fes_struct_para.chb_mode_index,gui_fes_struct_para.chab_stimulate_index);
				#endif	
		}
		else if((CH_A_ON == gui_fes_struct_para.ch_a_flag) && (CH_B_ON == gui_fes_struct_para.ch_b_flag))
		{
				GUI_FES_Display_Ui_Mode_Selec(gui_fes_struct_para.ch_a_flag,gui_fes_struct_para.ch_b_flag,gui_fes_struct_para.cha_mode_index,1);
		}   
}


/*****************************************************************************
  * Function:BC_FES_key_ok_stimulate_chb_entery_screen
  * 		 
  * Description:key ok entry channel b screen
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_key_ok_stimulate_chb_entery_screen(void)
{
BC_DEBUG("MODE_CHB selec_index=%d ch_a_flag=%d ch_b_flag=%d\r\n",gui_fes_struct_para.selec_index,gui_fes_struct_para.ch_a_flag,gui_fes_struct_para.ch_b_flag);
#if defined(_BC_FES_SWITCH)
    if(1)
#else
		if((CH_A_OFF == gui_fes_struct_para.ch_a_flag) && (CH_B_ON == gui_fes_struct_para.ch_b_flag))
#endif
		{
		    if(MODE_FES_UPPER == gui_fes_struct_para.chb_mode_index)//A通道 上肢参数
				{
				    BC_FES_chb_upper_set_para();
				}
				else if(MODE_FES_LOWER == gui_fes_struct_para.chb_mode_index)
				{
						BC_FES_chb_upper_set_para();
				}
				else if(MODE_FES_SWALLOW == gui_fes_struct_para.chb_mode_index)
				{
						BC_FES_chb_swallow_set_para();
				}
				else
				{
						
				}
				#if defined(_BC_FES_SWITCH)
		    SWITCH_CHB;
		    GUI_FES_Display_Ui_Main_Ch_Stimulate_Switch(gui_fes_struct_para.selec_index);
				BC_play_audio_1();
				#else
				GUI_FES_Display_Ui_Main_Ch_Stimulate(gui_fes_struct_para.ch_a_flag,gui_fes_struct_para.ch_b_flag,
			                                  gui_fes_struct_para.cha_mode_index, gui_fes_struct_para.chb_mode_index,gui_fes_struct_para.chab_stimulate_index);
				#endif
		}
		else if((CH_A_ON == gui_fes_struct_para.ch_a_flag) && (CH_B_ON == gui_fes_struct_para.ch_b_flag))
		{
				BC_FES_cha_upper_set_para();
				BC_FES_chb_upper_set_para();

				//进入双通道刺激界面
				GUI_FES_Display_Ui_Main_Ch_Stimulate(gui_fes_struct_para.ch_a_flag,gui_fes_struct_para.ch_b_flag,
			                                  gui_fes_struct_para.cha_mode_index, gui_fes_struct_para.chb_mode_index,gui_fes_struct_para.chab_stimulate_index);
		}
}


/*****************************************************************************
  * Function:BC_FES_stimu_combina
  * 		 
  * Description:FES Combined movements switch gui
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_stimu_combina(void)
{
    GUI_FES_Display_Ui_Stimu_Combina(g_action.g_action_start_flag);
}


/*****************************************************************************
  * Function:BC_FES_gui_key_ok_process
  * 		 
  * Description:FES ok key process
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_gui_key_ok_process(void)
{
    Get_Adc_Battery_Val(ADC_CHANNEL_15);
		GUI_Display_Ui_Battery_Level_Icon();
		
		if(gui_fes_struct_para.menu_id == MENU_ID_FES_MAIN)
		{	 
		#if defined(_BC_FES_SWITCH)
				if(CH_FES_A == gui_fes_struct_para.selec_index)
				{
						GUI_FES_Display_Ui_Mode_Selec_Switch(gui_fes_struct_para.selec_index,gui_fes_struct_para.cha_mode_index);
				}
				else if(CH_FES_B == gui_fes_struct_para.selec_index)
				{
						GUI_FES_Display_Ui_Mode_Selec_Switch(gui_fes_struct_para.selec_index,gui_fes_struct_para.chb_mode_index);
				}
		#else		
				BC_FES_key_ok_select_ch();
		#endif
		}
    else if(gui_fes_struct_para.menu_id == MENU_ID_FES_MODE_CHA)
    {
        if(MODE_FES_CUSTOM > gui_fes_struct_para.cha_mode_index)
        {
            if(MODE_FES_SWALLOW == gui_fes_struct_para.cha_mode_index)
            {
                //BC_DAC1_Stop();
                BC_DAC1_Start();
            		GUI_FES_Display_Ui_Stimu_Combina_Main(0);
            }
						else
						{
    		        GUI_FES_Display_Ui_Stimu_Time(gui_fes_struct_para.selec_index,gui_fes_struct_para.stimu_time);
						}
        }
    }
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_MODE_CHB)
		{
		    if(MODE_FES_CUSTOM > gui_fes_struct_para.chb_mode_index)
		    {
						GUI_FES_Display_Ui_Stimu_Time(gui_fes_struct_para.selec_index,gui_fes_struct_para.stimu_time);
		    }
		}
#if defined(_BC_FES_ACTION)
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_MODE_COMBINA)
		{
        if(MODE_FES_SWALLOW == gui_fes_struct_para.cha_mode_index)
        {
            if((g_action.action_cha_str > 0) && (g_action.action_chb_str > 0))
            {
								g_action.g_action_start_flag = !g_action.g_action_start_flag;
								if(g_action.g_action_start_flag)
								{
								    g_action.action_index = 1;
								    BC_Ch1_Pwm_On();	
										BC_play_audio_1();
								}
								else
								{
								    g_action.action_index = 0;
		                BC_Ch1_Pwm_Off();
										BC_play_audio_2();
								}
								BC_FES_stimu_combina();	
            }
				}		
				
		}
#endif
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_TIME)
		{	
				if(CH_FES_A == gui_fes_struct_para.selec_index)
				{
						BC_FES_key_ok_stimulate_cha_entery_screen();
				}
				else if(CH_FES_B == gui_fes_struct_para.selec_index)
				{
						BC_FES_key_ok_stimulate_chb_entery_screen();
				}
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHA)
    {
				if(0 < gui_fes_struct_para.cha_current_strength)
				{
						g_data.cha_start_stop_flag = !g_data.cha_start_stop_flag;	
						if(g_data.cha_start_stop_flag)
						{
						    BC_FES_cha_restart_countdown_time();
								#if !defined(_TEST_)
								BC_Ch1_Pwm_On();
								#endif
								BC_play_audio_3();
								BC_FES_cha_set_stimulation_time_status(BC_TRUE);
						}
						else
						{
		 						BC_Ch1_Pwm_Off();
								BC_play_audio_2();
								BC_FES_cha_set_stimulation_time_status(BC_FALSE);
						}
				}
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHB)
    {
    #if defined(_BC_FES_SWITCH)	
				if(0 < gui_fes_struct_para.chb_current_strength)
		#else
		    if(0 < gui_fes_struct_para.chb_current_strength)
		#endif
				{
						g_data.chb_start_stop_flag = !g_data.chb_start_stop_flag;	
						if(g_data.chb_start_stop_flag)
						{
						    BC_FES_chb_restart_countdown_time();
								#if defined(_BC_FES_SWITCH)	
								BC_Ch1_Pwm_On();
								#else
								BC_Ch2_Pwm_On();
								#endif
								BC_play_audio_3();
								BC_FES_chb_set_stimulation_time_status(BC_TRUE);
						}
						else
						{
						    #if defined(_BC_FES_SWITCH)	
								BC_Ch1_Pwm_Off();
								#else
		 						BC_Ch2_Pwm_Off();
								#endif
								BC_play_audio_2();
								BC_FES_chb_set_stimulation_time_status(BC_FALSE);
						}
				}
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHA_CHB)
		{
				if(MODE_FES_STRENGTH_CHA == gui_fes_struct_para.chab_stimulate_index)
				{
						gui_fes_struct_para.chab_stimu_cha_strength_flag = !gui_fes_struct_para.chab_stimu_cha_strength_flag;
						if(gui_fes_struct_para.chab_stimu_cha_strength_flag)
						{
								gui_fes_struct_para.chab_stimu_cha_strength_on_flag = CH_A_ON;
						}
						else
						{
								gui_fes_struct_para.chab_stimu_cha_strength_on_flag = CH_A_OFF;
						}			
				}
				else if(MODE_FES_STOP_START_CHA == gui_fes_struct_para.chab_stimulate_index)
				{
						g_data.cha_start_stop_flag = !g_data.cha_start_stop_flag;		
			
						if(g_data.cha_start_stop_flag)
						{			    
								BC_FES_cha_restart_countdown_time();
								BC_Ch1_Pwm_On();
								BC_play_audio_3();
								BC_FES_cha_set_stimulation_time_status(BC_TRUE);
						}
						else
						{
		 						BC_Ch1_Pwm_Off();
								BC_play_audio_2();
								BC_FES_cha_set_stimulation_time_status(BC_FALSE);
						}
				}
				else if(MODE_FES_STRENGTH_CHB == gui_fes_struct_para.chab_stimulate_index)
				{
						gui_fes_struct_para.chab_stimu_chb_strength_flag = !gui_fes_struct_para.chab_stimu_chb_strength_flag;
						if(gui_fes_struct_para.chab_stimu_chb_strength_flag)
						{
								gui_fes_struct_para.chab_stimu_chb_strength_on_flag = CH_B_ON;
						}
						else
						{
								gui_fes_struct_para.chab_stimu_chb_strength_on_flag = CH_B_OFF;
						}			
				}
				else if(MODE_FES_STOP_START_CHB == gui_fes_struct_para.chab_stimulate_index)
				{
						g_data.chb_start_stop_flag = !g_data.chb_start_stop_flag;		
			
						if(g_data.chb_start_stop_flag)
						{   
								BC_FES_chb_restart_countdown_time();
								BC_Ch2_Pwm_On();
								BC_play_audio_3();
								BC_FES_chb_set_stimulation_time_status(BC_TRUE);
						}
						else
						{
		 						BC_Ch2_Pwm_Off();
								BC_play_audio_2();
								BC_FES_chb_set_stimulation_time_status(BC_FALSE);
						}
				}

				GUI_FES_Display_Ui_Main_Cha_Chb_Stimulate_Select(gui_fes_struct_para.chab_stimulate_index);
		}
				
}


/*****************************************************************************
  * Function:BC_FES_gui_key_back_process
  * 		 
  * Description:FES back key process
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_gui_key_back_process(void)
{
    g_data.cha_start_stop_flag = 0;
    g_data.chb_start_stop_flag = 0;
		
		if((gui_fes_struct_para.menu_id == MENU_ID_FES_MODE_CHA) || (gui_fes_struct_para.menu_id == MENU_ID_FES_MODE_CHB))
		{  		
    #if defined(_BC_FES_ACTION)
		    g_action.action_cha_str = 0;
				g_action.action_chb_str = 0;
    #endif
				GUI_Display_Ui_Main_Menu();				
		}
#if defined(_BC_FES_ACTION)
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_MODE_COMBINA)
		{
    		g_action.action_index = 0;
				g_action.g_action_flag = 0;
				g_action.g_action_start_flag = 0;
				gui_fes_struct_para.cha_mode_index = MODE_FES_UPPER;
				
				SWITCH_CHA;
				
				if((g_action.action_cha_str > 0) && (g_action.action_chb_str > 0))
				{
				    BC_Ch1_Pwm_Off();
						BC_play_audio_4();
				}
				BC_DAC1_Stop();				
				GUI_Display_Ui_Main_Menu();	
		}
#endif
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_TIME)
		{
		#if defined(_BC_FES_SWITCH)
				if(CH_FES_A == gui_fes_struct_para.selec_index)
				{
						GUI_FES_Display_Ui_Mode_Selec_Switch(gui_fes_struct_para.selec_index,gui_fes_struct_para.cha_mode_index);
				}
				else if(CH_FES_B == gui_fes_struct_para.selec_index)
				{
						GUI_FES_Display_Ui_Mode_Selec_Switch(gui_fes_struct_para.selec_index,gui_fes_struct_para.chb_mode_index);
				}
		#endif		
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHA)
		{
        BC_FES_key_back_cha_stimulate();
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHB)
		{   
		    BC_FES_key_back_chb_stimulate();
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHA_CHB)
		{
				gui_fes_struct_para.chab_stimu_cha_strength_on_flag = CH_A_OFF;
        gui_fes_struct_para.chab_stimu_chb_strength_on_flag = CH_B_OFF;
				gui_fes_struct_para.chab_stimulate_index = MODE_FES_STRENGTH_CHA;

				BC_Fes_Ch_Timer_Off();
				
        gui_fes_struct_para.cha_current_strength = 0;
				BC_Ch1_Pwm_Off();
				BC_DAC1_Stop();

				gui_fes_struct_para.chb_current_strength = 0;
				BC_Ch2_Pwm_Off();
				BC_DAC2_Stop();

				BC_FES_cha_set_stimulation_time_status(BC_FALSE);
				BC_FES_chb_set_stimulation_time_status(BC_FALSE);

				//返回模式选择界面
				GUI_Display_Ui_Main_Menu();
		}
}



/*****************************************************************************
  * Function:BC_FES_gui_key_down_process
  * 		 
  * Description:FES down key process
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_gui_key_down_process(void)
{
    BC_DEBUG("chab_stimulate_index = %d\r\n", gui_fes_struct_para.chab_stimulate_index);
		
		if(gui_fes_struct_para.menu_id == MENU_ID_FES_MAIN)
		{
		    #if defined(_BC_FES_SWITCH)
				if((CH_FES_START < gui_fes_struct_para.selec_index) && (CH_FES_B > gui_fes_struct_para.selec_index))
				#else
				if((CH_FES_START < gui_fes_struct_para.selec_index) && (CH_FES_OK > gui_fes_struct_para.selec_index))
				#endif
				{
            gui_fes_struct_para.selec_index++;
				}
				BC_DEBUG("down selec_index = %d\r\n", gui_fes_struct_para.selec_index);
		    GUI_FES_Display_Ui_Main_Menu_Ch_Selec(gui_fes_struct_para.selec_index,gui_fes_struct_para.ch_a_flag,gui_fes_struct_para.ch_b_flag);
		}
    else if(gui_fes_struct_para.menu_id == MENU_ID_FES_MODE_CHA)
    {
				if((MODE_FES_START < gui_fes_struct_para.cha_mode_index) && (MODE_FES_CUSTOM > gui_fes_struct_para.cha_mode_index))
				{
            gui_fes_struct_para.cha_mode_index++;
				}
				BC_DEBUG("down cha_mode_index = %d\r\n", gui_fes_struct_para.cha_mode_index);
		    GUI_FES_Display_Ui_Main_Ch_Mode_Back(gui_fes_struct_para.cha_mode_index);
		}		
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_MODE_CHB)
		{
    		if((MODE_FES_START < gui_fes_struct_para.chb_mode_index) && (MODE_FES_CUSTOM > gui_fes_struct_para.chb_mode_index))
				{
            gui_fes_struct_para.chb_mode_index++;
				}
				BC_DEBUG("down chb_mode_index = %d\r\n", gui_fes_struct_para.chb_mode_index);
		    GUI_FES_Display_Ui_Main_Ch_Mode_Back(gui_fes_struct_para.chb_mode_index);
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_TIME)
		{
				//BC_DEBUG("3 total_time=%d\r\n",gui_struct_para.total_time);
        if((FES_TOTAL_TIME_MIN <= gui_fes_struct_para.stimu_time) && (FES_TOTAL_TIME_MAX > gui_fes_struct_para.stimu_time))
        {
						gui_fes_struct_para.stimu_time += FES_TOTAL_TIME_STEP;
						GUI_FES_DrawImageNum_Adjust_Ex(gui_fes_struct_para.stimu_time,0,0);
        }
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHA)
		{
				BC_FES_key_down_channel_a_stimulate();
		}	
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHB)
		{
		    #if defined(_BC_FES_SWITCH)
        BC_FES_key_down_channel_b_stimulate();
				#else
				if((FES_STRENGTH_MAX > gui_fes_struct_para.chb_current_strength) &&
					 (FES_STRENGTH_MIN <= gui_fes_struct_para.chb_current_strength))
				{
				    gui_fes_struct_para.chb_current_strength++;
						if(1 == gui_fes_struct_para.chb_current_strength)
						{
                BC_DAC2_Start();
								BC_Ch2_Pwm_On();								
						    BC_Fes_Ch_Timer_On();
						}
						BC_DAC2_Set_Level(gui_fes_struct_para.chb_current_strength);
						
						GUI_FES_DrawImageNum_Adjust(gui_fes_struct_para.chb_current_strength,0,0);

						BC_DEBUG("down chb current_strength = %d\r\n", gui_fes_struct_para.chb_current_strength);
				}
				#endif
		}	
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHA_CHB)
		{
        if(CH_A_ON == gui_fes_struct_para.chab_stimu_cha_strength_on_flag)
		    {
		    		if((FES_STRENGTH_MAX > gui_fes_struct_para.cha_current_strength) &&
			         (FES_STRENGTH_MIN <= gui_fes_struct_para.cha_current_strength))
						{
						    gui_fes_struct_para.cha_current_strength++;
								if(1 == gui_fes_struct_para.cha_current_strength)
								{
		                BC_DAC1_Start();
										BC_Ch1_Pwm_On();
								}
								BC_DAC1_Set_Level(gui_fes_struct_para.cha_current_strength);
								
								BC_DEBUG("down chab a current_strength = %d\r\n", gui_fes_struct_para.cha_current_strength);
								GUI_FES_Display_Ui_Main_Cha_Chb_Stimulate_Select(gui_fes_struct_para.chab_stimulate_index);
						}
		    }
				else if(CH_B_ON == gui_fes_struct_para.chab_stimu_chb_strength_on_flag)
		    {
		    		if((FES_STRENGTH_MAX > gui_fes_struct_para.chb_current_strength) &&
			         (FES_STRENGTH_MIN <= gui_fes_struct_para.chb_current_strength))
						{
						    gui_fes_struct_para.chb_current_strength++;
								if(1 == gui_fes_struct_para.chb_current_strength)
								{
		                BC_DAC2_Start();
										BC_Ch2_Pwm_On();
								}
								BC_DAC2_Set_Level(gui_fes_struct_para.chb_current_strength);								
		      
								BC_DEBUG("down chab b current_strength = %d\r\n", gui_fes_struct_para.chb_current_strength);
								GUI_FES_Display_Ui_Main_Cha_Chb_Stimulate_Select(gui_fes_struct_para.chab_stimulate_index);
						}
		    }
				else
				{
						if((MODE_FES_STOP_START_CHB > gui_fes_struct_para.chab_stimulate_index) &&
							 (MODE_FES_STRENGTH_CHA <= gui_fes_struct_para.chab_stimulate_index))
						{  
								gui_fes_struct_para.chab_stimulate_index++;   	  
								GUI_FES_Display_Ui_Main_Cha_Chb_Stimulate_Select(gui_fes_struct_para.chab_stimulate_index);
						}
				}
				
		}		
}


/*****************************************************************************
  * Function:BC_FES_gui_key_up_process
  * 		 
  * Description:FES up key process
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_gui_key_up_process(void)
{
		if(gui_fes_struct_para.menu_id == MENU_ID_FES_MAIN)
		{
				if((CH_FES_A < gui_fes_struct_para.selec_index) && (CH_FES_END > gui_fes_struct_para.selec_index))
				{
            gui_fes_struct_para.selec_index--;
				}
				BC_DEBUG("up selec_index = %d\r\n", gui_fes_struct_para.selec_index);
		    GUI_FES_Display_Ui_Main_Menu_Ch_Selec(gui_fes_struct_para.selec_index,gui_fes_struct_para.ch_a_flag,gui_fes_struct_para.ch_b_flag);
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_MODE_CHA)
    {
				if((MODE_FES_UPPER < gui_fes_struct_para.cha_mode_index) && (MODE_FES_END > gui_fes_struct_para.cha_mode_index))
				{
            gui_fes_struct_para.cha_mode_index--;
				}
				BC_DEBUG("up cha_mode_index = %d\r\n", gui_fes_struct_para.cha_mode_index);
		    GUI_FES_Display_Ui_Main_Ch_Mode_Back(gui_fes_struct_para.cha_mode_index);
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_MODE_CHB)
		{
    		if((MODE_FES_UPPER < gui_fes_struct_para.chb_mode_index) && (MODE_FES_END > gui_fes_struct_para.chb_mode_index))
				{
            gui_fes_struct_para.chb_mode_index--;
				}
				BC_DEBUG("up chb_mode_index = %d\r\n", gui_fes_struct_para.chb_mode_index);
		    GUI_FES_Display_Ui_Main_Ch_Mode_Back(gui_fes_struct_para.chb_mode_index);
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_TIME)
		{
				if((FES_TOTAL_TIME_MIN < gui_fes_struct_para.stimu_time) && (FES_TOTAL_TIME_MAX >= gui_fes_struct_para.stimu_time))
				{
						gui_fes_struct_para.stimu_time -= FES_TOTAL_TIME_STEP;
						GUI_FES_DrawImageNum_Adjust_Ex(gui_fes_struct_para.stimu_time,0,0);
				}
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHA)
		{
        BC_FES_key_up_channel_a_stimulate();
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHB)
		{
		#if defined(_BC_FES_SWITCH)
				BC_FES_key_up_channel_b_stimulate();
		#else
        if((FES_STRENGTH_MAX >= gui_fes_struct_para.chb_current_strength) &&
					 (FES_STRENGTH_MIN < gui_fes_struct_para.chb_current_strength))
		    {
            gui_fes_struct_para.chb_current_strength--;
						BC_DAC2_Set_Level(gui_fes_struct_para.chb_current_strength); 
						if(0 == gui_fes_struct_para.chb_current_strength)
						{
                BC_DAC2_Stop();
							  BC_Ch2_Pwm_Off();
								BC_Fes_Ch_Timer_Off();
						}
						
						GUI_FES_DrawImageNum_Adjust(gui_fes_struct_para.chb_current_strength,0,0);

						BC_DEBUG("up chb current_strength = %d\r\n", gui_fes_struct_para.chb_current_strength);
			  }
		#endif
		}
		else if(gui_fes_struct_para.menu_id == MENU_ID_FES_STIMULATE_CHA_CHB)
		{
		    if(CH_A_ON == gui_fes_struct_para.chab_stimu_cha_strength_on_flag)
		    {
						if((FES_STRENGTH_MAX >= gui_fes_struct_para.cha_current_strength) &&
							 (FES_STRENGTH_MIN < gui_fes_struct_para.cha_current_strength))
				    {
		            gui_fes_struct_para.cha_current_strength--;
								BC_DAC1_Set_Level(gui_fes_struct_para.cha_current_strength); 
								if(0 == gui_fes_struct_para.cha_current_strength)
								{
		                BC_DAC1_Stop();
									  BC_Ch1_Pwm_Off();
								}
								
								BC_DEBUG("up chab a current_strength = %d\r\n", gui_fes_struct_para.cha_current_strength);
								GUI_FES_Display_Ui_Main_Cha_Chb_Stimulate_Select(gui_fes_struct_para.chab_stimulate_index);
					  }
				}
				else if(CH_B_ON == gui_fes_struct_para.chab_stimu_chb_strength_on_flag)
		    {
						if((FES_STRENGTH_MAX >= gui_fes_struct_para.chb_current_strength) &&
							 (FES_STRENGTH_MIN < gui_fes_struct_para.chb_current_strength))
				    {
		            gui_fes_struct_para.chb_current_strength--;
								BC_DAC2_Set_Level(gui_fes_struct_para.chb_current_strength); 
								if(0 == gui_fes_struct_para.chb_current_strength)
								{
		                BC_DAC2_Stop();
									  BC_Ch2_Pwm_Off();
								}

								BC_DEBUG("up chab b current_strength = %d\r\n", gui_fes_struct_para.chb_current_strength);
								GUI_FES_Display_Ui_Main_Cha_Chb_Stimulate_Select(gui_fes_struct_para.chab_stimulate_index);
					  }
				}
				else
				{
            if((MODE_FES_STOP_START_CHB >= gui_fes_struct_para.chab_stimulate_index) &&
							 (MODE_FES_STRENGTH_CHA < gui_fes_struct_para.chab_stimulate_index))
						{
					  		gui_fes_struct_para.chab_stimulate_index--; 	
								GUI_FES_Display_Ui_Main_Cha_Chb_Stimulate_Select(gui_fes_struct_para.chab_stimulate_index);
						}
				}
		}
}


/*****************************************************************************
  * Function:BC_FES_cha_parse_data
  * 		 
  * Description: FES cha control data parse
  * 55 DB 02 00 02 01 00 00 01 01 01 84 C9 	channel A on	 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_cha_parse_data(uint8_t *data)
{
		static uint8_t pwm_cha_switch_flag = 1; 
		uint32_t period = 0;
		uint16_t us_val = 0;
		uint8_t  hz_val = 0;
		
		if(PARA_OFF_ON == data[9])//PWM开关
    {
        if(0x01 == data[10])//PWM开
        {
		        if(pwm_cha_switch_flag)
		        {
		            pwm_cha_switch_flag = 0;
								BC_DAC1_Start();
								BC_Ch1_Pwm_On();
								//BC_play_audio_1();
		        }
        }
				else
				{
				    pwm_cha_switch_flag = 1;
						//BC_DAC1_Stop();
						BC_Ch1_Pwm_Off();
						//BC_play_audio_4();
				}		
		}		
		else if(PARA_MODE == data[9]) 
		{
				if(0x01 == data[10])//Mode A upper
				{
						BC_FES_cha_upper_set_para();
						gui_fes_struct_para.cha_mode_index = MODE_FES_UPPER;
						BC_DEBUG("CHA MODE A\r\n");
				}
				else if(0x02 == data[10])//Mode B lower
				{
						BC_FES_cha_upper_set_para();
						gui_fes_struct_para.cha_mode_index = MODE_FES_LOWER;
						BC_DEBUG("CHA MODE B\r\n");
				} 	
				else if(0x03 == data[10])//Mode C
				{
						BC_FES_cha_swallow_set_para();
						gui_fes_struct_para.cha_mode_index = MODE_FES_SWALLOW;
						BC_DEBUG("CHA MODE C\r\n");
				}
				else if(0x04 == data[9])//Mode CUSTOM 55 DB 01 00 05 04 00 00 02 04 00 C8 05 B7 3D
				{
						/*us_val = (data[10]<<8) | data[11];
						hz_val = data[12];
						
						if((us_val >= VNS_PULSE_WIDTH_MIN) && (us_val <= VNS_PULSE_WIDTH_MAX)  &&
							 (hz_val >= VNS_FREQUENCY_MIN) && (hz_val <= VNS_FREQUENCY_MAX))
						{
								period = us_val/2;
								MX_TIM3_Init(period);
								g_data.pwm1_cycle = 1000000/hz_val/period;
						}
						else
						{
								us_val = VNS_PULSE_WIDTH_MIN;
								hz_val = VNS_FREQUENCY_MIN;
									
								period = us_val/2;
								MX_TIM3_Init(period);
								g_data.pwm1_cycle = 1000000/hz_val/period;
						}*/
						
						BC_DEBUG("CHA pwm1_cycle=%d period=%d us=%d hz=%d\r\n",g_data.pwm1_cycle,period,us_val,hz_val);
				}
		}
		else if(PARA_STRENGTH == data[9]) 
		{
				if(FES_STRENGTH_MAX > data[10]) //3.3v
				{
				    gui_fes_struct_para.cha_current_strength = data[10];
						BC_DAC1_Set_Level(gui_fes_struct_para.cha_current_strength);
						BC_DEBUG("DAC1 PC set strength = %d\r\n",data[10]);
				}
				else
				{
					 
				}
		}
		else
		{
				
		}
}


/*****************************************************************************
  * Function:BC_FES_chb_parse_data
  * 		 
  * Description: FES chb control data parse
  * 55 DB 02 00 02 01 00 00 01 01 01 84 C9 	channel B on	 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_chb_parse_data(uint8_t *data)
{
		static uint8_t pwm_chb_switch_flag = 1; 
		uint32_t period = 0;
		uint16_t us_val = 0;
		uint8_t  hz_val = 0;
		
		if(PARA_OFF_ON == data[9])//PWM开关
    {
        if(0x01 == data[10])//PWM开
        {
		        if(pwm_chb_switch_flag)
		        {
		            pwm_chb_switch_flag = 0;
								#if defined(_BC_FES_SWITCH)
								BC_DAC1_Start();
								BC_Ch1_Pwm_On();
								#else
								BC_DAC2_Start();
								BC_Ch2_Pwm_On();
								#endif
		        }
        }
				else
				{
				    pwm_chb_switch_flag = 1;
						#if defined(_BC_FES_SWITCH)
						BC_Ch1_Pwm_Off();	
						#else
						BC_Ch2_Pwm_Off();	
						#endif
				}		
		}		
		else if(PARA_MODE == data[9]) 
		{
				if(0x01 == data[10])//Mode A
				{
						BC_FES_chb_upper_set_para();
						BC_DEBUG("CHB MODE A\r\n");
				}
				else if(0x02 == data[10])//Mode B
				{
						BC_FES_chb_upper_set_para();
						BC_DEBUG("CHB MODE B\r\n");
				} 	
				else if(0x03 == data[10])//Mode C
				{
						BC_FES_cha_swallow_set_para();
						BC_DEBUG("CHB MODE C\r\n");
				}
				else if(0x04 == data[9])//Mode CUSTOM 55 DB 01 00 05 04 00 00 02 04 00 C8 05 B7 3D
				{
						/*us_val = (data[10]<<8) | data[11];
						hz_val = data[12];
						
						if((us_val >= VNS_PULSE_WIDTH_MIN) && (us_val <= VNS_PULSE_WIDTH_MAX)  &&
							 (hz_val >= VNS_FREQUENCY_MIN) && (hz_val <= VNS_FREQUENCY_MAX))
						{
								period = us_val/2;
								MX_TIM3_Init(period);
								g_data.pwm2_cycle = 1000000/hz_val/period;
						}
						else
						{
								us_val = VNS_PULSE_WIDTH_MIN;
								hz_val = VNS_FREQUENCY_MIN;
									
								period = us_val/2;
								MX_TIM3_Init(period);
								g_data.pwm2_cycle = 1000000/hz_val/period;
						}*/
						
						BC_DEBUG("CHB pwm2_cycle=%d period=%d us=%d hz=%d\r\n",g_data.pwm2_cycle,period,us_val,hz_val);
				}
		}
		else if(PARA_STRENGTH == data[9]) 
		{
				if(FES_STRENGTH_MAX > data[10]) //3.3v
				{
				    #if defined(_BC_FES_SWITCH)
            gui_fes_struct_para.cha_current_strength = data[10];
						BC_DAC1_Set_Level(gui_fes_struct_para.cha_current_strength);
						#else
				    gui_fes_struct_para.chb_current_strength = data[10];
						BC_DAC2_Set_Level(gui_fes_struct_para.chb_current_strength);
						#endif
				}
				else
				{
					 
				}
		}
		else
		{
				
		}
}



/*****************************************************************************
  * Function:BC_FES_parse_data
  * 		 
  * Description: FES control data parse
  * 55 DB 02 00 02 01 00 00 01 01 01 84 C9 	channel on	 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_FES_parse_data(uint8_t *data)
{		
		static uint8_t pwm2_switch_flag = 1; 
			
		if(CHANNEL_A == data[8]) //通道1
		{
		#if defined(_BC_FES_SWITCH)
				SWITCH_CHA;
	  #endif
		    BC_FES_cha_parse_data(data);
				BC_response_data(data);
		}
		else if(CHANNEL_B == data[8]) //通道2
		{
		#if defined(_BC_FES_SWITCH)
				SWITCH_CHB;
		    BC_FES_cha_parse_data(data);
	  #else
		    BC_FES_chb_parse_data(data);
		#endif
				BC_response_data(data);
		}
		else
	  {
        //
		}
}

#else
/*****************************************************************************
  * Function:BC_VNS_set_stimulation_time
  * 		 
  * Description:设置刺激时间长度
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_set_stimulation_time(uint16_t total_time)
{
		gui_struct_para.time_min = total_time/60;
		gui_struct_para.time_sec = total_time%60;
}


/*****************************************************************************
  * Function:BC_VNS_custom_time_do
  * 		 
  * Description:设置自定义刺激时间，休息时间，总时间
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_custom_time_do(void)
{		
		BC_DEBUG("total_time=%d g_total1_time = %d start_time=%d stop_time=%d\r\n",gui_struct_para.total_time, g_total1_time,gui_struct_para.start_time,gui_struct_para.stop_time);
    GUI_Display_Ui_Electrode_Connect();

		if(0 < g_total1_time)
	  {
	  		g_total1_time--;
				
				if(0 < (gui_struct_para.start_time-1))
				{
						gui_struct_para.start_time--;
						g_start_falg = 1;
				}
				else
				{
            //stop
            if(!g_pwm_off_falg)
            {
            		g_pwm_off_falg = 1;
                BC_Ch1_Pwm_Off();
            }
						
						if(0 < (gui_struct_para.stop_time-1))
						{
								gui_struct_para.stop_time--;
								g_start_falg = 0;	
						}
						else
						{
								//start
								g_pwm_off_falg = 0;
                BC_Ch1_Pwm_On();
								
								gui_struct_para.start_time = g_start_time;
						    gui_struct_para.stop_time = g_stop_time;
						}
				}
		}
		else
		{
    		//stop
    		BC_Ch1_Pwm_Off();
				gui_struct_para.total_time = g_total_time;
				gui_struct_para.start_time = g_start_time;
				gui_struct_para.stop_time = g_stop_time;
		}		
}


/*****************************************************************************
  * Function:BC_VNS_countdown_time
  * 		 
  * Description:VNS 定时器倒计时
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_countdown_time(void)
{
    BC_DEBUG("min=%d sec=%d\r\n",gui_struct_para.time_min,gui_struct_para.time_sec);
		
    if(0 == gui_struct_para.time_sec--)
    {
        if(gui_struct_para.time_min > 0)
        {
            gui_struct_para.time_min--;
						gui_struct_para.time_sec = 59;
    	  }
				else
				{
            if(0 == gui_struct_para.time_min)
            {
                //关闭定时器
						    BC_Ch1_Countdown_tim_Off();
								BC_Ch1_Pwm_Off(); 
								BC_VNS_set_stimulation_time(0);
								BC_play_audio_4();
						}
				}
		}

    BC_VNS_custom_time_do();
	  GUI_Display_Countdown_Time(96,260,gui_struct_para.time_min,gui_struct_para.time_sec);
}


/*****************************************************************************
  * Function:BC_VNS_mode_a_set_para
  * 		 
  * Description:设置模式A参数
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_mode_a_set_para(void)
{
		g_data.pwm1_cycle = VNS_PWM1_MODE_A_CYC;
    MX_TIM3_Init(PERIOD_VALUE);

		gui_struct_para.total_time = 30;
		gui_struct_para.start_time = 30;
		gui_struct_para.stop_time = 270;

		g_total_time = gui_struct_para.total_time;
		g_total1_time = g_total_time*60;
		g_start_time = gui_struct_para.start_time;
		g_stop_time  = gui_struct_para.stop_time;
}


/*****************************************************************************
  * Function:BC_VNS_mode_b_set_para
  * 		 
  * Description:设置模式B参数
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_mode_b_set_para(void)
{
    g_data.pwm1_cycle = VNS_PWM1_MODE_B_CYC;
		MX_TIM3_Init(PERIOD_VALUE);

		gui_struct_para.total_time = 30;
		gui_struct_para.start_time = 1800;
		gui_struct_para.stop_time = 0;

		g_total_time = gui_struct_para.total_time;
		g_total1_time =  g_total_time*60;
		g_start_time = gui_struct_para.start_time;
		g_stop_time  = gui_struct_para.stop_time;
}


/*****************************************************************************
  * Function:BC_VNS_mode_c_set_para
  * 		 
  * Description:设置模式C参数
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_mode_c_set_para(void)
{
    BC_DAC1_Start();
    gui_struct_para.current_strength = VNS_MODE_C_STRENGTH; //0.5mA
		BC_DAC1_Set_Level(gui_struct_para.current_strength); 
		
    g_data.pwm1_cycle = VNS_PWM1_MODE_C_CYC;
		MX_TIM3_Init(PERIOD_VALUE);

		gui_struct_para.total_time = 30;
		gui_struct_para.start_time = 30;
		gui_struct_para.stop_time = 30;

		g_total_time = gui_struct_para.total_time;
		g_total1_time =  g_total_time*60;
		g_start_time = gui_struct_para.start_time;
		g_stop_time  = gui_struct_para.stop_time;
}


/*****************************************************************************
  * Function:BC_VNS_mode_custom_set_default_para
  * 		 
  * Description:设置自定义模式默认参数
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_mode_custom_set_default_para(void)
{
    uint32_t  period = 0;

		gui_struct_para.custom_index = CUSTOM_ID_PLUSE;
		
		period = gui_struct_para.us_val/2;
	  MX_TIM3_Init(period-1);
		g_data.pwm1_cycle = 1000000/gui_struct_para.hz_val/period;
		BC_DAC1_Set_Level(gui_struct_para.current_strength); 

		gui_struct_para.total_time = g_total_time;
		gui_struct_para.start_time = g_start_time;
		gui_struct_para.stop_time = g_stop_time;

		BC_DEBUG("CUSTOM period=%d pwm1_cycle=%d\r\n",period,g_data.pwm1_cycle);
}
 




/*****************************************************************************
  * Function:BC_VNS_parse_data
  * 		 
  * Description: VNS control data parse
  * 55 DB 01 00 02 01 00 00 01 00 DD 71		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_parse_data(uint8_t *data)
{		
    static uint8_t pwm_switch_flag = 1; 
		uint32_t period = 0;
		uint16_t us_val = 0;
		uint8_t  hz_val = 0;
			
		if(PARA_OFF_ON == data[8])
		{
		    if(0x01 == data[9])//打开PWM
		    {
		        if(pwm_switch_flag)
		        {
		            pwm_switch_flag = 0;
								BC_DAC1_Start();
								BC_Ch1_Pwm_On();
		        }
				}
				else
				{
				    pwm_switch_flag = 1;
						//BC_DAC1_Stop();
						BC_Ch1_Pwm_Off();			
				}		
		}
		else if(PARA_MODE == data[8]) 
		{
		    if(0x01 == data[9])//Mode A
		    {
            BC_VNS_mode_a_set_para();
						BC_DEBUG("MODE A\r\n");
				}
				else if(0x02 == data[9])//Mode B
				{
            BC_VNS_mode_b_set_para();
						BC_DEBUG("MODE B\r\n");
				}		
				else if(0x03 == data[9])//Mode C
				{
            BC_VNS_mode_c_set_para();
						BC_DEBUG("MODE C\r\n");
				}
				else if(0x04 == data[9])//Mode CUSTOM 55 DB 01 00 05 04 00 00 02 04 00 C8 05 B7 3D
				{
            us_val = (data[10]<<8) | data[11];
				    hz_val = data[12];
						
						if((us_val >= VNS_PULSE_WIDTH_MIN) && (us_val <= VNS_PULSE_WIDTH_MAX)  &&
							 (hz_val >= VNS_FREQUENCY_MIN) && (hz_val <= VNS_FREQUENCY_MAX))
						{
                period = us_val/2;
								MX_TIM3_Init(period);
								g_data.pwm1_cycle = 1000000/hz_val/period;
						}
						else
						{
						    us_val = VNS_PULSE_WIDTH_MIN;
								hz_val = VNS_FREQUENCY_MIN;
									
                period = us_val/2;
								MX_TIM3_Init(period);
								g_data.pwm1_cycle = 1000000/hz_val/period;
						}
				    
						BC_DEBUG("pwm1_cycle=%d period=%d us=%d hz=%d\r\n",g_data.pwm1_cycle,period,us_val,hz_val);
				}
		}
		else if(PARA_STRENGTH == data[8]) 
		{
		    if(VNS_STRENGTH_MAX > data[9]) //3.3v
		    {
				    BC_DAC1_Set_Level(data[9]);
		    }
				else
				{
           
				}
		}
		else
		{
        
		}

		BC_response_data(data);
}


/*****************************************************************************
  * Function: BC_VNS_gui_key_3_custom_adjust_menu  
  * 		 
  * Description: 按键3的自定义界面调节菜单
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_gui_key_3_custom_adjust_menu(void)
{
    if(gui_struct_para.ok_flag)
    {
        if(CUSTOM_ID_PLUSE == gui_struct_para.custom_index)
        {
            BC_DEBUG("3 us_val=%d\r\n",gui_struct_para.us_val);
            if((VNS_PULSE_WIDTH_MIN <= gui_struct_para.us_val) && (VNS_PULSE_WIDTH_MAX > gui_struct_para.us_val))
            {
                gui_struct_para.us_val += VNS_PULSE_WIDTH_STEP;
								GUI_VNS_Display_Ui_Custom_Pluse_Width_Ex();
            }
				}
				else if(CUSTOM_ID_FREQU == gui_struct_para.custom_index)
				{
				    BC_DEBUG("3 hz_val=%d\r\n",gui_struct_para.hz_val);
            if((VNS_FREQUENCY_MIN <= gui_struct_para.hz_val) && (VNS_FREQUENCY_MAX > gui_struct_para.hz_val))
            {
                gui_struct_para.hz_val += VNS_FREQUENCY_STEP;
								GUI_VNS_Display_Ui_Custom_Frequency_Ex();
            }
				}
		    else if(CUSTOM_ID_STREN == gui_struct_para.custom_index)
		    {		      
		        if((VNS_STRENGTH_MAX > gui_struct_para.current_strength) &&
							 (VNS_STRENGTH_MIN <= gui_struct_para.current_strength))
						{
						    gui_struct_para.current_strength++;
						    if(MODE_CUSTOM == gui_struct_para.menu_mode_index)
						    {
				            GUI_VNS_Display_Ui_Custom_Strength_Ex();	
						    }
								else
								{
		                GUI_Display_Strength_Adjust_Val(50,120,gui_struct_para.current_strength);
								}
								
								BC_DEBUG("p3_%d current_strength = %d\r\n", gui_struct_para.menu_id,gui_struct_para.current_strength);
						}
		    } 
				else if(CUSTOM_ID_TOTIM == gui_struct_para.custom_index)
				{
            BC_DEBUG("3 total_time=%d\r\n",gui_struct_para.total_time);
            if((VNS_TOTAL_TIME_MIN <= gui_struct_para.total_time) && (VNS_TOTAL_TIME_MAX > gui_struct_para.total_time))
            {
                gui_struct_para.total_time += VNS_TOTAL_TIME_STEP;
								GUI_VNS_Display_Ui_Custom_Total_Time_Ex();
            }
				}
				else if(CUSTOM_ID_STATIM == gui_struct_para.custom_index)
				{
            BC_DEBUG("3 start_time=%d\r\n",gui_struct_para.start_time);
            if((VNS_START_TIME_MIN <= gui_struct_para.start_time) && ((gui_struct_para.total_time*60)> gui_struct_para.start_time))
            {
                gui_struct_para.start_time += VNS_START_TIME_STEP;
								GUI_VNS_Display_Ui_Custom_Start_Time_Ex();
            }
				}
				else if(CUSTOM_ID_STOTIM == gui_struct_para.custom_index)
				{
            BC_DEBUG("3 stop_time=%d\r\n",gui_struct_para.stop_time);
            if((VNS_STOP_TIME_MIN <= gui_struct_para.stop_time) && (VNS_STOP_TIME_MAX> gui_struct_para.stop_time))
            {
                gui_struct_para.stop_time += VNS_STOP_TIME_STEP;
								GUI_VNS_Display_Ui_Custom_Stop_Time_Ex();
            }
				}
				else
				{
            
				}
    }
		else
    {
        if((7 >= gui_struct_para.custom_index) &&
					 (1 < gui_struct_para.custom_index))
        {
            gui_struct_para.custom_index--;
            GUI_VNS_Display_Ui_Custom_Selec(gui_struct_para.custom_index);
				    BC_DEBUG("3 custom_index = %d\r\n", gui_struct_para.custom_index);
				}        
    }
}



/*****************************************************************************
  * Function: BC_VNS_gui_key_4_custom_adjust_menu  
  * 		 
  * Description: 按键4的自定义界面调节菜单
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_gui_key_4_custom_adjust_menu(void)
{
    if(gui_struct_para.ok_flag)
    {
        if(CUSTOM_ID_PLUSE == gui_struct_para.custom_index)
        {
            BC_DEBUG("4 us_val=%d\r\n",gui_struct_para.us_val);
            if((VNS_PULSE_WIDTH_MIN < gui_struct_para.us_val) && (VNS_PULSE_WIDTH_MAX >= gui_struct_para.us_val))
            {
                gui_struct_para.us_val -= VNS_PULSE_WIDTH_STEP;
								GUI_VNS_Display_Ui_Custom_Pluse_Width_Ex();
            }
				}
				else if(CUSTOM_ID_FREQU == gui_struct_para.custom_index)
				{
				    BC_DEBUG("4 hz_val=%d\r\n",gui_struct_para.hz_val);
            if((VNS_FREQUENCY_MIN < gui_struct_para.hz_val) && (VNS_FREQUENCY_MAX >= gui_struct_para.hz_val))
            {
                gui_struct_para.hz_val -= VNS_FREQUENCY_STEP;
								GUI_VNS_Display_Ui_Custom_Frequency_Ex();
            }
				}
		    else if(CUSTOM_ID_STREN == gui_struct_para.custom_index)
        {  
						if((VNS_STRENGTH_MAX >= gui_struct_para.current_strength) &&
						  (VNS_STRENGTH_MIN < gui_struct_para.current_strength))
						{
								gui_struct_para.current_strength--;
								if(MODE_CUSTOM == gui_struct_para.menu_mode_index)
								{
										GUI_VNS_Display_Ui_Custom_Strength_Ex();
								}
								else
								{
										GUI_Display_Strength_Adjust_Val(50,120,gui_struct_para.current_strength);
								}
								

								BC_DEBUG("p4_%d current_strength = %d\r\n", gui_struct_para.menu_id,gui_struct_para.current_strength);
						}
				}
				else if(CUSTOM_ID_TOTIM == gui_struct_para.custom_index)
				{
            BC_DEBUG("4 total_time=%d\r\n",gui_struct_para.total_time);
            if((VNS_TOTAL_TIME_MIN < gui_struct_para.total_time) && (VNS_TOTAL_TIME_MAX >= gui_struct_para.total_time))
            {
                gui_struct_para.total_time -= VNS_TOTAL_TIME_STEP;
								GUI_VNS_Display_Ui_Custom_Total_Time_Ex();
            }
				}
				else if(CUSTOM_ID_STATIM == gui_struct_para.custom_index)
				{
            BC_DEBUG("4 start_time=%d\r\n",gui_struct_para.start_time);
            if((VNS_START_TIME_MIN < gui_struct_para.start_time) && ((gui_struct_para.total_time*60) >= gui_struct_para.start_time))
            {
                gui_struct_para.start_time -= VNS_START_TIME_STEP;
								GUI_VNS_Display_Ui_Custom_Start_Time_Ex();
            }
				}
				else if(CUSTOM_ID_STOTIM == gui_struct_para.custom_index)
				{
            BC_DEBUG("4 stop_time=%d\r\n",gui_struct_para.stop_time);
            if((VNS_STOP_TIME_MIN < gui_struct_para.stop_time) && (VNS_STOP_TIME_MAX >= gui_struct_para.stop_time))
            {
                gui_struct_para.stop_time -= VNS_STOP_TIME_STEP;
								GUI_VNS_Display_Ui_Custom_Stop_Time_Ex();
            }
				}
				else
				{
            
				}
		}
		else
    {
			  if((7 > gui_struct_para.custom_index) &&
					 (0 < gui_struct_para.custom_index))
        {
            gui_struct_para.custom_index++;
            GUI_VNS_Display_Ui_Custom_Selec(gui_struct_para.custom_index);
				    BC_DEBUG("4 custom_index = %d\r\n", gui_struct_para.custom_index);
				}  
    }
}


/*****************************************************************************
  * Function:BC_VNS_gui_key_ok_process
  * 		 
  * Description:VNS ok key process
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_gui_key_ok_process(void)
{
    uint32_t period = 0;

		Get_Adc_Battery_Val(ADC_CHANNEL_15);
		GUI_Display_Ui_Battery_Level_Icon();
		
    if(gui_struct_para.menu_id == MENU_ID_MAIN)
    {
        if(MODE_A == gui_struct_para.menu_mode_index)
        {       
            //设置模式A参数
            #if !defined(_TEST_)
            BC_VNS_mode_a_set_para();
						#endif
						//设置时间参数
					  BC_VNS_set_stimulation_time(VNS_TOTAL_TIME_MAX*60);
						//开始
						//BC_Ch1_Pwm_On();
            //进入强度调节界面
		        GUI_VNS_Display_Ui_Adjust_Strength();
        }
				else if(MODE_B == gui_struct_para.menu_mode_index)
				{
            //设置模式B参数
            BC_VNS_mode_b_set_para();
						//设置时间参数
					  BC_VNS_set_stimulation_time(VNS_TOTAL_TIME_MAX*60);
						//开始
						//BC_Ch1_Pwm_On();
            //进入强度调节界面
            GUI_VNS_Display_Ui_Adjust_Strength();
				}
				else if(MODE_C == gui_struct_para.menu_mode_index)
				{
            //设置模式C参数
            BC_VNS_mode_c_set_para();
						//设置时间参数
					  BC_VNS_set_stimulation_time(VNS_TOTAL_TIME_MAX*60);
						//开始
						BC_Ch1_Pwm_On();
            //进入强度调节界面
            GUI_VNS_Display_Ui_Adjust_Strength();
				}
				else if(MODE_CUSTOM == gui_struct_para.menu_mode_index)
				{   
				    //设置自定义模式默认参数
				    BC_VNS_mode_custom_set_default_para();
				    //设置时间参数
						BC_VNS_set_stimulation_time(gui_struct_para.total_time*60);
            //进入自定义参数调节界面
            GUI_VNS_Display_Ui_Custom_Menu();
				}
    }
		else if(gui_struct_para.menu_id == MENU_ID_STRENGTH)
		{
		    //开始刺激
		    //BC_Ch1_Pwm_On();
		    
		    if(0 < gui_struct_para.current_strength)
		    {
		        //开始倒计时
		        BC_Ch1_Countdown_tim_On();
						BC_play_audio_1();
		    
				    //进入刺激界面
		        GUI_VNS_Display_Ui_Stimulate();
		    }
		}
		else if(gui_struct_para.menu_id == MENU_ID_STIMULATE)
		{	
       //开始/暂停刺激
			 g_data.start_stop_flag = !g_data.start_stop_flag;
			 if(!g_data.start_stop_flag)
			 {
			    if((0 < g_total1_time) && (0 < g_start_time) && (0 < gui_struct_para.current_strength))
			    {
			    		BC_Ch1_Countdown_tim_On();
							
							if(g_start_falg)
							{
							    BC_Ch1_Pwm_On();
									BC_play_audio_3();
							}
			    }
					else
					{
							g_start_falg = 0;
					}
			 }
			 else
			 {
          BC_Ch1_Countdown_tim_Off();
					BC_Ch1_Pwm_Off();
					BC_play_audio_2();
			 }
		}
		else if(gui_struct_para.menu_id == MENU_ID_CUSTOM)
	  {
		    if(CUSTOM_ID_OK == gui_struct_para.custom_index)
		    {
		        if(0 < gui_struct_para.current_strength)
		        {
				        period = gui_struct_para.us_val/2;
				        //设置参数
							  MX_TIM3_Init(period-1);
								g_data.pwm1_cycle = 1000000/gui_struct_para.hz_val/period;
								
								BC_DAC1_Start();
								BC_DAC1_Set_Level(gui_struct_para.current_strength); 
								
								g_total_time = gui_struct_para.total_time;
								g_total1_time =  g_total_time*60;
								g_start_time = gui_struct_para.start_time;
								g_stop_time  = gui_struct_para.stop_time;

								//打开定时器和PWM
								if((0 < g_total1_time) && (0 < g_start_time))
								{		
										BC_Ch1_Countdown_tim_On();
						        BC_Ch1_Pwm_On();
								}
								
								BC_VNS_set_stimulation_time(gui_struct_para.total_time*60);
								
						    //进入刺激界面
				        GUI_VNS_Display_Ui_Stimulate();
								BC_play_audio_1();
		        }
		    }
        else
        {
		        gui_struct_para.ok_flag = !gui_struct_para.ok_flag;
						BC_DEBUG("ok_flag=%d\r\n",gui_struct_para.ok_flag);
						GUI_VNS_Display_Ui_Custom_Selec(gui_struct_para.custom_index);
        }
		}
}


/*****************************************************************************
  * Function:BC_VNS_gui_key_back_process
  * 		 
  * Description:VNS back key process
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_gui_key_back_process(void)
{	
    g_data.start_stop_flag = 0;
		g_pwm_off_falg = 0;
		
    if(gui_struct_para.menu_id == MENU_ID_STRENGTH)
    {
        //返回模式选择界面
        gui_struct_para.current_strength = 0;
				BC_Ch1_Pwm_Off();
				BC_DAC1_Stop();
        GUI_Display_Ui_Main_Menu();
    }
		else if(gui_struct_para.menu_id == MENU_ID_STIMULATE)
		{
		    //关闭定时器
		    BC_Ch1_Countdown_tim_Off();
    		//返回模式选择界面
				gui_struct_para.current_strength = 0;
				BC_Ch1_Pwm_Off();
				BC_DAC1_Stop();
        GUI_Display_Ui_Main_Menu();
				BC_play_audio_4();
		}
		else if(gui_struct_para.menu_id == MENU_ID_CUSTOM)
		{
		    //关闭定时器
		    BC_Ch1_Countdown_tim_Off();
				
        gui_struct_para.current_strength = 0;					
				gui_struct_para.ok_flag = 0;
				BC_Ch1_Pwm_Off();
				BC_DAC1_Stop();
        GUI_Display_Ui_Main_Menu();
		}
}


/*****************************************************************************
  * Function:BC_VNS_gui_key_up_process
  * 		 
  * Description:VNS up key process
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_gui_key_up_process(void)
{
    if(gui_struct_para.menu_id == MENU_ID_MAIN)
		{
        if((4 > gui_struct_para.menu_mode_index) &&
					 (0 < gui_struct_para.menu_mode_index))
        {
            gui_struct_para.menu_mode_index++;
						GUI_Display_Ui_Main_Menu_Selec(gui_struct_para.menu_mode_index);
				    BC_DEBUG("4 menu_mode_index = %d\r\n", gui_struct_para.menu_mode_index);
				}
		}
		else if(gui_struct_para.menu_id == MENU_ID_STRENGTH)
		{
        if((VNS_STRENGTH_MAX >= gui_struct_para.current_strength) &&
					 (VNS_STRENGTH_MIN < gui_struct_para.current_strength))
		    {
		        if(MODE_C != gui_struct_para.menu_mode_index)
		        {
                gui_struct_para.current_strength--;
								BC_DAC1_Set_Level(gui_struct_para.current_strength); 
								if(0 == gui_struct_para.current_strength)
								{
                    BC_DAC1_Stop();
										#if !defined(_TEST_)
										BC_Ch1_Pwm_Off();
										#endif
								}
		        }
						//GUI_Display_Strength_Adjust_Val(110,160,gui_struct_para.current_strength);

						GUI_VNS_DrawImageNum_Test_Adjust(gui_struct_para.current_strength,1,0);

						BC_DEBUG("4 current_strength = %d\r\n", gui_struct_para.current_strength);
			  }
		}
		else if(gui_struct_para.menu_id == MENU_ID_STIMULATE)
		{
        if((VNS_STRENGTH_MAX >= gui_struct_para.current_strength) &&
					 ((VNS_STRENGTH_MIN+1) < gui_struct_para.current_strength))
		    {
		        if(MODE_C != gui_struct_para.menu_mode_index)
		        {
                gui_struct_para.current_strength--;
								BC_DAC1_Set_Level(gui_struct_para.current_strength);	
								if(0 == gui_struct_para.current_strength)
								{
                    BC_DAC1_Stop();
										BC_Ch1_Pwm_Off();
								}
		        }
						
						//GUI_Display_Strength_Adjust_Val(50,120,gui_struct_para.current_strength);
						GUI_VNS_DrawImageNum_Adjust(gui_struct_para.current_strength,0,0);

						BC_DEBUG("p4_%d current_strength = %d\r\n", gui_struct_para.menu_id,gui_struct_para.current_strength);
			  }
		}
		else if(gui_struct_para.menu_id == MENU_ID_CUSTOM)
		{
        BC_VNS_gui_key_4_custom_adjust_menu();
		}
}



/*****************************************************************************
  * Function:BC_VNS_gui_key_down_process
  * 		 
  * Description:VNS down key process
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_gui_key_down_process(void)
{
    if(gui_struct_para.menu_id == MENU_ID_MAIN)
		{
        if((4 >= gui_struct_para.menu_mode_index) &&
					 (1 < gui_struct_para.menu_mode_index))
        {
            gui_struct_para.menu_mode_index--;
						GUI_Display_Ui_Main_Menu_Selec(gui_struct_para.menu_mode_index);
				    BC_DEBUG("3 menu_mode_index = %d\r\n", gui_struct_para.menu_mode_index);
				}
		}
	  else if(gui_struct_para.menu_id == MENU_ID_STRENGTH)
		{
				if((VNS_STRENGTH_MAX > gui_struct_para.current_strength) &&
					 (VNS_STRENGTH_MIN <= gui_struct_para.current_strength))
				{
				    if(MODE_C != gui_struct_para.menu_mode_index)
				    {
						    gui_struct_para.current_strength++;
								if(1 == gui_struct_para.current_strength)
								{
                    BC_DAC1_Start();
										#if !defined(_TEST_)
										BC_Ch1_Pwm_On();
										#endif
								}
				        BC_DAC1_Set_Level(gui_struct_para.current_strength);
		            //GUI_Display_Strength_Adjust_Val(110,160,gui_struct_para.current_strength);
				    }
						//GUI_Display_Strength_Adjust_Val(110,160,gui_struct_para.current_strength);
						GUI_VNS_DrawImageNum_Test_Adjust(gui_struct_para.current_strength,1,0);
						BC_DEBUG("3 current_strength = %d\r\n", gui_struct_para.current_strength);
				}
		}
		else if(gui_struct_para.menu_id == MENU_ID_STIMULATE)
		{
				if((VNS_STRENGTH_MAX > gui_struct_para.current_strength) &&
					 (VNS_STRENGTH_MIN <= gui_struct_para.current_strength))
				{
				    if(MODE_C != gui_struct_para.menu_mode_index)
				    {
						    gui_struct_para.current_strength++;
								if(1 == gui_struct_para.current_strength)
								{
                    BC_DAC1_Start();
										BC_Ch1_Pwm_On();
								}
								BC_DAC1_Set_Level(gui_struct_para.current_strength);
				    }
						
						//GUI_Display_Strength_Adjust_Val(50,120,gui_struct_para.current_strength);
						GUI_VNS_DrawImageNum_Adjust(gui_struct_para.current_strength,0,0);
			
						BC_DEBUG("p3_%d current_strength = %d\r\n", gui_struct_para.menu_id,gui_struct_para.current_strength);
				}
		}
		else if(gui_struct_para.menu_id == MENU_ID_CUSTOM)
		{
		    BC_VNS_gui_key_3_custom_adjust_menu();
		}
}



/*****************************************************************************
  * Function:BC_VNS_pc_connect_after_process
  * 		 
  * Description:VNS after pc connect process
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_pc_connect_after_process(void)
{
		BC_VNS_mode_a_set_para();
																	
		BC_Ch1_Pwm_Off();
		BC_DAC1_Stop();
    BC_Timer3_Off();
		BC_Timer5_Off();
		
		g_data.start_stop_flag = 0;
}


#endif
/*****************************************************************************
  * Function:BC_vns_parse_rx_data
  * 		 
  * Description: parse usart1 rx data
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_parse_rx_data(void)
{
    uint8_t  type = USART1_RX_BUF[2];
		uint32_t crc_val = 0;
		uint8_t  crc_val_h = 0, crc_val_l = 0;

    if((0x55 == USART1_RX_BUF[0]) && (0xDB == USART1_RX_BUF[1]))
    {
        crc_val = BC_crc_cal_value(USART1_RX_BUF,(USART1_RX_STA-2));
				crc_val_h = (crc_val>>8)&0xFF;
				crc_val_l = (crc_val&0xFF);

				BC_DEBUG("RX[%02X][%02X] CRC[%02X][%02X]\r\n",USART1_RX_BUF[USART1_RX_STA-2],USART1_RX_BUF[USART1_RX_STA-1],crc_val_h,crc_val_l);

				if((crc_val_h == USART1_RX_BUF[USART1_RX_STA-2]) && (crc_val_l == USART1_RX_BUF[USART1_RX_STA-1]))
				{
            switch(type)
				    {
				    #if defined(_BC_FES_)
				        case DEVICE_FES:
									            BC_FES_parse_data(USART1_RX_BUF);
									            break;
						#else		
								case DEVICE_VNS:
									            BC_VNS_parse_data(USART1_RX_BUF);
							            		break;
						#endif							
								case DEVICE_CHECK:
									            if(0x01 == USART1_RX_BUF[8]) //55 DB FE 00 01 01 00 00 01 7F 67
									            {
									                g_data.is_pc = 1;
																	
                              #if defined(_BC_FES_)
																	BC_FES_pc_connect_after_process();
															#else
																	BC_VNS_pc_connect_after_process();
															#endif   
																	
																	GUI_Display_Logo_Picture();
									            }
                               
									            BC_check_response_data(USART1_RX_BUF[5],USART1_RX_BUF[8]);
							            		break;
															
										
								default:
							            		break;
										
						}
				}
    }
}


/*****************************************************************************
  * Function:BC_VNS_USART1_DATA_PROCESS
  * 		 
  * Description: parse usart1 rx data
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_VNS_USART1_DATA_PROCESS(void)
{
    uint8_t i = 0;

    if(ReviceState1)
    {
		    BC_DEBUG("\r\nR_X:[");
				for(i=0; i<USART1_RX_STA; i++)
				{
			  		BC_DEBUG("%02X",USART1_RX_BUF[i]);
				}
				BC_DEBUG("]\r\n");
				
        BC_VNS_parse_rx_data();
				USART1_RX_STA = 0;
        ReviceState1 = 0;
				memset(USART1_RX_BUF,0,USART_RX_LEN);
    }
}


/*****************************************************************************
  * Function:BC_countdown_time
  * 		 
  * Description:定时器倒计时
  *
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_countdown_time(void)
{
		#if defined(_BC_FES_) 
		BC_FES_countdown_time();
		#else
    BC_VNS_countdown_time();
		#endif
}


/*****************************************************************************
  * Function: BC_gui_key_ok_handle	  
  * 		 
  * Description: GUI界面按键(确认/暂停)处理 
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_gui_key_ok_handle(void)
{
    #if defined(_BC_FES_)
    BC_FES_gui_key_ok_process();
		#else
    BC_VNS_gui_key_ok_process();
		#endif	
}


/*****************************************************************************
  * Function: BC_gui_key_back_handle	  
  * 		 
  * Description: GUI界面按键(返回)处理 
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_gui_key_back_handle(void)
{
    #if defined(_BC_FES_)
    BC_FES_gui_key_back_process();
		#else
    BC_VNS_gui_key_back_process();
		#endif
}


/*****************************************************************************
  * Function: BC_gui_key_up_handle	  
  * 		 
  * Description: GUI界面按键(加)处理 
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_gui_key_up_handle(void)
{
    #if defined(_BC_FES_)
    BC_FES_gui_key_up_process();
		#else
    BC_VNS_gui_key_up_process();
		#endif
}


/*****************************************************************************
  * Function: BC_gui_key_down_handle	  
  * 		 
  * Description: GUI界面按键(减)处理 
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_gui_key_down_handle(void)
{
    #if defined(_BC_FES_)
    BC_FES_gui_key_down_process();
		#else
    BC_VNS_gui_key_down_process();
		#endif
}


/*****************************************************************************
  * Function: HAL_GPIO_EXTI_Callback	  
  * 		 
  * Description: 按键外部中断的回调函数 
  * 		 
  * Parameters:  
  * 		 [GPIO_Pin]
  * Return: 	 
  * 		 void
*****************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(g_data.is_pc)
    {
        GUI_Display_Ui_Main_Menu();
				g_data.is_pc = 0;
				if(BC_Ch1_Pwm_Onoff_Flag())
				{
						BC_Ch1_Pwm_Off();
						BC_DAC1_Stop();
				}
		#if defined(_BC_FES_)
				#if !defined(_BC_FES_SWITCH)
				BC_Ch2_Pwm_Off();
				BC_DAC2_Stop();
				#endif
		#endif
		}

		#if defined(_BC_FES_)
		BC_DEBUG("menu_id=%d is_pc=%d\r\n", gui_fes_struct_para.menu_id,g_data.is_pc);
		#else
		BC_DEBUG("menu_id=%d is_pc=%d\r\n", gui_struct_para.menu_id,g_data.is_pc);
		#endif
		//HAL_Delay(10);
    if(GPIO_Pin == KEY_3_Pin) //减 -> 返回
    {
        if(0 == HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin))
        {        
						//BC_VNS_gui_key_3_handle();
						BC_gui_key_back_handle();
			  }
		}
		else if(GPIO_Pin == KEY_4_Pin) //加 -> OK
		{
        if(0 == HAL_GPIO_ReadPin(KEY_4_GPIO_Port, KEY_4_Pin))
        {      
						//BC_VNS_gui_key_4_handle();
						BC_gui_key_ok_handle();
			  }
		}
    
    if(GPIO_Pin == KEY_1_Pin) //OK -> 加
    {   
        if(0 == HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin))
        {
            //BC_VNS_gui_key_1_handle();
						BC_gui_key_down_handle();
        }
		}
		else if(GPIO_Pin == KEY_2_Pin) //返回 -> 减
    {
        if(0 == HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin))
        {
            //BC_VNS_gui_key_2_handle();
						BC_gui_key_up_handle();
				}
		}
		else if(GPIO_Pin == POWER_KEY_Pin)
		{
         if(0 == HAL_GPIO_ReadPin(POWER_KEY_GPIO_Port, POWER_KEY_Pin))
         {
            g_data.power_off_key_count = 0;
         		BC_DEBUG("power off key -1-\r\n");
				 }
				 else
				 {
				 		//BC_DEBUG("power off ley 2\r\n");
				 }
		}
}  


void BC_FES_TEST(void)
{
#if 0
    if(g_turn_on_off_flag)
    {
		    if(g_tim2_flag)
		    {
		        //g_tim2_flag = 0;
		        //BC_Timer3_On();    
				}
				else
				{
		        BC_Timer3_Off();
						BC_Timer2_Off();
						g_turn_on_off_flag = 0;
						//g_tim2_flag = 0;
				}
    }
#endif
}


/*****************************************************************************
  * Function: BC_task_thread_1	  
  * 		 
  * Description: thread 1
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_task_thread_1(void)
{	
		BC_VNS_USART1_DATA_PROCESS();
}


/**************************************************************************
  * Function:	  
  * 		 BC_task
  * Description: 
  * 		 while(1) task
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_task(void)
{
    BC_task_thread_1();
}



