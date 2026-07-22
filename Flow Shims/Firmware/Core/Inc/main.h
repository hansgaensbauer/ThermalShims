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
#include "stm32l0xx_hal.h"

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

void debug_printf(const char *fmt, ...);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define A1_Pin GPIO_PIN_1
#define A1_GPIO_Port GPIOA
#define A0_Pin GPIO_PIN_3
#define A0_GPIO_Port GPIOA
#define A2_Pin GPIO_PIN_5
#define A2_GPIO_Port GPIOA
#define A3_Pin GPIO_PIN_6
#define A3_GPIO_Port GPIOA
#define VD_EN_Pin GPIO_PIN_7
#define VD_EN_GPIO_Port GPIOA
#define EN0_1_Pin GPIO_PIN_0
#define EN0_1_GPIO_Port GPIOB
#define EN1_1_Pin GPIO_PIN_1
#define EN1_1_GPIO_Port GPIOB
#define EN0_0_Pin GPIO_PIN_8
#define EN0_0_GPIO_Port GPIOA
#define VTEMP_Pin GPIO_PIN_9
#define VTEMP_GPIO_Port GPIOA
#define EN1_0_Pin GPIO_PIN_10
#define EN1_0_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_5
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_6
#define LED2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define PRINT

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
