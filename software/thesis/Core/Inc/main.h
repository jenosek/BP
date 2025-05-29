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
#include "stm32h7xx_hal.h"

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
void USB_CDC_RxHandler(uint8_t*, uint32_t);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NTC_2__Pin GPIO_PIN_0
#define NTC_2__GPIO_Port GPIOA
#define NTC_2_A1_Pin GPIO_PIN_1
#define NTC_2_A1_GPIO_Port GPIOA
#define INT_N_Pin GPIO_PIN_7
#define INT_N_GPIO_Port GPIOA
#define CC1_Pin GPIO_PIN_4
#define CC1_GPIO_Port GPIOC
#define CC2_Pin GPIO_PIN_5
#define CC2_GPIO_Port GPIOC
#define ADC_VBUS_Pin GPIO_PIN_0
#define ADC_VBUS_GPIO_Port GPIOB
#define FLT__Pin GPIO_PIN_1
#define FLT__GPIO_Port GPIOB
#define COMMAND_DONE_Pin GPIO_PIN_9
#define COMMAND_DONE_GPIO_Port GPIOE
#define nIRQ_Pin GPIO_PIN_10
#define nIRQ_GPIO_Port GPIOE
#define nIRQ_EXTI_IRQn EXTI15_10_IRQn
#define SPI4_nSEL_Pin GPIO_PIN_11
#define SPI4_nSEL_GPIO_Port GPIOE
#define DCMI_3V3_Pin GPIO_PIN_15
#define DCMI_3V3_GPIO_Port GPIOE
#define SDN_Pin GPIO_PIN_12
#define SDN_GPIO_Port GPIOB
#define DCMI_RST_Pin GPIO_PIN_9
#define DCMI_RST_GPIO_Port GPIOD
#define DCMI_PWDN_Pin GPIO_PIN_10
#define DCMI_PWDN_GPIO_Port GPIOD
#define DCMI_MCLK_Pin GPIO_PIN_8
#define DCMI_MCLK_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
