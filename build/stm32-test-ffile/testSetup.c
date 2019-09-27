/**
 *  \file       testSetup.c
 *  \brief      .
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.17  DaBa  v1.0.00  Initial version
  */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "cubemx.h"
#include "stdio.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/*
 * Unity printout on UART5 at CubeMX configuration (115200 8N1)
 */
int
__io_putchar(int ch)
{
	while(HAL_UART_GetState(&huart5) != HAL_UART_STATE_READY);

	HAL_UART_Transmit_DMA(&huart5, (uint8_t *)(&ch), 1);

	return ch;
}

/*
 * Pre main() call Unity hook for hardware initialization.
 */
void
suiteSetUp(void)
{
    HAL_Init();

    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_UART5_Init();
    MX_CAN1_Init();
    MX_ADC1_Init();
    MX_SPI3_Init();
}

/*
 * Post main() call Unity.
 */
int
suiteTearDown(int num_failures)
{
	while(1)
		;

	return 0;
}

/* ------------------------------ End of file ------------------------------ */
