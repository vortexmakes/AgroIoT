/*
 * tplhal.c
 */

#include "rkh.h"
#include "bsp.h"
#include "tplhal.h"
#include "tplfsm.h"
#include "wserial.h"
#include <stdio.h>

int enable_xmit = 0;
int enable_rcv = 0;
static char xmit_thr;
static MUInt tpl_running;

void
tplink_rx_isr( unsigned char byte )
{
	if( enable_rcv )
		tplfsm_rcv_isr( byte );
}

void
tplink_tx_isr( void )
{
	tplfsm_xmit_isr();

	if( enable_xmit )
		tx_data( 0, xmit_thr );
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
	tx_data( 0, data );
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
#if 0
	enter_critical();
	SCIC2_TCIE = 0;

	TPL_485DIR = 0;

	SCIC2_RIE = 1;
	SCIC2_RE = 1;
	exit_critical();
#endif
}	

