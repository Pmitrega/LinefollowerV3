/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PDN_Pin GPIO_PIN_14
#define PDN_GPIO_Port GPIOC
#define WAKEUP_Pin GPIO_PIN_15
#define WAKEUP_GPIO_Port GPIOC
#define SENSOR_ON_Pin GPIO_PIN_3
#define SENSOR_ON_GPIO_Port GPIOC
#define ADC_BATT_Pin GPIO_PIN_0
#define ADC_BATT_GPIO_Port GPIOA
#define AIN1_Pin GPIO_PIN_10
#define AIN1_GPIO_Port GPIOB
#define AIN2_Pin GPIO_PIN_12
#define AIN2_GPIO_Port GPIOB
#define BIN1_Pin GPIO_PIN_13
#define BIN1_GPIO_Port GPIOB
#define BIN2_Pin GPIO_PIN_14
#define BIN2_GPIO_Port GPIOB
#define STBY_TB_Pin GPIO_PIN_15
#define STBY_TB_GPIO_Port GPIOB
#define ENCB_2_Pin GPIO_PIN_6
#define ENCB_2_GPIO_Port GPIOC
#define ENCB_1_Pin GPIO_PIN_7
#define ENCB_1_GPIO_Port GPIOC
#define PWMB_Pin GPIO_PIN_8
#define PWMB_GPIO_Port GPIOC
#define PWMA_Pin GPIO_PIN_9
#define PWMA_GPIO_Port GPIOC
#define ENCA_2_Pin GPIO_PIN_8
#define ENCA_2_GPIO_Port GPIOA
#define ENCA_1_Pin GPIO_PIN_9
#define ENCA_1_GPIO_Port GPIOA
#define BUT2_Pin GPIO_PIN_15
#define BUT2_GPIO_Port GPIOA
#define BUT1_Pin GPIO_PIN_10
#define BUT1_GPIO_Port GPIOC
#define LED4_Pin GPIO_PIN_2
#define LED4_GPIO_Port GPIOD
#define LED3_Pin GPIO_PIN_3
#define LED3_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_4
#define LED2_GPIO_Port GPIOB
#define LED1_PWM_Pin GPIO_PIN_7
#define LED1_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define TRUE (1)
#define FALSE (0)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
