/*
 *	dfspi.c
 *
 *	Dataflash SPI peripherical management.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 26/05/06.
 */

#include "mytypes.h"
#include "dfspi.h"
#include "spi.h"

#define	DUMMY_DATA	0xFF

static
uchar
spi_xfer(uchar b)
{
    uchar r;

    if(HAL_SPI_TransmitReceive(&hspi1, &b, &r, 1, 100) != HAL_OK)
        return r;
    else
        return 0;
}

void
dfspi_init(void)
{
    MX_SPI1_Init();
}

void
dfspi_select_channel(void)
{
}

void
dfspi_deselect_channel(void)
{

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

