/*
 * 	dfspi.h
 * 		Dataflash SPI Hardware Abstraction Layer
 */

#ifndef __DFSPI_H__
#define __DFSPI_H__

#include "mytypes.h"

#define DFLASH_BASE				1
#define DF_BOUNCE_TIME 			(50/SLEEP_BASE)
#define DF_RELEASE_TIME 		(1000/DFLASH_BASE)

void dfspi_init(void);

void dfspi_select_channel(void);
void dfspi_deselect_channel(void);

void dfspi_send_byte(uchar b);
uchar dfspi_get_byte(void);

#endif
