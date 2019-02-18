/*
 *	file: ffport.h - Visual Studio 2008 port
 *	Last updated for version: 1.0.00
 *	Date of the last update:  Nov 20, 2012
 *
 *  Copyright (C) 2010 Leandro Francucci. All rights reserved.
 *
 *  RKH is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RKH is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RKH, see copying.txt file.
 *
 * Contact information:
 * RKH web site:	http://
 * e-mail:			francuccilea@gmail.com
 */

#ifndef __FFPORT_H__
#define __FFPORT_H__

#include "ffile.h"

/**
 *	Trigger the watchdog timer.
 */

#define FFILE_WATCHDOG()        (void)0

/**
 *  Initializes and opens the flash driver.
 */

void flash_init(void);

/**
 *  Close the flash driver.
 */

void flash_close(void);

/**
 *	Automatically erase a specified memory page and then transfer data
 *	from a ram buffer into that memory page. The buffer must be
 *	FF_PHYS_BLOCK_SIZE bytes long, the same size as a flash page.
 *
 *	At the end, compares the data in memory page to the data in ram
 *	buffer for verifying that the flash device succesfully programmed
 *	the ram buffer contents into the memory page.
 *
 *  \param pto		page address to be erased and programmed.
 *  \param pfrom	ram buffer that is to be written in memory.
 */

void flash_write_page(unsigned long pto, const void *pfrom);

/**
 *	Automatically erase a specified memory page and then transfer data
 *	from a ram buffer into that memory page. When calling to this function
 *	is required to specify the number of bytes to be transfer into that
 *	memory page.
 *
 *	At the end, compares the data in memory page to the data in ram
 *	buffer for verifying that the flash device succesfully programmed
 *	the ram buffer contents into the memory page.
 *
 *  \param pto		location in the page to be erased.
 *  \param pfrom	ram buffer that is to be written in memory.
 *  \param qty		number of bytes to be programmed.
 */

void flash_write_data(unsigned long pto, const void *pfrom, int qty);

/**
 *  Read data directly from any one of the flash pages. The data read
 *  are placed into a ram buffer. This buffer must be FF_PHYS_BLOCK_SIZE
 *  bytes long, the same size as a flash page.
 *
 *  \param pto		pointer to the buffer into which the read page
 *                  will be copied.
 *  \param pfrom	page address to be read.
 */

void flash_read_page(void *pto, unsigned long pfrom);

/**
 *  Read data directly from any one of the flash pages. The data read
 *  are placed into a ram buffer. When calling to this function
 *	is required to specify the number of bytes to be read.
 *
 *  \param pto		pointer to the buffer into which the read data
 *                  will be copied.
 *  \param pfrom	location in the page to be read.
 *  \param qty		number of bytes to be read.
 */

void flash_read_data(void *pto, unsigned long pfrom, int qty);

#endif
/* ------------------------------ End of file ------------------------------ */
