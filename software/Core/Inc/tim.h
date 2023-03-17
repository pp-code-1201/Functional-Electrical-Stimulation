/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "sys.h"
/* USER CODE END Includes */

extern TIM_HandleTypeDef htim2;

extern TIM_HandleTypeDef htim3;

extern TIM_HandleTypeDef htim4;

extern TIM_HandleTypeDef htim5;

/* USER CODE BEGIN Private defines */
#if defined(_BC_FES_)
#define  PERIOD_VALUE       (uint32_t)(100 - 1)  /* tim3 tim4 中断周期100us */ 
#else
#define  PERIOD_VALUE       (uint32_t)(125 - 1)  /* tim3 中断周期125us */ 
#endif

/* USER CODE END Private defines */

void MX_TIM2_Init(void);
void MX_TIM5_Init(void);

/* USER CODE BEGIN Prototypes */
void MX_TIM2_Init(void);
void MX_TIM3_Init(uint32_t period);
void MX_TIM4_Init(uint32_t period);
void MX_TIM5_Init(void);

#if defined(_BC_FES_)
void BC_Timer4_On(void);
void BC_Timer4_Off(void);
void BC_Ch2_Pwm_On(void);
void BC_Ch2_Pwm_Off(void);
void BC_Fes_Ch_Timer_On(void);
void BC_Fes_Ch_Timer_Off(void);
#endif


void BC_Timer2_On(void);
void BC_Timer2_Off(void);
void BC_Timer3_On(void);
void BC_Timer3_Off(void);

void BC_Timer5_On(void);
void BC_Timer5_Off(void);

void BC_Ch1_Pwm_On(void);
void BC_Ch1_Pwm_Off(void);
void BC_Ch1_Countdown_tim_On(void);
void BC_Ch1_Countdown_tim_Off(void);
uint8_t BC_Ch1_Pwm_Onoff_Flag(void);



/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

