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
static uchar spi_xfer(uchar b);

/* ---------------------------- Local functions ---------------------------- */
static
uchar
spi_xfer(uchar b)
{
    uchar r;
    HAL_StatusTypeDef res;

    res = HAL_SPI_TransmitReceive(&hspi1, &b, &r, 1, 100);

    return (res == HAL_OK) ? r : 0;
}

/* ---------------------------- Global functions --------------------------- */
void
dfspi_init(void)
{
    dfspi_cs(1);

    MX_SPI1_Init();

    /* 
     * STM32 not sets SCK signal to high after init, although CPOL is set to 1,
     * then a dummy not CS asserted transfer is needed to force SPI_CLK
     * go to high before first assertion of CS.
     */
    spi_xfer(DUMMY_DATA);
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
dfspi_send_byte(uchar b)
{
    spi_xfer(b);
}

uchar
dfspi_get_byte(void)
{
    return spi_xfer(DUMMY_DATA);
}

/* ------------------------------ End of file ------------------------------ */
