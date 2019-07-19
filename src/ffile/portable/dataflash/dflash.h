/*
 * 		dflash.h
 * 			Lower level dataflash functions
 */

#ifndef __DFLASH_H__
#define __DFLASH_H__


#include "mytypes.h"
#include "dftype.h"

/*
 * 	Opcodes internals
 */

enum
{
	BLOCK_ERASE,
	MM_PAGE_READ,
	MM2B1_XFER,
	B1_READ,
	MM2B2_XFER,	
	B2_READ,		
	STATUS_READ,	
	APRW_B1,		
	APRW_B2,		
	MMCMP_B1,
	MMCMP_B2,	
	CONT_AREAD,	
	PAGE_ERASE,	
	MMPRG_B1,	
	MMPRG_B1_BIN_ERASE,
	B1_WRITE,
	MMPRG_B2,		  
	MMPRG_B2_BIN_ERASE,
	B2_WRITE,
	MMPRG_B1_WOUT_BIN_ERASE,
	MMPRG_B2_WOUT_BIN_ERASE,

	DF_NUM_OPCODES,
};

/*
 * dataflash error codes
 */
enum
{
	DFOK,
	FLASH_OK = DFOK,
	BAD_OPCODE, BAD_ADDRESS,
	BAD_POINTER,
	FLASH_UNKNOWN,
	FLASH_MUST_POWERCYCLE,

	NUM_FATAL_ERRORS
};


/*	For page or byte address invalid	*/

#define DFNULL	-1

/*	Mask for page comparision status */

#define FLASH_COMPARE			0x40

/*
 * 		Public functions
 */

/*
 * 	dflash_init:
 * 		Inits whatever needed and
 * 		calls lower level to initialize
 */

void dflash_init( void );

/*
 * 	dflash_send_command:
 *		This function is a generic function that setups
 *		the correct command to the Atmel flash part.
 *		First, the device is selected,
 *		Then the command is issued
 *		If, after the command, something must be read or written,
 *		CS is left active, otherwise the device is deselected
 */

void dflash_send_command( MUInt command, int page_address, int byte_address );

/*
 * 	dflash_read_status:
 * 		Flash command STATUS_READ is issued
 * 		and status is returned
 */

MUInt dflash_read_status( void );

/*
 * 	dflash_wait_ready:
 * 		Waits till flash is ready
 */

void dflash_wait_ready( void );

/*
 * 	dflash_read:
 * 		Reads 'qty' bytes from SPI into 'prx'
 * 		Then deselects device
 */

void dflash_read( uchar *prx, uint qty );

/*
 * 	dflash_write:
 * 		Writes 'qty' bytes into SPI from 'ptx'
 * 		Then deselects device
 */

void dflash_write( uchar *ptx, uint qty );


/*
 * df_refresh:
 */

int dflash_refresh( uint page );

void dflash_set_binary_page_size( void );

/*
 * dflash_fatal:
 * 	Data flash fatal error hook function
 */

MUInt dflash_fatal( MUInt error_code );

#endif
