/**
 *  \file       dfspi.c
 *  \brief      Dataflash SPI peripheral for STM32.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.06.27  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "mytypes.h"
#include "dfspi.h"
#include "spi.h"

/* ----------------------------- Local macros ------------------------------ */
#define dfspi_cs(b)      HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, b)

/* ------------------------------- Constants ------------------------------- */
#define	DUMMY_DATA	0xFF

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
dfspi_init(void)
{
    uchar dummy = DUMMY_DATA;

    dfspi_cs(1);

    MX_SPI1_Init();

    /* 
     * STM32 not sets SCK signal to high after init, although CPOL is set to 1,
     * then a dummy not CS asserted transfer is needed to force SPI_CLK
     * go to high before first assertion of CS.
     */
    HAL_SPI_Transmit(&hspi1, &dummy, 1, 100);
}

void
dfspi_select_channel(void)
{
    dfspi_cs(0);
}

void
dfspi_deselect_channel(void)
{
    dfspi_cs(1);
}

void
dfspi_write_byte(uchar b)
{
    HAL_SPI_Transmit(&hspi1, &b, 1, 100);
}

void
dfspi_read_byte(uchar *p)
{
    HAL_SPI_Receive(&hspi1, p, 1, 100);
}

void
dfspi_write(uchar *p, uint qty)
{
    HAL_SPI_Transmit(&hspi1, p, qty, 100);
}

void
dfspi_read(uchar *p, uint qty)
{
    HAL_SPI_Receive(&hspi1, p, qty, 100);
}

/* ------------------------------ End of file ------------------------------ */
