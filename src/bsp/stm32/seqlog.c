/*
 *	seqlog.c
 */

#include "mytypes.h"
#include "seqlog.h"
#include "cubemx.h"

void
set_hard_leds( unsigned short minor, MUInt arga, MUInt argb )
{
    (void)argb;
  
    if((minor & LED_POWER) != 0)
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, arga);

    if((minor & LED_GSM) != 0)
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, arga);

    if((minor & LED_STORAGE) != 0)
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, arga);

    if((minor & LED_GPS) != 0)
        HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, arga);

    if((minor & LED_WORK) != 0)
        HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, arga);

    if((minor & LED_SIM) != 0)
        HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, arga);
}

