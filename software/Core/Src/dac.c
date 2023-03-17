/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dac.c
  * @brief   This file provides code for the configuration
  *          of the DAC instances.
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
#include "dac.h"

/* USER CODE BEGIN 0 */
#include "usart.h"
#include "app.h"
#include "sys.h"
#include "GUI.h"
/* USER CODE END 0 */

DAC_HandleTypeDef hdac;

/* DAC init function */
void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT2 config
  */
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(dacHandle->Instance==DAC)
  {
  /* USER CODE BEGIN DAC_MspInit 0 */

  /* USER CODE END DAC_MspInit 0 */
    /* DAC clock enable */
    __HAL_RCC_DAC_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    PA5     ------> DAC_OUT2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN DAC_MspInit 1 */

  /* USER CODE END DAC_MspInit 1 */
  }
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* dacHandle)
{

  if(dacHandle->Instance==DAC)
  {
  /* USER CODE BEGIN DAC_MspDeInit 0 */

  /* USER CODE END DAC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DAC_CLK_DISABLE();

    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    PA5     ------> DAC_OUT2
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5);

  /* USER CODE BEGIN DAC_MspDeInit 1 */

  /* USER CODE END DAC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/*****************************************************************************
  * Function:	  
  * 		 BC_DAC1_Set_Val
  * Description: 
  * 		 Set the DAC output voltage value 
  *      vol:0~3300,代表0~3.3V
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_DAC1_Set_Val(uint16_t vol)
{   
		uint32_t temp=vol;
		//temp/=1000;
		//temp=temp*4096/3.3;
	  HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp);//12位右对齐数据格式设置DAC值
	  //BC_DEBUG("DAC1 set voltage = %d\r\n",temp);
}


/*****************************************************************************
  * Function:	  
  * 		 BC_DAC1_Set_Level
  * Description: 
  * 		 Set the DAC output voltage value for strength 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_DAC1_Set_Level(uint8_t index)
{
    uint32_t temp = 0;
		
		#if defined(_BC_FES_)
		if((MODE_FES_LOWER == gui_fes_struct_para.cha_mode_index) || (MODE_FES_LOWER == gui_fes_struct_para.chb_mode_index))
		{
				temp = 4096/(FES_STRENGTH_MAX+FES_LOWER_STRENGTH_MAX_OFFSET);
				//BC_DEBUG("DAC1 LOWER set strength = %d\r\n",index);
		}
		else
		{
    		temp = 4096/(FES_STRENGTH_MAX+FES_STRENGTH_MAX_OFFSET);
				//BC_DEBUG("DAC1 set strength = %d\r\n",index);
		}
		#else
		temp = 4096/(VNS_STRENGTH_MAX+VNS_STRENGTH_MAX_OFFSET);
		#endif
    BC_DAC1_Set_Val(index*temp);	
}


/*****************************************************************************
  * Function:	  
  * 		 BC_DAC1_Start
  * Description: 
  * 		 Set DAC1 Start 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_DAC1_Start(void)
{
		HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
		BC_DEBUG("DAC1 START\r\n");
}


/*****************************************************************************
  * Function:	  
  * 		 BC_DAC1_Start
  * Description: 
  * 		 Set DAC1 Stop 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_DAC1_Stop(void)
{
    BC_DAC1_Set_Level(0);
		HAL_DAC_Stop(&hdac,DAC_CHANNEL_1);
		BC_DEBUG("DAC1 STOP\r\n");
}


#if defined(_BC_FES_)

/*****************************************************************************
  * Function:	  
  * 		 BC_DAC2_Set_Val
  * Description: 
  * 		 Set the DAC output voltage value 
  *      vol:0~3300,代表0~3.3V
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_DAC2_Set_Val(uint16_t vol)
{   
		double temp=vol;
		//temp/=1000;
		//temp=temp*4096/3.3;
	  HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,temp);//12位右对齐数据格式设置DAC值
	  //BC_DEBUG("DAC2 set voltage = %1.2fV\r\n",temp/1000);
}



/*****************************************************************************
  * Function:	  
  * 		 BC_DAC2_Set_Level
  * Description: 
  * 		 Set the DAC output voltage value for strength 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_DAC2_Set_Level(uint8_t index)
{
    uint32_t temp = 0;
		
    //BC_DEBUG("DAC2 set strength = %d\r\n",index);

		temp = 4096/(FES_STRENGTH_MAX+FES_STRENGTH_MAX_OFFSET);
    BC_DAC2_Set_Val(index*temp);	
}



/*****************************************************************************
  * Function:	  
  * 		 BC_DAC2_Start
  * Description: 
  * 		 Set DAC2 Start 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_DAC2_Start(void)
{
		HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
		BC_DEBUG("DAC2 START\r\n");
}


/*****************************************************************************
  * Function:	  
  * 		 BC_DAC2_Stop
  * Description: 
  * 		 Set DAC2 Stop 
  * Parameters:  
  * 		 [void]
  * Return: 	 
  * 		 void
*****************************************************************************/
void BC_DAC2_Stop(void)
{
		HAL_DAC_Stop(&hdac,DAC_CHANNEL_2);
		BC_DEBUG("DAC2 STOP\r\n");
}
#endif


/* USER CODE END 1 */
