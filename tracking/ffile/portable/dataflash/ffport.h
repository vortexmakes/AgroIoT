/**
 *  \file       ffport.h
 *  \brief      FFile dataflash port declaration.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.06.25  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

#ifndef __FFPORT_H__
#define __FFPORT_H__

#include "dflash.h"

#define FFILE_WATCHDOG()		;//kick_watchdog()


/*
 * 	Size of sector flash in bytes.
 */

#define PHYS_BLOCK_SIZE			DF_PAGE_SIZE//512
#define NUM_PHYS_BLOCKS			DF_PAGE_COUNT//8192

#define FLASH_BASE_ADDRESS		(ulong)(0x0000)
#define FLASH_BASE_PAGE			0

#define FLASH_INV_SECT			(NUM_PHYS_BLOCKS)

/*
 * flash_init:
 *	Data Flash initialization.
 *	It must be called before accessing.
 */

void flash_init( void );

void flash_close( void );

/*
 * flash_verify_device:
 *	returns the status of flash device:
 *	It may be:
 *		0 - OK.
 *		BAD_FLASH_ACCESS - Memory unreachable.
 *		FLASH_MUST_POWERCYCLE - Memory needs a power down/up cycle to submit
 *								page resizing. It may hapens in production.
 */

MInt flash_verify_device( void );


/*
 * 	flash_write_data:
 *
 *	Automatically erase a specified memory page and then transfer data
 *	from a ram buffer into that memory page. When calling to this function 
 *	is required to specify the number of bytes to be transfer into that 
 *	memory page.
 *	
 * 	Arguments:
 *
 * 	'pto':		points to a location in the page to be erased.
 * 	'pfrom':	pointer to the ram buffer that is to be written 
 * 				in memory.
 * 	'qty':		number of bytes to be programmed.
 */

void flash_write_data( unsigned long pto, const void *pfrom, int qty );


/*
 * 	flash_write_page:
 *
 *	Automatically erase a specified memory page and then transfer data
 *	from a ram buffer into that memory page. The buffer must be 
 *	PHYS_BLOCK_SIZE bytes long, the same size as a flash page.
 *	
 * 	Arguments:
 *
 * 	'pto':		points to page address to be erased and programmed.
 * 	'pfrom':	pointer to the ram buffer that is to be written 
 * 				in memory.
 */

#define flash_write_page( pto, pfrom )	\
			flash_write_data( pto, pfrom, PHYS_BLOCK_SIZE )


/*
 * 	flash_cmp_data:
 *
 *	Performs data comparision between local data and dataflash memory data.
 *	
 *	At the end, compares the data in memory page to the data in ram 
 *	buffer.
 *
 * 	Arguments:
 *
 * 	'pto':		points to page address to compare.
 * 	'pfrom':	pointer to the ram buffer to compare.
 * 	'qty':		number of bytes to be compared.
 *
 * 	Returns:
 * 			0 - if datas are equal.
 * 			1 - if at least one bit not match.
 */

MUInt flash_cmp_data( void *pto, const void *pfrom, int qty );


/*
 * 	flash_read_data:
 *
 * 	Read data directly from any one of the flash pages. The data read
 * 	are placed into a ram buffer. When calling to this function 
 *	is required to specify the number of bytes to be read. 
 *
 * 	Arguments:
 *
 * 	'pto':		pointer to the buffer into which the read data
 * 				will be copied.
 * 	'pfrom':	points to a location in the page to be read.
 * 	'qty':		number of bytes to be read.
 */

void flash_read_data( void *pto, unsigned long pfrom, int qty );


/*
 * 	flash_read_page:
 *
 * 	Read data directly from any one of the flash pages. The data read
 * 	are placed into a ram buffer. This buffer must be PHYS_BLOCK_SIZE 
 * 	bytes long, the same size as a flash page.
 *
 * 	Arguments:
 *
 * 	'pto':		pointer to the buffer into which the read page
 * 				will be copied.
 * 	'pfrom':	points to page address to be read.
 */

#define flash_read_page( pto, pfrom )	\
			flash_read_data( pto, pfrom, PHYS_BLOCK_SIZE )


#endif
