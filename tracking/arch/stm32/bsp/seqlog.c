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
  
#ifdef __PRINTF__
	printf( "set_hard_tone: %u, %d, %d\n", minor, arga, argb );:enb
#endif

    switch(minor)
    {
        case LED_POWER:
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, arga);
            break;

        case LED_GSM:
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, arga);
            break;

        case LED_STORAGE:
            HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, arga);
            break;

        case LED_GPS:
            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, arga);
            break;

        case LED_WORK:
            HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, arga);
            break;

        case LED_SIM:
            HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, arga);
            break;

        default:
            break;
    }
}

