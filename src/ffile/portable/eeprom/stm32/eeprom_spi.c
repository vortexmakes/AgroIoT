/**
 *  \file       eeprom_spi.c
 *  \brief      EEPROM SPI implementation for STM32.
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
#include "eeprom_spi.h"
#include "spi.h"

/* ----------------------------- Local macros ------------------------------ */
#define eeprom_cs(b)      HAL_GPIO_WritePin(EEPROM_CS_GPIO_Port, \
                                                        EEPROM_CS_Pin, b)

/* ------------------------------- Constants ------------------------------- */
#define	DUMMY_DATA	0xFF

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
eeprom_spi_init(void)
{
    uchar dummy = DUMMY_DATA;

    eeprom_cs(1);


    MX_SPI3_Init();

    /* 
     * STM32 not sets SCK signal to high after init, although CPOL is set to 1,
     * then a dummy not CS asserted transfer is needed to force SPI_CLK
     * go to high before first assertion of CS.
     */
    HAL_SPI_Transmit(&hspi3, &dummy, 1, 100);
}

void
eeprom_spi_select(void)
{
    eeprom_cs(0);
}

void
eeprom_spi_deselect(void)
{
    eeprom_cs(1);
}

void
eeprom_spi_write_byte(uchar b)
{
    HAL_SPI_Transmit(&hspi3, &b, 1, 100);
}

void
eeprom_spi_read_byte(uchar *p)
{
    HAL_SPI_Receive(&hspi3, p, 1, 100);
}

void
eeprom_spi_write(uchar *p, uint qty)
{
    HAL_SPI_Transmit_DMA(&hspi3, p, qty);

    while(HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY);
}

void
eeprom_spi_read(uchar *p, uint qty)
{
    HAL_SPI_Receive_DMA(&hspi3, p, qty);

    while(HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY);
}

/* ------------------------------ End of file ------------------------------ */
