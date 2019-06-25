/*
 * 	spihal.h
 * 		SPI Hardware Abstraction Layer
 */

#ifndef __SPIHAL_H__
#define __SPIHAL_H__

#include "mytypes.h"

#include "spi.h"

#define DFLASH_BASE				1
#define DF_BOUNCE_TIME 			(50/SLEEP_BASE)
#define DF_RELEASE_TIME 		(1000/DFLASH_BASE)

#define	DUMMY_DATA	0xFF

#if 0
#define spihal_init()				PTED_PTED6 = 1

#define spi_select_channel()		PTED_PTED6 = 0
#define spi_deselect_channel()		PTED_PTED6 = 1

#define spi_send_byte(b)	spi_xfer((uchar)b)
#define spi_get_byte()		spi_xfer((uchar)DUMMY_DATA)
#else
#define spihal_init()				;

#define spi_select_channel()		;
#define spi_deselect_channel()		;

#define spi_send_byte(b)			;
#define spi_get_byte()				0
#endif

#endif
