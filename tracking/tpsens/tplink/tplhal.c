/*
 * tplhal.c
 */

#include "rkh.h"
#include "bsp.h"
#include "tplhal.h"
#include "tplfsm.h"

static int enable_xmit = 0;
static char xmit_thr;
static MUInt tpl_running;

void
tplink_rx_isr( unsigned char byte )
{
	tplfsm_rcv_isr( byte );
}

void
tplink_tx_isr( void )
{
	tplfsm_xmit_isr();

	if( enable_xmit )
	    bsp_serial_putchar( TPSENS_PORT, xmit_thr );
}

/*
 * Public Functions
 */

void
init_tpl_hal( void )
{
	bsp_serial_open(TPSENS_PORT);

	tpl_running = 1;
}

void
deinit_tpl_hal( void )
{
	tpl_running = 0;

	bsp_serial_close(TPSENS_PORT);
}

void
tpl_choke_xmit( uchar data )
{
	enable_xmit = 1;
	bsp_serial_putchar( TPSENS_PORT, data );
}

void
tpl_xmit( uchar data )
{
	xmit_thr = data;
}

void
tpl_stop_xmit( void )
{
	enable_xmit = 0;
}

void
call_to_feox( void ) 
{
	tplink_tx_isr();
}

void
tpl_eoftx( void )
{
}	

