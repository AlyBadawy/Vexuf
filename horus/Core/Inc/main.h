/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#define Tamper_Pin GPIO_PIN_13
#define Tamper_GPIO_Port GPIOC
#define LSEin_Pin GPIO_PIN_14
#define LSEin_GPIO_Port GPIOC
#define LSEout_Pin GPIO_PIN_15
#define LSEout_GPIO_Port GPIOC
#define HSEin_Pin GPIO_PIN_0
#define HSEin_GPIO_Port GPIOH
#define HSEout_Pin GPIO_PIN_1
#define HSEout_GPIO_Port GPIOH
#define Boot1_Pin GPIO_PIN_2
#define Boot1_GPIO_Port GPIOB
#define ActDa_Pin GPIO_PIN_12
#define ActDa_GPIO_Port GPIOB
#define ActLa_Pin GPIO_PIN_13
#define ActLa_GPIO_Port GPIOB
#define ActCk_Pin GPIO_PIN_14
#define ActCk_GPIO_Port GPIOB
#define ActLd_Pin GPIO_PIN_15
#define ActLd_GPIO_Port GPIOB
#define MCO_Pin GPIO_PIN_8
#define MCO_GPIO_Port GPIOA
#define SWDio_Pin GPIO_PIN_13
#define SWDio_GPIO_Port GPIOA
#define SWCk_Pin GPIO_PIN_14
#define SWCk_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
