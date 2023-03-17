/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "sys.h"
#include "usart.h"
#include "GUI.h"

#define OUTPUT_ADC_VAL        (1)
#define BATTERY_FULL_ADC_VAL  (2400) //4.2V 电池满电
#define BATTERY_2_ADC_VAL     (2200) //电池2格（2200-2400）
#define BATTERY_LOW_ADC_VAL   (2035) //电池1格（2031-2200）低于3.3V
#define CHARGING_ADC_VAL      (4000) //高电平（4096）


u16 g_adcx = 0;
u16 g_adcx_battery = 0;
u16 g_adcx_charging = 0;


/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    PA2     ------> ADC1_IN2
    PA3     ------> ADC1_IN3
    PC5     ------> ADC1_IN15
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    PA2     ------> ADC1_IN2
    PA3     ------> ADC1_IN3
    PC5     ------> ADC1_IN15
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_5);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
u16 Get_Adc(u32 ch)   
{
    u16 val = 0;
    ADC_ChannelConfTypeDef ADC1_ChanConf;
		
    ADC1_ChanConf.Channel=ch;                                   
    ADC1_ChanConf.Rank=ADC_REGULAR_RANK_1;                                       
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;                   
    HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        
	
    HAL_ADC_Start(&hadc1);                               
	
    if(HAL_OK == HAL_ADC_PollForConversion(&hadc1,100))
    {
				val = (u16)HAL_ADC_GetValue(&hadc1);
		}

    return val;	        	
}


//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u32 ch,u8 times)
{
		u32 temp_val=0;
		u8 t;
		for(t=0;t<times;t++)
		{
			temp_val+=Get_Adc(ch);
			//HAL_Delay(5);
		}
		
		HAL_ADC_Stop(&hadc1);
		
		return temp_val/times;
} 


void Get_Adc_Val(u32 ch)
{
	  //float temp;
		
		g_adcx = Get_Adc_Average(ch,2);
    //temp=(float)adcx*(3.3/4096); 
    //BC_DEBUG("adc = %d\r\n",g_adcx);
}


u8 Get_Adc_Electrode_Connect_Status(void)
{
    u8 ret = 0;
		
    if(g_adcx > OUTPUT_ADC_VAL)
    {
    		ret = 1;
		}
		else
		{
				ret = 0;
		}

		return ret;
}


void Get_Adc_Battery_Val(u32 ch)
{
	  //float temp;
		
		g_adcx_battery = Get_Adc_Average(ch,2);
    //temp=(float)adcx*(3.3/4096); 
    BC_DEBUG("battery_adc = %d\r\n",g_adcx_battery);
}

u8 Get_Adc_Battery_Full_Status(void)
{
    u8 ret = 0;
		
    if(g_adcx_battery > BATTERY_FULL_ADC_VAL)
    {
    		ret = 1;
		}
		else
		{
				ret = 0;
		}

		return ret;
}


u8 Get_Adc_Battery_Poweroff_Status(void)
{
    static u8 count = 0;
		static u32 adcx = 0;
		u32 aver_adcx = 0;
    u8 ret = BATTERY_ICON_1;

    if(2 > count)
    {
				count++;
				adcx += g_adcx_battery;
    }
		else
		{
        count = 0;
				aver_adcx = adcx/2;
				BC_DEBUG("average battery_adc = %lu\r\n",aver_adcx);
				adcx = 0;
				
			  if(aver_adcx < (BATTERY_LOW_ADC_VAL - 5))
		    {
		    		ret = BATTERY_ICON_0;
				}
				else
				{
						ret = BATTERY_ICON_1;
				}
		}
		
		return ret;
}


u8 Get_Adc_Battery_Level_Status(void)
{
    u8 ret = 0;
		
		if(g_adcx_battery > BATTERY_FULL_ADC_VAL)
    {
				ret = BATTERY_ICON_3;
		}
		else if((g_adcx_battery < BATTERY_FULL_ADC_VAL) && (g_adcx_battery > BATTERY_2_ADC_VAL))
		{
				ret = BATTERY_ICON_2;
		}
		else if((g_adcx_battery > BATTERY_LOW_ADC_VAL) && (g_adcx_battery < BATTERY_2_ADC_VAL))
    {
    		ret = BATTERY_ICON_1;
		}
		else
		{
				ret = BATTERY_ICON_0;
		}

		return ret;
}


void Get_Adc_Charging_Val(u32 ch)
{
	  //float temp;
		
		g_adcx_charging = Get_Adc_Average(ch,2);
    //temp=(float)adcx*(3.3/4096); 
    BC_DEBUG("charging_adc = %d\r\n",g_adcx_charging);
}


u8 Get_Adc_Charging_Status(void)
{
    u8 ret = 0;
		
    if(g_adcx_charging > CHARGING_ADC_VAL)
    {
    		ret = 1;
		}
		else
		{
				ret = 0;
		}

		return ret;
}


/* USER CODE END 1 */
