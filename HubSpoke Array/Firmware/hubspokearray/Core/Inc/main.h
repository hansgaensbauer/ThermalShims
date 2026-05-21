/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
void DEBUG_PRINT(char * format, ...);
void printtemps(uint32_t * temps);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define WR_Pin GPIO_PIN_1
#define WR_GPIO_Port GPIOA
#define EN_Pin GPIO_PIN_3
#define EN_GPIO_Port GPIOA
#define DAC_Pin GPIO_PIN_4
#define DAC_GPIO_Port GPIOA
#define A0_Pin GPIO_PIN_5
#define A0_GPIO_Port GPIOA
#define A1_Pin GPIO_PIN_6
#define A1_GPIO_Port GPIOA
#define A2_Pin GPIO_PIN_7
#define A2_GPIO_Port GPIOA
#define CS_DRIVE_Pin GPIO_PIN_0
#define CS_DRIVE_GPIO_Port GPIOB
#define CS_TEMP_Pin GPIO_PIN_1
#define CS_TEMP_GPIO_Port GPIOB
#define A4_Pin GPIO_PIN_2
#define A4_GPIO_Port GPIOB
#define A3_Pin GPIO_PIN_8
#define A3_GPIO_Port GPIOA
#define LD1_Pin GPIO_PIN_5
#define LD1_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_6
#define LD2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

//#define MAIN_BOARD

#define HEATER_RETENTION 100

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
