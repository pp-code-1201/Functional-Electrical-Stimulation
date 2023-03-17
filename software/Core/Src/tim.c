/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "usart.h"
#include "gpio.h"
#include "app.h"
#include "dac.h"
#include "gui.h"
#include "adc.h"


uint8_t g_tim3_onoff_flag = 0;
/* USER CODE END 0 */

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7200-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  BC_Timer2_On();
  /* USER CODE END TIM2_Init 2 */

}

/* TIM5 init function */
void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 7200-1;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 10000-1;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspInit 0 */

  /* USER CODE END TIM4_MspInit 0 */
    /* TIM4 clock enable */
    __HAL_RCC_TIM4_CLK_ENABLE();

    /* TIM4 interrupt Init */
    HAL_NVIC_SetPriority(TIM4_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
  /* USER CODE BEGIN TIM4_MspInit 1 */

  /* USER CODE END TIM4_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspInit 0 */

  /* USER CODE END TIM5_MspInit 0 */
    /* TIM5 clock enable */
    __HAL_RCC_TIM5_CLK_ENABLE();

    /* TIM5 interrupt Init */
    HAL_NVIC_SetPriority(TIM5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);
  /* USER CODE BEGIN TIM5_MspInit 1 */

  /* USER CODE END TIM5_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspDeInit 0 */

  /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();

    /* TIM4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
  /* USER CODE BEGIN TIM4_MspDeInit 1 */

  /* USER CODE END TIM4_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspDeInit 0 */

  /* USER CODE END TIM5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM5_CLK_DISABLE();

    /* TIM5 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM5_IRQn);
  /* USER CODE BEGIN TIM5_MspDeInit 1 */

  /* USER CODE END TIM5_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

#if 1
void MX_TIM3_Init(uint32_t period)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 72-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_DOWN;
  htim3.Init.Period = period;//125-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

#if defined(_BC_FES_)
void MX_TIM4_Init(uint32_t period)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 72-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = period;//125-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}


/*****************************************************************************
  * Function:BC_Timer4_On
  * 		 
  * Description: Timer on PWM2
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Timer4_On(void)
{
		HAL_TIM_Base_Start_IT(&htim4); 

		BC_DEBUG("Tim4 on\r\n");
}


/*****************************************************************************
  * Function:BC_Timer4_Off
  * 		 
  * Description: Timer off PWM2
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Timer4_Off(void)
{
		HAL_TIM_Base_Stop_IT(&htim4); 

		BC_DEBUG("Tim4 off\r\n");
}



/*****************************************************************************
  * Function:BC_Ch2_Pwm_On
  * 		 
  * Description: tim4 ch2 pwm on
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Ch2_Pwm_On(void)
{
    PWM2_GPIO1_SET;
		PWM2_GPIO2_SET;
		BC_Timer4_On();
}


/*****************************************************************************
  * Function:BC_Ch2_Pwm_Off
  * 		 
  * Description: tim4 ch2 pwm off
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Ch2_Pwm_Off(void)
{
		BC_Timer4_Off();
		PWM2_GPIO1_RESET;
		PWM2_GPIO2_RESET;
}


/*****************************************************************************
  * Function:BC_Fes_Ch_Timer_On
  * 		 
  * Description: tim5 ch1&ch2 count down tim on
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Fes_Ch_Timer_On(void)
{	
		BC_Timer5_On();
}


/*****************************************************************************
  * Function:BC_Fes_Ch_Timer_Off
  * 		 
  * Description: tim5 ch1&ch2 count down tim off
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Fes_Ch_Timer_Off(void)
{	
		BC_Timer5_Off();
}


#endif

#endif


/*****************************************************************************
  * Function:BC_Timer3_Ch1_Pwm_Process
  * 		 
  * Description: tim3 ch1 pwm process
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Timer3_Ch1_Pwm_Process(void)
{
    static uint16_t tim_count = 0;

#if defined(_BC_FES_)	

    if(tim_count < g_data.pwm1_cycle)
    {
        tim_count++;
    }
		else
		{
        tim_count = 0;
		}
		
    if(1 == tim_count)
    {
        BC_continuous_action_pwm_dac(tim_count);
        PWM1_GPIO1_RESET;
				PWM1_GPIO2_SET;
    }
		else if(2 == tim_count)
		{
				BC_continuous_action_pwm_dac_half(tim_count);
		    PWM1_GPIO1_SET;
			  PWM1_GPIO2_RESET;
				//Get_Adc_Val(ADC_CHANNEL_1);
		} 
    else if(4 == tim_count)
    {
        BC_DAC1_Set_Level(0);
        PWM1_GPIO1_SET;
			  PWM1_GPIO2_SET;
		}
				
#else

		if(tim_count < g_data.pwm1_cycle) //400 50ms
    {
        tim_count++;
		}
		else
		{
        tim_count = 0;
		}

		if(1 == tim_count)
		{
 				PWM1_GPIO1_SET;
				Get_Adc_Val(ADC_CHANNEL_1);
		}
		else if(2 == tim_count)
		{
        PWM1_GPIO1_RESET;
			  PWM1_GPIO2_SET;
		}
		
		if(3 == tim_count)
		{
				PWM1_GPIO1_RESET;
				PWM1_GPIO2_RESET;
		}
		
#endif
}




/*****************************************************************************
  * Function:BC_Timer4_Ch2_Pwm_Process
  * 		 
  * Description: tim4 ch2 pwm process
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Timer4_Ch2_Pwm_Process(void)
{
#if defined(_BC_FES_)
    static uint16_t tim_count = 0;
		
		if(tim_count < g_data.pwm2_cycle)
    {
        tim_count++;
    }
		else
		{
        tim_count = 0;
		}
		
    if(1 == tim_count)
    {
        BC_DAC2_Set_Level(gui_fes_struct_para.chb_current_strength);
        PWM2_GPIO1_RESET;
				PWM2_GPIO2_SET;  
    }
		else if(2 == tim_count)
		{
		    BC_DAC2_Set_Level(gui_fes_struct_para.chb_current_strength/2);
		    PWM2_GPIO1_SET;
			  PWM2_GPIO2_RESET;  
		} 
    else if(4 == tim_count)
    {
        BC_DAC2_Set_Level(0);
        PWM2_GPIO1_SET;
			  PWM2_GPIO2_SET;
		}
#endif
}


/*****************************************************************************
  * Function:HAL_TIM_PeriodElapsedCallback
  * 		 
  * Description: tim callback function
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		if(htim->Instance == TIM2)
		{
	  		g_data.hr_cycle++; //与PC通信心跳周期
	  		BC_send_hr_cycle();
		}
		else if(htim->Instance == TIM3)
		{
		    BC_Timer3_Ch1_Pwm_Process();
		}
		else if(htim->Instance == TIM4)
		{
		    BC_Timer4_Ch2_Pwm_Process();
		}
		else if(htim->Instance == TIM5)	
		{
				BC_countdown_time();
		}
}


/*****************************************************************************
  * Function:BC_Timer2_On
  * 		 
  * Description: Timer on
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Timer2_On(void)
{
		HAL_TIM_Base_Start_IT(&htim2); 

		BC_DEBUG("Tim2 on\r\n");
}


/*****************************************************************************
  * Function:BC_Timer2_Off
  * 		 
  * Description: Timer off
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Timer2_Off(void)
{
		HAL_TIM_Base_Stop_IT(&htim2); 

		BC_DEBUG("Tim2 off\r\n");
}


/*****************************************************************************
  * Function:BC_Timer3_On
  * 		 
  * Description: Timer on PWM1
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Timer3_On(void)
{
		HAL_TIM_Base_Start_IT(&htim3); 

		BC_DEBUG("Tim3 on\r\n");
}


/*****************************************************************************
  * Function:BC_Timer3_Off
  * 		 
  * Description: Timer off PWM1
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Timer3_Off(void)
{
		HAL_TIM_Base_Stop_IT(&htim3); 

		BC_DEBUG("Tim3 off\r\n");
}


/*****************************************************************************
  * Function:BC_Timer5_On
  * 		 
  * Description: Timer on
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Timer5_On(void)
{
		HAL_TIM_Base_Start_IT(&htim5); 

		BC_DEBUG("Tim5 on\r\n");
}


/*****************************************************************************
  * Function:BC_Timer5_Off
  * 		 
  * Description: Timer off
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Timer5_Off(void)
{
		HAL_TIM_Base_Stop_IT(&htim5); 

		BC_DEBUG("Tim5 off\r\n");
}


/*****************************************************************************
  * Function:BC_Ch1_Pwm_On
  * 		 
  * Description: tim3 ch1 pwm on
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Ch1_Pwm_On(void)
{
    g_tim3_onoff_flag = 1;
    PWM1_GPIO1_SET;
	  PWM1_GPIO2_SET;
		BC_Timer3_On();
}


/*****************************************************************************
  * Function:BC_Ch1_Pwm_Off
  * 		 
  * Description: tim3 ch1 pwm off
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Ch1_Pwm_Off(void)
{
    g_tim3_onoff_flag = 0;
    BC_Timer3_Off();
    PWM1_GPIO1_RESET;
	  PWM1_GPIO2_RESET;
}


/*****************************************************************************
  * Function:BC_Ch1_Countdown_tim_On
  * 		 
  * Description: tim5 ch1 count down tim on
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Ch1_Countdown_tim_On(void)
{	
		BC_Timer5_On();
}


/*****************************************************************************
  * Function:BC_Ch1_Countdown_tim_On
  * 		 
  * Description: tim5 ch1 count down tim off
  * 		 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_Ch1_Countdown_tim_Off(void)
{
		BC_Timer5_Off();
}


uint8_t BC_Ch1_Pwm_Onoff_Flag(void)
{
    return g_tim3_onoff_flag;
}

/* USER CODE END 1 */
