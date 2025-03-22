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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "link_list.h"
#include "argb_common.h"
#include "argb_led.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define get_length(a) (uint32_t)(sizeof(a)/sizeof(a[0]))
  extern uint8_t SB;
  extern int8_single_chain *now_data;
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
#define PA2_TIM2_GPIO_input_Pin GPIO_PIN_2
#define PA2_TIM2_GPIO_input_GPIO_Port GPIOA
#define PA6_TIM16_RA99_PWM1_Pin GPIO_PIN_6
#define PA6_TIM16_RA99_PWM1_GPIO_Port GPIOA
#define PA7_TIM17_RA99_PWM2_Pin GPIO_PIN_7
#define PA7_TIM17_RA99_PWM2_GPIO_Port GPIOA
#define PB1_TIM3_CH4_2811_2812_Pin GPIO_PIN_1
#define PB1_TIM3_CH4_2811_2812_GPIO_Port GPIOB
#define PB12_Cascade_detection_Pin GPIO_PIN_12
#define PB12_Cascade_detection_GPIO_Port GPIOB
#define PA8_CAN_STB_GPIO_output_Pin GPIO_PIN_8
#define PA8_CAN_STB_GPIO_output_GPIO_Port GPIOA
#define PA15_WIFI_EN_Pin GPIO_PIN_15
#define PA15_WIFI_EN_GPIO_Port GPIOA
#define USART3_TX_WIFI_Pin GPIO_PIN_10
#define USART3_TX_WIFI_GPIO_Port GPIOC
#define USART3_RX_WIFI_Pin GPIO_PIN_11
#define USART3_RX_WIFI_GPIO_Port GPIOC
#define PB5_EXTI_Pin GPIO_PIN_5
#define PB5_EXTI_GPIO_Port GPIOB
#define PB5_EXTI_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
