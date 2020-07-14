/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#define LED5_Pin GPIO_PIN_2
#define LED5_GPIO_Port GPIOE
#define LED6_Pin GPIO_PIN_3
#define LED6_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_4
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_5
#define LED2_GPIO_Port GPIOE
#define MODEM_PWRON_Pin GPIO_PIN_3
#define MODEM_PWRON_GPIO_Port GPIOC
#define MEM_CS_Pin GPIO_PIN_4
#define MEM_CS_GPIO_Port GPIOA
#define MEM_CLK_Pin GPIO_PIN_5
#define MEM_CLK_GPIO_Port GPIOA
#define MEM_MISO_Pin GPIO_PIN_6
#define MEM_MISO_GPIO_Port GPIOA
#define MEM_MOSI_Pin GPIO_PIN_7
#define MEM_MOSI_GPIO_Port GPIOA
#define MODEM_PWR_ENABLE_Pin GPIO_PIN_0
#define MODEM_PWR_ENABLE_GPIO_Port GPIOB
#define EXTVCC_ADC_Pin GPIO_PIN_1
#define EXTVCC_ADC_GPIO_Port GPIOB
#define USB_ENABLE_Pin GPIO_PIN_13
#define USB_ENABLE_GPIO_Port GPIOB
#define USB_DM_Pin GPIO_PIN_14
#define USB_DM_GPIO_Port GPIOB
#define USB_DP_Pin GPIO_PIN_15
#define USB_DP_GPIO_Port GPIOB
#define PULL_SELECT2_Pin GPIO_PIN_8
#define PULL_SELECT2_GPIO_Port GPIOD
#define INPUT2_Pin GPIO_PIN_9
#define INPUT2_GPIO_Port GPIOD
#define PULL_SELECT1_Pin GPIO_PIN_10
#define PULL_SELECT1_GPIO_Port GPIOD
#define SIM_SELECT_Pin GPIO_PIN_11
#define SIM_SELECT_GPIO_Port GPIOD
#define OUTPUT1_Pin GPIO_PIN_12
#define OUTPUT1_GPIO_Port GPIOD
#define OUTPUT2_Pin GPIO_PIN_13
#define OUTPUT2_GPIO_Port GPIOD
#define INPUT1_Pin GPIO_PIN_14
#define INPUT1_GPIO_Port GPIOD
#define DISPLAY_TX_Pin GPIO_PIN_6
#define DISPLAY_TX_GPIO_Port GPIOC
#define DISPLAY_RX_Pin GPIO_PIN_7
#define DISPLAY_RX_GPIO_Port GPIOC
#define EXTPOWER_Pin GPIO_PIN_8
#define EXTPOWER_GPIO_Port GPIOC
#define CAN_ENABLE_Pin GPIO_PIN_9
#define CAN_ENABLE_GPIO_Port GPIOC
#define RS485_DIR_Pin GPIO_PIN_8
#define RS485_DIR_GPIO_Port GPIOA
#define RS485_TX_Pin GPIO_PIN_9
#define RS485_TX_GPIO_Port GPIOA
#define RS485_RX_Pin GPIO_PIN_10
#define RS485_RX_GPIO_Port GPIOA
#define CAN_TX_Pin GPIO_PIN_11
#define CAN_TX_GPIO_Port GPIOA
#define CAN_RX_Pin GPIO_PIN_12
#define CAN_RX_GPIO_Port GPIOA
#define EEPROM_SCK_Pin GPIO_PIN_10
#define EEPROM_SCK_GPIO_Port GPIOC
#define PWROFF_Pin GPIO_PIN_11
#define PWROFF_GPIO_Port GPIOC
#define SERIALOUT_Pin GPIO_PIN_12
#define SERIALOUT_GPIO_Port GPIOC
#define EXTGPS_Pin GPIO_PIN_2
#define EXTGPS_GPIO_Port GPIOD
#define EEPROM_DOUT_Pin GPIO_PIN_4
#define EEPROM_DOUT_GPIO_Port GPIOB
#define EEPROM_DIN_Pin GPIO_PIN_5
#define EEPROM_DIN_GPIO_Port GPIOB
#define EEPROM_CS_Pin GPIO_PIN_6
#define EEPROM_CS_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_0
#define LED3_GPIO_Port GPIOE
#define LED4_Pin GPIO_PIN_1
#define LED4_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
