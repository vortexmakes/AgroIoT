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
#include "rkhfwk_pubsub.h"
#include "signals.h"
#include "topics.h"
#include "tplink.h"
#include "tplhal.h"
#include "ps.h"
#include "cboxcmd.h"
#include "cbdata.h"
#include "mytypes.h"

/* ----------------------------- Local macros ------------------------------ */
#define assembleShort(p,s)  \
        { \
            s = (*p) << 8 | *((p)+1); \
            p += 2; \
        }

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
ruint tpSens;

/* ---------------------------- Local variables ---------------------------- */
static const PS_PLBUFF_T reqs[] = 
{
	{ 1, { CBOX_NULL } },
	{ 1, { CBOX_READ_ALL } }
};

static PSM_PKT_T pskt;
static RKH_STATIC_EVENT(endOfCycle, evEndOfCycle);
static CBOX_STR cbox;

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
    tpSensor_publish(&endOfCycle, &tpSens);
}

CBOX_STR *
get_cbdata(void)
{
    return &cbox;
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

/* ------------------------------ End of file ------------------------------ */
