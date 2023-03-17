/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
//PWM output
#define PWM1_GPIO1_SET			GPIOB->BSRR=1<<0//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)
#define PWM1_GPIO1_RESET		GPIOB->BRR=1<<0//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)
#define PWM1_GPIO2_SET			GPIOB->BSRR=1<<1//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)
#define PWM1_GPIO2_RESET		GPIOB->BRR=1<<1//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)

#define PWM2_GPIO1_SET			GPIOB->BSRR=1<<8//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET)
#define PWM2_GPIO1_RESET		GPIOB->BRR=1<<8//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET)
#define PWM2_GPIO2_SET			GPIOB->BSRR=1<<9//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET)
#define PWM2_GPIO2_RESET		GPIOB->BRR=1<<9//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET)


//LED output
#define LED_BLUE_ON					HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin,GPIO_PIN_SET)
#define LED_BLUE_OFF        HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin,GPIO_PIN_RESET)
#define LED_YELLOW_ON				HAL_GPIO_WritePin(LED_Y_GPIO_Port,LED_Y_Pin,GPIO_PIN_SET)
#define LED_YELLOW_OFF      HAL_GPIO_WritePin(LED_Y_GPIO_Port,LED_Y_Pin,GPIO_PIN_RESET)
#define LED_RED_ON					HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET)
#define LED_RED_OFF         HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_RESET)

//POWER
#define POWER_ON            HAL_GPIO_WritePin(GPIOB, ON_OFF_Pin,GPIO_PIN_SET)
#define POWER_OFF           HAL_GPIO_WritePin(GPIOB, ON_OFF_Pin,GPIO_PIN_RESET)

//CHANNEL SWITCH
#define SWITCH_CHA          HAL_GPIO_WritePin(GPIOE, channel_switch_Pin, GPIO_PIN_RESET)
#define SWITCH_CHB          HAL_GPIO_WritePin(GPIOE, channel_switch_Pin, GPIO_PIN_SET)

//charge full
#define CHARGE_FULL_STATUS  HAL_GPIO_ReadPin(charge_full_GPIO_Port,charge_full_Pin)  //1,charge full
//charging 
#define CHARGING_STATUS     HAL_GPIO_ReadPin(charging_GPIO_Port,charging_Pin)  //1,charging

                          

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

