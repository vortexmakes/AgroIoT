
#include <stdio.h>
#include "tplink.h"
#include "tplhal.h"
#include "ps.h"
#include "cboxcmd.h"
#include "sleep.h"
#include "cbdata.h"
#include "mytypes.h"

static const PS_PLBUFF_T reqs[] = 
{
	{ 1, { CBOX_NULL } },
	{ 1, { CBOX_READ_ALL } }
};

MUInt cbfrm_count;
static CBOX_STR *tmp_cbox;
static MUInt cbpol_running;
/********
* TODO: Resolve save_in_flash???
*/
//extern MUInt save_in_flash;

static uchar last_sections_state;


/**
 * 	Protocol scheduler's callbacks.
 *
 * 	PROSENS uses the callback mechanism to notify events 
 * 	to application code. Each of callbacks could be enabled or 
 * 	disabled from the pscfg.h file using the preprocessor directives.
 */

/**
 * 	/brief
 * 	This function is called when the protocol starts a new polling cycle.
 */
void 
ps_on_startcycle( void )
{
}

/**
 * 	/brief
 * 	This function is called when the protocol stops the polling.
 */

void 
ps_on_stop( void )
{
}

/**
 * 	/brief
 * 	This function is called when the protocol ends a polling cycle.
 */
void 
ps_on_endcycle( void )
{
	/********
	* TODO: Resolve save_in_flash???
	*/
#if 0
	set_host_ch( HOST_TO_GPS );

	/* ------ End updating cycle of GPS data ------ */
	gps_keepalive_verify();
	if( save_in_flash == 1 )
	{
		ELEM_TYPE news = SAVE_IN_FLASH;

		save_in_flash = 0;
		put_gsqueue( NEWS_QUEUE, news );
	}
	gps_save_data_in_ram();
#endif
}

/**
 * 	Station's callbacks.
 *
 * 	PROSENS uses the callback mechanism to notify events 
 * 	to application code. Each of callbacks could be enabled or 
 * 	disabled from the pscfg.h file using the preprocessor directives.
 */
void
reset_cb_data( void )
{
	memset(tmp_cbox,0,sizeof(CBOX_STR));
	tmp_cbox->a.x = CBOX_NOT_PRESENT; 
	tmp_cbox->a.y = CBOX_NOT_PRESENT; 
	tmp_cbox->a.z = CBOX_NOT_PRESENT; 

	cbfrm_count = 0;
}

#define CBP_ADD(x,y) 	((y *)q)->x += ((y *)t)->x;
#define CBP_PROM(x,y,cast)	((y *)q)->x = (((y *)q)->x > 0) ?			\
								(cast)((((y *)q)->x + ((y *)t)->x) >> 1) :	\
								((y *)t)->x

#define CBP_ASIGN(x,y)   ((y *)q)->x = ((y *)t)->x;

#define __LOG_FRAMECNT_IN_Y_AXIS__	0 // logs cbox frames cuantity in Y-AXIS for debug

void
proc_cbdata( CBOX_STR *p )
{
	void *q;
	void *t;

    /* TODO: fix responce´s payload processing
     */
	if( tmp_cbox == NULL )
		return;
#if 0
	++cbfrm_count;

	q = &tmp_cbox->h;
	t = &p->h;

	CBP_ADD( hoard, GRSENS_T );
	CBP_ADD( pqty, GRSENS_T );

	CBP_PROM( flow, GRSENS_T, ushort );

	q = &tmp_cbox->a;
	t = &p->a;

	CBP_PROM( x, ACCEL_T, short );
#if __LOG_FRAMECNT_IN_Y_AXIS__ == 1
	tmp_cbox->a.y = cbfrm_count;
#else
	CBP_PROM( y, ACCEL_T, short );
#endif
	CBP_PROM( z, ACCEL_T, short );

	tmp_cbox->m |= p->m;

	q = tmp_cbox;
	t = p;

	CBP_ASIGN( hum, CBOX_STR);
	
/********
* TODO: Resolve 
      save_in_flash???
      last_sections_state????
	if(tmp_cbox->a.x == EQTYPE_SPRAYERPRO && tmp_cbox->hum != last_sections_state)
	{
 
			save_in_flash = 1;
	}
	last_sections_state = tmp_cbox->hum;
*/
#endif
}

/**
 * 	/brief
 * 	This function is called when the station has sent data to the master.
 *
 * 	Every station could be defined its own action on this situation, 
 * 	to do that, use the station parameter within this function.
 */
void 
on_st_rcv( ST_T station, PS_PLBUFF_T *pb )
{
	switch( station )
	{
		case ADDR_NORIA:
			if( ((CBOX_STR *)(pb->payload))->cmd != CBOX_READ_ALL )
				break;

			if( pb->qty != 0x10 )
				break;
			
			proc_cbdata( (CBOX_STR *) pb->payload );
			break;
		case ADDR_CAUDALIMETRO:
		default:
			break;
	}
}

/**
 
 * 	/brief
 * 	This function is called when the master sends the request command to 
 * 	the station. Every station could be defined its own request command, 
 * 	to do that, use the station parameter within this function.
 *
 * 	Therefore, use PS_PLBUFF_T structure to store the request command 
 * 	payload. Thus, when invoking the on_st_req() callback the protocol 
 * 	scheduler will send to station the defined request command.
 *
 *	/returns
 *	The pointer to the request command.
 */

PS_PLBUFF_T *
on_st_req( ST_T station )
{
	switch( station )
	{
		case ADDR_NORIA:
			return (PS_PLBUFF_T *)&reqs[ CBOX_READ_ALL ];
		case ADDR_CAUDALIMETRO:
		default:
			return (PS_PLBUFF_T *)&reqs[ CBOX_NULL ];
	}	
}

/**
 * 	/brief
 * 	This function is called when the station is out of polling.
 *
 * 	Every station could be defined its own action on this situation, 
 * 	to do that, use the station parameter within this function.
 */

void
on_st_stop( ST_T station, uchar reason )
{
  (void)station;
  (void)reason;
	cbpol_running = 0;
}

/**
 * 	/brief
 * 	This function is called when the station is added into the polling.
 *
 * 	Every station could be defined its own action on this situation, 
 * 	to do that, use the station parameter within this function.
 */

void
on_st_run( ST_T station )
{
  (void)station;
	cbpol_running = 1;
}

/*
 * tplink Callback functions
 */

static PSM_PKT_T pskt;

void
tplink_onrcv( TPLFRM_T *p )
{
	pskt.m.m = PS_SIG_RESPONSE;
	pskt.station = p->addr;
	memcpy(pskt.buff.payload, p->pload, p->qty);
	pskt.buff.qty = (uchar)p->qty;

	ps_rcv_packet( &pskt );
}

void
tplink_ontout( void )
{
	ps_timeout();
}

void
tplink_onchkerr( void )
{
	ps_timeout();
}

/*
 * Tracking logic specific 
 */

void
get_cb_data( void )
{
/* TODO: Charger status???
	if( !cbpol_running )
	{
		switch( get_charge_status() )
		{
			case LINE_NOBATT:
			case LINE_BATTCHG:
			case LINE_BATT:
				ps_start();
				return;
			default:
				ps_restart();
				return;
		}
	}
*/

	ps_restart();
}

void
cb_init( CBOX_STR *p )
{
	tmp_cbox = p;

	ps_init();
	ps_start();
}	


