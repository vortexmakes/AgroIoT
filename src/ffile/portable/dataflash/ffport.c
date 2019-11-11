/**
 *  \file       ffport.c
 *  \brief      FFile dataflash port implementation.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.06.25  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina db@vortexmakes.com
 */

#include "mytypes.h"
#include "dflash.h"
#include "ffport.h"

#define FLASH_SIZE_PHY_PAGE		PHYS_BLOCK_SIZE


/*
 * Dataflash Built in erase option enable
 */

#define BERASE_ENABLE		1


/*
 * 	Select working buffer 1 or 2
 */

#define MM2BUF_XFER 		MM2B1_XFER
#define MMCMP_BUF			MMCMP_B1
#define BUF_WRITE			B1_WRITE 
#define MMPRG_BUF_BERASE	MMPRG_B1_BIN_ERASE 
#define MMPRG_BUF_WO_BERASE	MMPRG_B1_WOUT_BIN_ERASE


static ushort df_page; 
static ushort df_address;
static MUInt df_err = FLASH_UNKNOWN;


#define calc_dflash_address(a)	\
		{						\
			df_page = (ushort)(((ulong)(a)-FLASH_BASE_ADDRESS)/PHYS_BLOCK_SIZE);\
			df_address = (ushort)(((ulong)(a)-FLASH_BASE_ADDRESS)%PHYS_BLOCK_SIZE);		\
		}

#define calc_qty_in_page()		(ushort)(PHYS_BLOCK_SIZE-df_address)

void
flash_init( void )
{
	uchar status, density;

	dflash_init();

	df_err = FLASH_UNKNOWN;

	dflash_send_command( STATUS_READ, DFNULL, DFNULL );
	dflash_read( &status, 1 );

	density = (uchar)(( status >> DENSITY_POS ) & DENSITY_MASK);

	if( density != DF_DEVICE_ID )
	{
		df_err = FLASH_UNKNOWN;
		return;
	}

	if( !(status & POWER2_PSIZE_MASK) )
	{
		dflash_set_binary_page_size();
		df_err = FLASH_MUST_POWERCYCLE;
		return;
	}

	df_err = FLASH_OK;
	return;
}


void 
flash_close( void )
{
}


MInt
flash_verify_device( void )
{
	return df_err;
}

void
flash_erase_page( void *pto )
{
	if( df_err != DFOK )
		return;

	calc_dflash_address( pto );
	dflash_send_command( PAGE_ERASE, FLASH_BASE_PAGE + df_page, 0 );
	dflash_wait_ready();
}


void
flash_write_data( unsigned long pto, const void *pfrom, int qty )
{
	uint page;

	if( df_err != DFOK )
		return;

	calc_dflash_address( pto );

	dflash_send_command( BUF_WRITE, 0, df_address );
	dflash_write( (uchar *)pfrom, (ushort)qty );
	page = FLASH_BASE_PAGE + df_page;

#if BERASE_ENABLE == 1	
	dflash_send_command( MMPRG_BUF_BERASE, page, 0 );
#else
	dflash_send_command( MMPRG_BUF_WO_BERASE, page, 0 );
#endif

	dflash_wait_ready();

	dflash_refresh( page );
}


MUInt
flash_cmp_data( void *pto, const void *pfrom, int qty )
{
	if( df_err != DFOK )
		return 0;

	calc_dflash_address( pto );

	dflash_send_command( MM2BUF_XFER, FLASH_BASE_PAGE + df_page, 0 );
	dflash_wait_ready();

	dflash_send_command( BUF_WRITE, 0, df_address );
	dflash_write( (uchar *)pfrom, (ushort)qty );

	dflash_send_command( MMCMP_BUF, FLASH_BASE_PAGE + df_page, 0 );
	dflash_wait_ready();

	return (dflash_read_status() & FLASH_COMPARE);
}


void
flash_read_data( void *pto, unsigned long pfrom, int qty )
{
	short qtp;
	ushort rq;
	ushort addr;

	if( df_err != DFOK )
		return;

	calc_dflash_address(pfrom);
	qtp = calc_qty_in_page();

	if( (qtp - qty) >= 0 )
	{
		dflash_send_command( MM_PAGE_READ, FLASH_BASE_PAGE + df_page, df_address );
		dflash_read( pto, (ushort)qty );
		return;
	}


	for( addr = df_address, rq = qtp; qty; )
	{
		dflash_send_command( MM_PAGE_READ, FLASH_BASE_PAGE + df_page, addr );
		dflash_read( pto, rq );

		qty -= rq;
		pto = ((uchar *)pto) + rq;
		(pfrom) += rq;

		rq = (ushort)(( qty >= PHYS_BLOCK_SIZE ) ? PHYS_BLOCK_SIZE : qty);
		++df_page;
		addr = 0;
	}
}
