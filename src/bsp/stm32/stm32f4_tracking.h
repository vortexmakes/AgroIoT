/**
 *  \file       stm32f4_tracking.h
 *  \brief      CUBE STM32F4 BSP for tracking32 Yipies board adapted from
 *              stm32f4_discovery.h
 *  \ingroup    bsp
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.01.23  DaBa  v0.0.01  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_TRACKING_H
#define __STM32F4_TRACKING_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
   
/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32F4_TRACKING
  * @{
  */ 
      
/** @addtogroup STM32F4_TRACKING_LOW_LEVEL
  * @{
  */ 

/** @defgroup STM32F4_TRACKING_LOW_LEVEL_Exported_Types STM32F4 TRACKING LOW LEVEL_Exported_Types
  * @{
  */
typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3,
  LED5 = 4,
  LED6 = 5
} Led_TypeDef;

typedef enum 
{  
  INPUT1 = 0,
  INPUT2 = 0,
} Input_TypeDef;

typedef enum 
{  
  INPUT_MODE_GPIO = 0,
  INPUT_MODE_EXTI = 1
} InputMode_TypeDef;     
/**
  * @}
  */ 

/** @defgroup STM32F4_TRACKING_LOW_LEVEL_Exported_Constants STM32F4 TRACKING LOW LEVEL Exported Constants
  * @{
  */ 

/** 
* @brief  Define for STM32F4_TRACKING board  
*/ 
#if !defined (USE_STM32F4_TRACK)
 #define USE_STM32F4_TRACK
#endif

/** @defgroup STM32F4_TRACKING_LOW_LEVEL_LED STM32F4 TRACKING LOW LEVEL LED
  * @{
  */
#define LEDn                             6

#define LED1_PIN                         GPIO_PIN_4
#define LED1_GPIO_PORT                   GPIOE
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()

#define LED2_PIN                         GPIO_PIN_5
#define LED2_GPIO_PORT                   GPIOE
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()

#define LED3_PIN                         GPIO_PIN_0
#define LED3_GPIO_PORT                   GPIOE
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()
  
#define LED4_PIN                         GPIO_PIN_1
#define LED4_GPIO_PORT                   GPIOE
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()

#define LED5_PIN                         GPIO_PIN_2
#define LED5_GPIO_PORT                   GPIOE
#define LED5_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED5_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()

#define LED6_PIN                         GPIO_PIN_3
#define LED6_GPIO_PORT                   GPIOE
#define LED6_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED6_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__) do{if((__INDEX__) == 0) LED1_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 1) LED2_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 2) LED3_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 3) LED4_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 4) LED5_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 5) LED6_GPIO_CLK_ENABLE(); \
                                           }while(0)

#define LEDx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) LED1_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 1) LED2_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 2) LED3_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 3) LED4_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 4) LED5_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 5) LED6_GPIO_CLK_DISABLE(); \
                                            }while(0)
/**
  * @}
  */ 
  
/** @defgroup STM32F4_TRACKING_LOW_LEVEL_INPUT STM32F4 TRACKING LOW LEVEL INPUT
  * @{
  */  
#define INPUTn                          2 

/**
 * @brief Wakeup push-button
 */
#define INPUT1_PIN                GPIO_PIN_14
#define INPUT1_GPIO_PORT          GPIOD
#define INPUT1_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOD_CLK_ENABLE()
#define INPUT1_GPIO_CLK_DISABLE() __HAL_RCC_GPIOD_CLK_DISABLE()
#define INPUT1_EXTI_IRQn          EXTI15_10_IRQn

#define INPUT2_PIN                GPIO_PIN_9
#define INPUT2_GPIO_PORT          GPIOD
#define INPUT2_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOD_CLK_ENABLE()
#define INPUT2_GPIO_CLK_DISABLE() __HAL_RCC_GPIOD_CLK_DISABLE()
#define INPUT2_EXTI_IRQn          EXTI9_5_IRQn

#define INPUTx_GPIO_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) INPUT1_GPIO_CLK_ENABLE(); else \
                                               if((__INDEX__) == 1) INPUT2_GPIO_CLK_ENABLE(); \
                                               }while(0)

#define INPUTx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) INPUT1_GPIO_CLK_DISABLE(); else \
                                               if((__INDEX__) == 1) INPUT2_GPIO_CLK_DISABLE(); \
                                               }while(0)
/**
  * @}
  */ 
  
/** @defgroup STM32F4_TRACKING_LOW_LEVEL_BUS STM32F4 TRACKING LOW LEVEL BUS
  * @{
  */  

