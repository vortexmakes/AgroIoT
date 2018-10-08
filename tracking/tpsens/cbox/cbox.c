/**
 *  \file       cbox.c
 * 	\bried      Module to handle cbox responces-
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.17  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdio.h>
#include "tplink.h"
#include "tplhal.h"
#include "ps.h"
#include "cboxcmd.h"
#include "sleep.h"
#include "cbdata.h"
#include "mytypes.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/********
* TODO: Resolve save_in_flash???
*/
//extern MUInt save_in_flash;

/* ---------------------------- Local variables ---------------------------- */
static const PS_PLBUFF_T reqs[] = 
{
	{ 1, { CBOX_NULL } },
	{ 1, { CBOX_READ_ALL } }
};

MUInt cbfrm_count;
static CBOX_STR *tmp_cbox;
static MUInt cbpol_running;
static uchar last_sections_state;
static PSM_PKT_T pskt;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */

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

#define assembleShort(p,s)  \
        { \
            s = (*p) << 8 | *((p)+1); \
            p += 2; \
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

	if( tmp_cbox == NULL )
		return;
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
    CBOX_STR cbox;
    uchar *p;

	switch( station )
	{
		case ADDR_NORIA:
            p = (uchar *)pb->payload;
            cbox.cmd = *p++;
            cbox.m = *p++;

            assembleShort(p, cbox.h.hoard);
            assembleShort(p, cbox.h.pqty);
            assembleShort(p, cbox.h.flow);

            assembleShort(p, cbox.a.x);
            assembleShort(p, cbox.a.y);
            assembleShort(p, cbox.a.z);

            cbox.a.m = *p++;

            cbox.hum = *p++;

			proc_cbdata((CBOX_STR *)&cbox);
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

/* ------------------------------ End of file ------------------------------ */
