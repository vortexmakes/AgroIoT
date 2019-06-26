/*
 * 		dflash.c
 * 			Implements 'dflash.h'
 * 			Lower level datadflash functions
 */

#include "mytypes.h"
#include "dflash.h"
#include "dfspi.h"

#define RDY				0x80
#define ERROR0_MEMORY	0x00
#define ERROR1_MEMORY	0xFF
#define MAX_STATUS_WAIT_RESP 0x1FFF // ~ 66ms

#define dataflash_fatal(e)	(void)0;

typedef struct
{
	uchar	opcode;
	uchar	mode;
	uchar	num_bytes;
	uchar	additional;
	uchar	wait_term;
	uchar	read_write;
}OPC_T;

/*
 * 	+ => page 15
 * 	- => page 7
 */

static OPC_T const opc_actions[ DF_NUM_OPCODES ] =
{
	{	0x50,	0,	4,	0,	1,	0 	},	/*	BLOCK_ERASE				+*/
	{	0x52,	1,	4,	4,	0,	1	},	/*	MM_PAGE_READ			*/
	{	0x53,	0,	4,	0,	1,	0	},	/*	MM2B1_XFER				-*/
	{	0x54,	1,	4,	1,	0,	1	},	/*	B1_READ					*/
	{	0x55,	0,	4,	0,	1,	0	},	/*	MM2B2_XFER				-*/
	{	0x56,	1,	4,	1,	0,	1	},	/*	B2_READ					*/
	{	0x57,	1,	1,	0,	0,	1	},	/*	STATUS_READ				*/
	{	0x58,	0,	4,	0,	1,	0	},	/*	APRW_B1					-*/
	{	0x59,	0,	4,	0,	1,	0	},	/*	APRW_B2					-*/
	{	0x60,	0,	4,	0,	1,	0	},	/*	MMCMP_B1				-*/
	{	0x61,	0,	4,	0,	1,	0	},	/*	MMCMP_B2				-*/
	{	0x68,	1,	4,	4,	0,	1	},	/*	CONT_AREAD				*/
	{	0x81,	0,	4,	0,	1,	0	},	/*	PAGE_ERASE				+*/
	{	0x82,	0,	4,	0,	1,	0	},	/*	MMPRG_B1				-*/
	{	0x83,	0,	4,	0,	1,	0	},	/*	MMPRG_B1_BIN_ERASE		-*/
	{	0x84,	0,	4,	0,	0,	1	},	/*	B1_WRITE				*/
	{	0x85,	0,	4,	0,	1,	0	},	/*	MMPRG_B2				-*/
	{	0x86,	0,	4,	0,	1,	0	},	/*	MMPRG_B2_BIN_ERASE		-*/
	{	0x87,	0,	4,	0,	0,	1	},	/*	B2_WRITE				*/
	{	0x88,	0,	4,	0,	1,	0	},	/*	MMPRG_B1_WOUT_BIN_ERASE	-*/
	{	0x89,	0,	4,	0,	1,	0	}	/*	MMPRG_B2_WOUT_BIN_ERASE	-*/
};

static uchar cmd_buf[4];


/*
 * 	Sizes of each block in sectors
 */

const uchar bsc[ DF_NUM_SECTORS ] =
{	
#ifdef __DF_AT45DB161D__
	/* AT45DB161 */
	1, 31, 32, 32, 32, 32, 32, 32,
	32, 32, 32, 32, 32, 32, 32, 32, 32
#endif

#ifdef __DF_AT45DB321D__
	/* AT45DB321 */
	1, 15,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16
#endif
};


/*
 * 	dflash_compose_address
 *		Sets address data according to
 *		page and byte address and type of memory
 */

static
void
dflash_compose_address( uint page_addr, uint byte_addr )
{
	ulong addr;

 	addr = ((ulong)page_addr << DF_BADDR_BITS) |
			(byte_addr & ((1<<DF_BADDR_BITS) - 1));

    cmd_buf[1] = (uchar)(addr >> 16);
    cmd_buf[2] = (uchar)(addr >>  8);
    cmd_buf[3] = (uchar)addr;
}

/*
 * 	dflash_flush_command:
 * 		Sends an array of bytes to SPI
 * 		that are in cmd_buf in qty len
 */

static
void
dflash_flush_command( MUInt len)
{
	uchar *p;

	p = cmd_buf;
	while( len-- )
		dfspi_send_byte( *p++ );
}

/*
 * 		Public functions
 */

