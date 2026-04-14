/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
volatile uint8_t blue_button_pressed = 0;
volatile uint8_t echo_rising_captured = 0;
volatile uint32_t echo_pulse_us = 0;
volatile uint8_t echo_measurement_done = 0;

float distance_cm = 0.0f;
char uart_msg[100];
uint32_t last_button_tick = 0;

uint32_t last_measure_tick = 0;
uint8_t green_pwm_duty = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM6_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void HCSR04_SendTrig(void);
float HCSR04_CalcDistanceCm(uint32_t pulse_us);
void Debug_Print(char *msg);
void DelayUs(uint16_t us);

void LCD_Set_Data(uint8_t value);
void LCD_Send_Command(uint8_t cmd);
void LCD_Write_One_Char(uint8_t c);
void LCD_Write_String(char *str);
void LCD_Goto_XY(uint8_t row, uint8_t col);
void LCD_Clear(void);
void LCD_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Debug_Print(char *msg)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
}

void DelayUs(uint16_t us)
{
  __HAL_TIM_SET_COUNTER(&htim6, 0);
  while (__HAL_TIM_GET_COUNTER(&htim6) < us)
  {
  }
}

void HCSR04_SendTrig(void)
{
  HAL_GPIO_WritePin(HCSR04_Trig_GPIO_Port, HCSR04_Trig_Pin, GPIO_PIN_RESET);
  DelayUs(2);

  HAL_GPIO_WritePin(HCSR04_Trig_GPIO_Port, HCSR04_Trig_Pin, GPIO_PIN_SET);
  DelayUs(15);

  HAL_GPIO_WritePin(HCSR04_Trig_GPIO_Port, HCSR04_Trig_Pin, GPIO_PIN_RESET);
}

float HCSR04_CalcDistanceCm(uint32_t pulse_us)
{
  return pulse_us / 58.0f;
}

void LCD_Set_Data(uint8_t value)
{
  HAL_GPIO_WritePin(LCD_D0_GPIO_Port, LCD_D0_Pin, (value & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_D1_GPIO_Port, LCD_D1_Pin, (value & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_D2_GPIO_Port, LCD_D2_Pin, (value & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_D3_GPIO_Port, LCD_D3_Pin, (value & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (value & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (value & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (value & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (value & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void LCD_Send_Command(uint8_t cmd)
{
  LCD_Set_Data(cmd);

  HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);
}

void LCD_Write_One_Char(uint8_t c)
{
  LCD_Set_Data(c);

  HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);
}

void LCD_Write_String(char *str)
{
  while (*str)
  {
    LCD_Write_One_Char((uint8_t)(*str));
    str++;
  }
}

void LCD_Goto_XY(uint8_t row, uint8_t col)
{
  if (row == 0)
  {
    LCD_Send_Command(0x80 + col);
  }
  else
  {
    LCD_Send_Command(0xC0 + col);
  }
}

void LCD_Clear(void)
{
  LCD_Send_Command(0x01);
  HAL_Delay(2);
}

void LCD_Init(void)
{
  HAL_Delay(40);

  LCD_Send_Command(0x38);
  LCD_Send_Command(0x0C);
  LCD_Send_Command(0x01);
  LCD_Send_Command(0x06);
  LCD_Send_Command(0x80);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim6);
  HAL_GPIO_WritePin(HCSR04_Trig_GPIO_Port, HCSR04_Trig_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);

  LCD_Init();
  LCD_Clear();
  LCD_Goto_XY(0, 0);
  LCD_Write_String("MINH GIANG");
  LCD_Goto_XY(1, 0);
  LCD_Write_String("Distance:      ");

  Debug_Print("HC-SR04 continuous mode started.\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if ((HAL_GetTick() - last_measure_tick) >= 100)
	  {
	    last_measure_tick = HAL_GetTick();

	    echo_rising_captured = 0;
	    echo_pulse_us = 0;
	    echo_measurement_done = 0;

	    HCSR04_SendTrig();
	  }

	  if (echo_measurement_done == 1)
	  {
	    uint32_t distance_x100;

	    echo_measurement_done = 0;

	    distance_cm = HCSR04_CalcDistanceCm(echo_pulse_us);
	    distance_x100 = (echo_pulse_us * 100UL) / 58UL;

	    sprintf(uart_msg,
	            "Pulse = %lu us, Distance = %lu.%02lu cm\r\n",
	            echo_pulse_us,
	            distance_x100 / 100UL,
	            distance_x100 % 100UL);
	    Debug_Print(uart_msg);

	    LCD_Goto_XY(0, 0);
	    LCD_Write_String("Distance Sensor ");

	    LCD_Goto_XY(1, 0);
	    sprintf(uart_msg, "%lu.%02lu cm      ",
	            distance_x100 / 100UL,
	            distance_x100 % 100UL);
	    LCD_Write_String(uart_msg);

	    if (distance_cm < 20.0f)
	    {
	      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
	      green_pwm_duty = 0;
	    }
	    else
	    {
	      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);

	      if (distance_cm > 50.0f)
	      {
	        green_pwm_duty = 100;
	      }
	      else
	      {
	        green_pwm_duty = (uint8_t)(((distance_cm - 20.0f) * 100.0f) / 30.0f);
	      }
	    }
	  }

	  if (green_pwm_duty >= 100)
	  {
	    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
	  }
	  else if (green_pwm_duty == 0)
	  {
	    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
	  }
	  else
	  {
	    uint32_t pwm_phase = HAL_GetTick() % 10;
	    uint32_t pwm_on_time = (green_pwm_duty * 10U) / 100U;

	    if (pwm_phase < pwm_on_time)
	    {
	      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
	    }
	    else
	    {
	      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
	    }
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 89;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 65535;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LCD_D0_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_D3_Pin
                          |LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(HCSR04_Trig_GPIO_Port, HCSR04_Trig_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, LCD_RW_Pin|LCD_RS_Pin|LCD_E_Pin|GPIO_PIN_13
                          |GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Button_Pin */
  GPIO_InitStruct.Pin = B1_Button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_Button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D0_Pin LCD_D1_Pin LCD_D2_Pin LCD_D3_Pin
                           LCD_D4_Pin LCD_D5_Pin LCD_D6_Pin LCD_D7_Pin */
  GPIO_InitStruct.Pin = LCD_D0_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_D3_Pin
                          |LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : HCSR04_Trig_Pin */
  GPIO_InitStruct.Pin = HCSR04_Trig_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HCSR04_Trig_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : HCSR04_Echo_Pin */
  GPIO_InitStruct.Pin = HCSR04_Echo_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(HCSR04_Echo_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RW_Pin LCD_RS_Pin LCD_E_Pin PG13
                           PG14 */
  GPIO_InitStruct.Pin = LCD_RW_Pin|LCD_RS_Pin|LCD_E_Pin|GPIO_PIN_13
                          |GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == B1_Button_Pin)
  {
    uint32_t now = HAL_GetTick();

    if ((now - last_button_tick) > 200)
    {
      blue_button_pressed = 1;
      last_button_tick = now;
    }
  }

  if (GPIO_Pin == HCSR04_Echo_Pin)
  {
    if (HAL_GPIO_ReadPin(HCSR04_Echo_GPIO_Port, HCSR04_Echo_Pin) == GPIO_PIN_SET)
    {
      __HAL_TIM_SET_COUNTER(&htim6, 0);
      echo_rising_captured = 1;
    }
    else
    {
      if (echo_rising_captured == 1)
      {
        echo_pulse_us = __HAL_TIM_GET_COUNTER(&htim6);
        echo_rising_captured = 0;
        echo_measurement_done = 1;
      }
    }
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