/*############################### SPI1 #######################################*/
#define TRACKING_SPIx                              SPI1
#define TRACKING_SPIx_CLK_ENABLE()                 __HAL_RCC_SPI1_CLK_ENABLE()
#define TRACKING_SPIx_GPIO_PORT                    GPIOA                      /* GPIOA */
#define TRACKING_SPIx_AF                           GPIO_AF5_SPI1
#define TRACKING_SPIx_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define TRACKING_SPIx_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOA_CLK_DISABLE()
#define TRACKING_SPIx_SCK_PIN                      GPIO_PIN_5                 /* PA.05 */
#define TRACKING_SPIx_MISO_PIN                     GPIO_PIN_6                 /* PA.06 */
#define TRACKING_SPIx_MOSI_PIN                     GPIO_PIN_7                 /* PA.07 */

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SPIx_TIMEOUT_MAX                            0x1000 /*<! The value of the maximal timeout for BUS waiting loops */


/*############################# I2C1 #########################################*/
/* I2C clock speed configuration (in Hz) */
#ifndef BSP_I2C_SPEED
 #define BSP_I2C_SPEED                            100000
#endif /* BSP_I2C_SPEED */

/* I2C peripheral configuration defines (control interface of the audio codec) */
#define TRACKING_I2Cx                            I2C1
#define TRACKING_I2Cx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define TRACKING_I2Cx_SCL_SDA_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define TRACKING_I2Cx_SCL_SDA_AF                 GPIO_AF4_I2C1
#define TRACKING_I2Cx_SCL_SDA_GPIO_PORT          GPIOB
#define TRACKING_I2Cx_SCL_PIN                    GPIO_PIN_6
#define TRACKING_I2Cx_SDA_PIN                    GPIO_PIN_9

#define TRACKING_I2Cx_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define TRACKING_I2Cx_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()

/* I2C interrupt requests */                  
#define TRACKING_I2Cx_EV_IRQn                    I2C1_EV_IRQn
#define TRACKING_I2Cx_ER_IRQn                    I2C1_ER_IRQn

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define I2Cx_TIMEOUT_MAX    0x1000 /*<! The value of the maximal timeout for BUS waiting loops */


/*############################# ACCELEROMETER ################################*/
/* Read/Write command */
#define READWRITE_CMD                     ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD                  ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                        ((uint8_t)0x00)

/* Chip Select macro definition */
#define ACCELERO_CS_LOW()       HAL_GPIO_WritePin(ACCELERO_CS_GPIO_PORT, ACCELERO_CS_PIN, GPIO_PIN_RESET)
#define ACCELERO_CS_HIGH()      HAL_GPIO_WritePin(ACCELERO_CS_GPIO_PORT, ACCELERO_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  ACCELEROMETER Interface pins
  */
#define ACCELERO_CS_PIN                        GPIO_PIN_3                 /* PE.03 */
#define ACCELERO_CS_GPIO_PORT                  GPIOE                      /* GPIOE */
#define ACCELERO_CS_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define ACCELERO_CS_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOE_CLK_DISABLE()
#define ACCELERO_INT_GPIO_PORT                 GPIOE                      /* GPIOE */
#define ACCELERO_INT_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define ACCELERO_INT_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOE_CLK_DISABLE()
#define ACCELERO_INT1_PIN                      GPIO_PIN_0                 /* PE.00 */
#define ACCELERO_INT1_EXTI_IRQn                EXTI0_IRQn 
#define ACCELERO_INT2_PIN                      GPIO_PIN_1                 /* PE.01 */
#define ACCELERO_INT2_EXTI_IRQn                EXTI1_IRQn 
/**
  * @}
  */ 


/*############################### AUDIO ######################################*/
/**
  * @brief  AUDIO I2C Interface pins
  */
#define AUDIO_I2C_ADDRESS                     0x94
  
  /* Audio Reset Pin definition */
#define AUDIO_RESET_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOD_CLK_ENABLE()
#define AUDIO_RESET_PIN                       GPIO_PIN_4
#define AUDIO_RESET_GPIO                      GPIOD
/**
  * @}
  */  

/** @defgroup STM32F4_TRACKING_LOW_LEVEL_Exported_Macros STM32F4 TRACKING LOW LEVEL Exported Macros
  * @{
  */  
/**
  * @}
  */ 

/** @defgroup STM32F4_TRACKING_LOW_LEVEL_Exported_Functions STM32F4 TRACKING LOW LEVEL Exported Functions
  * @{
  */
uint32_t BSP_GetVersion(void);
void     BSP_LED_Init(Led_TypeDef Led);
void     BSP_LED_On(Led_TypeDef Led);
void     BSP_LED_Off(Led_TypeDef Led);
void     BSP_LED_Toggle(Led_TypeDef Led);
void     BSP_PB_Init(Input_TypeDef Input, InputMode_TypeDef Mode);
uint32_t BSP_PB_GetState(Input_TypeDef Input);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_TRACKING_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