/*
 * 	dflash_init:
 * 		Inits whatever needed and
 * 		calls lower level to initialize
 */

void
dflash_init(void)
{
	dfspi_init();
}


/*
 * 	dflash_send_command:
 *		This function is a generic function that setups
 *		the correct command to the Atmel flash part.
 *		First, the device is selected,
 *		Then the command is issued
 *		If, after the command, something must be read or written,
 *		CS is left active, otherwise the device is deselected
 */

void 
dflash_send_command(MUInt command, int page_address, int byte_address)
{
	OPC_T const *p;

	p = &opc_actions[ command ];

	dfspi_select_channel();
	if( page_address != DFNULL )
		dflash_compose_address( page_address, byte_address );
	*cmd_buf = p->opcode;
	dflash_flush_command( p->num_bytes );
	if( p->additional )
		dflash_flush_command( p->additional );
	if( !p->read_write )
		dfspi_deselect_channel();
}

/*
 * 	dflash_read_status:
 * 		Flash command STATUS_READ is issued
 * 		and status is returned
 */

MUInt
dflash_read_status(void)
{
	MUInt status;


	dfspi_select_channel();
	*cmd_buf = opc_actions[ STATUS_READ ].opcode;
	dflash_flush_command( 1 );
	status = dfspi_get_byte();
	dfspi_deselect_channel();
	return status;
}

/*
 * 	dflash_set_binary_page_size:
 * 		OTP Flash command, configure Power 2
 * 		page size.
 */

void
dflash_set_binary_page_size(void)
{
	dfspi_select_channel();
	cmd_buf[0] = 0x3D;
	cmd_buf[1] = 0x2A;
	cmd_buf[2] = 0x80;
	cmd_buf[3] = 0xA6;
	dflash_flush_command( 4 );
	dfspi_deselect_channel();
}


/*
 * 	dflash_wait_ready:
 * 		Waits till flash is ready
 */

void
dflash_wait_ready(void)
{
	ushort wait_resp_count;
	MUInt status;

	wait_resp_count = MAX_STATUS_WAIT_RESP;
	while(1)
	{
		status = dflash_read_status();

		if( --wait_resp_count == 0 )
			dataflash_fatal( BAD_FLASH_ACCESS );
		
		switch( status )
		{
			case ERROR0_MEMORY:
			case ERROR1_MEMORY:
				dataflash_fatal( BAD_FLASH_ACCESS );
				break;
			default:
				if( status & RDY )
					return;
				break;
		}
	}
}

/*
 * 	dflash_read:
 * 		Reads 'qty' bytes from SPI into 'prx'
 * 		Then deselects device
 */

void
dflash_read( uchar *prx, uint qty )
{
	while( qty-- )
		*prx++ = (uchar)dfspi_get_byte();
	dfspi_deselect_channel();
}

/*
 * 	dflash_write:
 * 		Writes 'qty' bytes into SPI from 'ptx'
 * 		Then deselects device
 */

void
dflash_write( uchar *ptx, uint qty )
{
	while( qty-- )
		dfspi_send_byte( *ptx++ );
	dfspi_deselect_channel();
}


static uint page_counters[ DF_NUM_SECTORS ];

/*
 * 	must_refresh:
 * 		This function only tests if refresh
 * 		must be done in 'page' of 'device'
 * 		If so, returns page to be refreshed,
 * 		else returns negative number.
 * 		Implemented according to first method
 * 		of referred Application Note AN-4
 */

int
must_refresh( int page )
{
	const uchar *p;
	MUInt sec_count;
	uint accu_page, next_border, ref_page;

	p = bsc;
	for( sec_count = accu_page = 0 ; sec_count < DF_NUM_SECTORS ; ++p, ++sec_count )
	{
		next_border = accu_page + *p * DF_PAGE_NUM_PER_BLOCK;
		if( page >= accu_page && page < next_border )
		{
			ref_page = accu_page + page_counters[ sec_count ]++;
			if( page_counters[ sec_count ] >= *p * DF_PAGE_NUM_PER_BLOCK )
				page_counters[ sec_count ] = 0;
			return ref_page;
		}
		accu_page = next_border;
	}
	return -1;
}


int
dflash_refresh( uint page )
{
	int ref_page;

	if( ( ref_page = must_refresh( page ) ) >= 0 && ref_page != page )
	{
		dflash_send_command( APRW_B1, page, 0 );
		dflash_wait_ready();
		return 1;
	}
	return 0;
}
