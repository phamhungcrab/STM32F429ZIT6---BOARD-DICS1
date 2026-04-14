/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Button_Pin GPIO_PIN_0
#define B1_Button_GPIO_Port GPIOA
#define B1_Button_EXTI_IRQn EXTI0_IRQn
#define LCD_D0_Pin GPIO_PIN_8
#define LCD_D0_GPIO_Port GPIOE
#define LCD_D1_Pin GPIO_PIN_9
#define LCD_D1_GPIO_Port GPIOE
#define LCD_D2_Pin GPIO_PIN_10
#define LCD_D2_GPIO_Port GPIOE
#define LCD_D3_Pin GPIO_PIN_11
#define LCD_D3_GPIO_Port GPIOE
#define LCD_D4_Pin GPIO_PIN_12
#define LCD_D4_GPIO_Port GPIOE
#define LCD_D5_Pin GPIO_PIN_13
#define LCD_D5_GPIO_Port GPIOE
#define LCD_D6_Pin GPIO_PIN_14
#define LCD_D6_GPIO_Port GPIOE
#define LCD_D7_Pin GPIO_PIN_15
#define LCD_D7_GPIO_Port GPIOE
#define HCSR04_Trig_Pin GPIO_PIN_14
#define HCSR04_Trig_GPIO_Port GPIOD
#define HCSR04_Echo_Pin GPIO_PIN_15
#define HCSR04_Echo_GPIO_Port GPIOD
#define HCSR04_Echo_EXTI_IRQn EXTI15_10_IRQn
#define LCD_RW_Pin GPIO_PIN_2
#define LCD_RW_GPIO_Port GPIOG
#define LCD_RS_Pin GPIO_PIN_3
#define LCD_RS_GPIO_Port GPIOG
#define LCD_E_Pin GPIO_PIN_4
#define LCD_E_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
