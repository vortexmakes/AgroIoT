/*
 *	file: ps.c
 *	Last updated for version: 1.0.00
 *	Date of the last update:  October 20, 2011
 *
 * 	Copyright (C) 2010 Leandro Francucci. All rights reserved.
 *
 * 	RKH is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation, either version 3 of the License, or
 * 	(at your option) any later version.
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
 * e-mail:			francuccilea@gmail.com
 */

/**
 * 	\file ps.c
 *	\brief
 *
 * 	Protocol scheduler module.
 */


#include <stdlib.h>
#include <stdarg.h>
#include "ps.h"
#include "psport.h"


#define forever						for(;;)
#define ps_is_active()				(prot.state == PS_RUNNING)
#define ps_is_station_active( x )	((x)->state != ST_UNAVAILABLE && 	\
									(x)->cstatus != ST_DIS	)
#define ps_get_curr_station()		((ST_T)prot.station)

/**
 * 	Callback configurations.
 */

#if PS_EN_ON_STARTCYCLE == 1
#define cb_ps_on_startcycle()		ps_on_startcycle()
#else
#define cb_ps_on_startcycle()
#endif

#if PS_EN_ON_ENDCYCLE == 1
#define cb_ps_on_endcycle()			ps_on_endcycle()
#else
#define cb_ps_on_endcycle()
#endif

#if PS_EN_ON_ST_TOUT == 1
#define cb_on_st_tout( s )			on_st_tout( s )
#else
#define cb_on_st_tout( s )
#endif

#if PS_EN_ON_ST_RUN == 1
#define cb_on_st_run( s )			on_st_run( s )
#else
#define cb_on_st_run( s )
#endif

#if PS_EN_ON_ST_STOP == 1
#define cb_on_st_stop( s, r )		on_st_stop( s, r )
#else
#define cb_on_st_stop( s, r )
#endif


/**
 * 	\brief
 * 	Defines station states.
 */

enum
{
	ST_UNAVAILABLE,
	ST_READY,
	ST_RDY_WAIT_RESPONSE,
	ST_RETRANSMIT,
	ST_REXMIT_WAIT_RESPONSE,
	
	ST_NUM_STATES
};


/**
 * 	\brief
 * 	Protocol scheduler states
 */

enum
{
	PS_STOPED,
	PS_RUNNING,

	PS_NUM_STATES
};


/**
 * 	\brief
 * 	Codes retrieved from ps_running() function.
 */

enum
{
	PS_RUNNING_EXIT, PS_RUNNING_CONTINUE
};


/**
 * 	\brief
 * 	Station status.
 */

enum
{
	ST_DIS, ST_ENA
};


/**
 * 	\brief
 *	Maintains the needed information of a station.
 */

typedef struct
{
	/**
	 * 	\brief
	 * 	Status.
	 *
	 * 	It can have the following values:
	 *
	 * 	- \b ST_DIS:	disabled
	 * 	- \b ST_ENA:	enabled
	 */

	unsigned cstatus		:1;			/** current status */
	unsigned ostatus		:1;			/** old status */		

	/**
	 * 	\brief
	 * 	State.
	 *
	 * 	A station could be transit for any of following states:
	 *
	 * 	- \b ST_UNAVAILABLE
	 * 	- \b ST_READY
	 * 	- \b ST_RDY_WAIT_RESPONSE
	 * 	- \b ST_RETRANSMIT
	 * 	- \b ST_REXMIT_WAIT_RESPONSE
	 */

	unsigned state			:3;

	/**
	 * 	\brief
	 * 	Number of retries.
	 */

	unsigned retry			:4;

	/**
	 * 	\brief
	 * 	Indicates if last sent packet was a request.
	 */

	unsigned was_request	:1;

	/**
	 * 	\brief
	 * 	Queue descriptor.
	 */

	PSQD_T qd;
} STATION_T;


/**
 * 	\brief
 *	Maintains the protocol scheduler's information and properties.
 */

typedef struct
{

	/**
	 * 	\brief
	 * 	It's set to zero when any station in the current polling cycle 
	 * 	was reported to previous requests.
	 */

	unsigned no_station	:1;

	/**
	 * 	\brief
	 * 	State.
	 *
	 * 	A protocol scheduler could be transit for any of 
	 * 	following states:
	 *
	 * 	- \b PS_STOPED
	 * 	- \b PS_RUNNING
	 */

	unsigned state		:2;

	/**
	 * 	\brief
	 * 	Current station number.
	 */

	unsigned station	:4;

	/**
	 * 	\brief
	 * 	Points to current station.
	 */
	
	STATION_T *pcs;

	/**
	 * 	\brief
	 * 	Payload buffer.
	 */

	PS_PLBUFF_T plbuff;
} PS_T;


/**
 * 	Maintains the protocol scheduler properties.
 */

static PS_T prot;


/**
 * 	Auxiliar variables.
 */

static PSM_T msg, *pmsg = &msg;


/**
 * 	Used to indicate that is the turn of station.
 */

static const PSM_T evtsn = { PS_SIG_SEND_NEXT };


/**
 * 	Maintains the station properties.
 */

static STATION_T stations[ PS_NUM_STATIONS ];


static
STATION_T *
ps_verify_station( ST_T station )
{
	return &stations[ station ];
}


static
PSM_T *
ps_process_event( PSM_T *pm )
{
	STATION_T *p;

	p = prot.pcs;
	if( p->ostatus != p->cstatus )
	{      
		p->ostatus = p->cstatus;
		pmsg->m = p->cstatus ? PS_SIG_ENABLE : PS_SIG_DISABLE;
		return pmsg;
	}
	return pm;
}


static
uchar
ps_is_xmit_request( void )
{
	STATION_T *p;

	p = prot.pcs;

	if( p->state == ST_READY ) 
		return PSQ_IS_EMPTY( p->qd );
	else if( p->state == ST_RETRANSMIT ) 
		return (uchar)(p->was_request);
	else
		return 1;
}


static
PS_PLBUFF_T *
ps_get_payload( void )
{
	if( ps_is_xmit_request() )
	{
		prot.pcs->was_request = 1;
		return on_st_req( prot.station );
	}
	prot.pcs->was_request = 0;
	PSQ_READ( prot.pcs->qd, &prot.plbuff );
	return &prot.plbuff;
}


static
uchar
ps_dispatch_to_station( ST_T station, PSM_T *pm )
{
	STATION_T *p;
	uchar r;

	p = prot.pcs;
	r = RET_OK;

	if( p->state == ST_UNAVAILABLE )
	{
		if( pm->m == PS_SIG_ENABLE )
		{
			PSQ_DEPLETE( p->qd );
			dll_send_frame( ps_get_curr_station(), ps_get_payload());
			cb_on_st_run( station );
			r = RET_WAITRESP;
			p->state = ST_RDY_WAIT_RESPONSE;
		}
	}
	else
	{
		if( pm->m == PS_SIG_DISABLE )
		{
			cb_on_st_stop( station, PS_SIG_DISABLE );
			r = RET_UNAVAILABLE;
			p->state = ST_UNAVAILABLE;
		}
		else
		{
			switch( p->state )
			{
				case ST_READY:
					if( pm->m == PS_SIG_SEND_NEXT )
					{
						dll_send_frame( ps_get_curr_station(), ps_get_payload() );
						r = RET_WAITRESP;
						p->state = ST_RDY_WAIT_RESPONSE;
					}
					break;
				case ST_RDY_WAIT_RESPONSE:
					if( pm->m == PS_SIG_RESPONSE )
					{
						if( !p->was_request )
							PSQ_REMOVE( p->qd, &prot.plbuff );

						on_st_rcv( station, &((( PSM_PKT_T* )pm)->buff) );
						p->state = ST_READY;
					}
					else if( pm->m == PS_SIG_TOUT )
					{
						p->retry = 1;
						cb_on_st_tout( station );
						p->state = ST_RETRANSMIT;
					}
					break;
				case ST_RETRANSMIT:
					if( pm->m == PS_SIG_SEND_NEXT )
					{
						dll_send_frame( ps_get_curr_station(), ps_get_payload() );
						r = RET_WAITRESP;
						p->state = ST_REXMIT_WAIT_RESPONSE;
					}
					break;
				case ST_REXMIT_WAIT_RESPONSE:
					if( pm->m == PS_SIG_RESPONSE )
					{
						if( !p->was_request )
							PSQ_REMOVE( p->qd, &prot.plbuff );

						on_st_rcv( station, &((( PSM_PKT_T* )pm)->buff) );
						p->retry = 0;
						r = RET_OK;
						p->state = ST_READY;
					}
					else if( pm->m == PS_SIG_TOUT )
					{
						if( ++p->retry < PS_NUM_RETRIES )
						{
							cb_on_st_tout( station );
							r = RET_OK;
							p->state = ST_RETRANSMIT;
						}
						else
						{
							p->cstatus = ST_DIS;
							p->ostatus = ST_DIS; 
							cb_on_st_stop( station, PS_SIG_TOUT );
							r = RET_UNAVAILABLE;
							p->state = ST_UNAVAILABLE;
						}
					}
					break;
			}
		}
	}
	return r;
}


static
uchar
ps_running( PSM_T *pm )
{
	PSM_T *prmsg;
	uchar r;

	prmsg = pm;

	forever
	{
		r = ps_dispatch_to_station( prot.station, prmsg );

		if( r == RET_WAITRESP )
			break;

		if( r == RET_OK )
			prot.no_station = 0;
		
		if( ++prot.station < PS_NUM_STATIONS )
		{
			prot.pcs = ps_verify_station( prot.station );
			prmsg = ( PSM_T* )&evtsn;
			prmsg = ps_process_event( prmsg );
		}
		else
		{
			prot.station = 0;
			cb_ps_on_endcycle();
#if PS_EN_ON_STOP == 1
			if( prot.no_station == 1 )
				ps_on_stop();
#endif
			return PS_RUNNING_EXIT;
		}
	}
	return PS_RUNNING_CONTINUE;
}


/**
 * 	\brief
 * 	Sends messages to protocol scheduler to be accordingly dispatched.
 *
 * 	This function is indirectly called from:
 *
 *  1.- timer interrupt,
 *  2.- serial receive interrupt, and
 *  3.- application space (by means of ps_start()).
 *
 * 	\param station	station address.
 *  \param pm		pointer to arrived message.
 */

static
void 
ps_dispatch_to_scheduler( PSM_T *pm )
{
	PSM_T *q;

	switch( prot.state )
	{
		case PS_STOPED:
			if( pm->m == PS_SIG_RUN )   /* only process the PS_SIG_RUN event */
			{
				prot.station = 0;		   /* prepares to invoke ps_running()*/
				prot.no_station = 1;							  /* funcion.*/
				prot.pcs = stations;
				pmsg->m = PS_SIG_SEND_NEXT;

				q = ps_process_event( pmsg );
				cb_ps_on_startcycle();

							    /* It's the choice point from state diagram. */
				if( ps_running( q ) == PS_RUNNING_CONTINUE )
					prot.state = PS_RUNNING; 	  /* at least one station is */
												    /* available on network. */
			}
			break;
		case PS_RUNNING:
			q = ps_process_event( pm );
								/* It's the choice point from state diagram. */
			if( ps_running( q ) == PS_RUNNING_EXIT )
				prot.state = PS_STOPED;		  /* all station are unavailable */
			break;
		default:
			break;
	}
}


static
void
ps_endis_all( uchar control )
{
	STATION_T *p;
	uchar i;

	for( i = 0, p = stations; i < PS_NUM_STATIONS; ++i, ++p )
		p->cstatus = control;
}


#if PS_PRINT_FORMAT == 1
static char pfs[ PS_PRINT_FORMAT_SIZE ];

void
print_format( uchar out, char *fmt, ... )
{
	va_list args;

	va_start( args, fmt );
	vsprintf( pfs, fmt, args );
	psput_string( out, pfs );
	va_end( args );
}
#endif


void
ps_init( void )
{
	dll_init();
}


void
ps_start( void )
{
	PSM_T smsg;

	ps_enter_critical();

	if( ps_is_active() )
	{
		ps_exit_critical();
		return;
	}

	ps_exit_critical();
	ps_endis_all( ST_ENA );
	smsg.m = PS_SIG_RUN;
	ps_dispatch_to_scheduler( &smsg );
}


#if PS_EN_RESTART == 1
void
ps_restart( void )
{
	PSM_T smsg;

	ps_enter_critical();

	if( ps_is_active() )
	{
		ps_exit_critical();
		return;
	}

	ps_exit_critical();
	smsg.m = PS_SIG_RUN;
	ps_dispatch_to_scheduler( &smsg );
}
#endif


#if PS_EN_STOP == 1
void 
ps_stop( void )
{
	ps_enter_critical();

	if( ps_is_active() )
		ps_endis_all( ST_DIS );

	ps_exit_critical();
}
#endif


#if PS_EN_GET_INFO == 1

static const char *sts[] = 
{
	"ST_UNAVAILABLE",
	"ST_READY",
	"ST_RDY_WAIT_RESPONSE",
	"ST_RETRANSMIT",
	"ST_REXMIT_WAIT_RESPONSE"
};


static const char *pss[] = 
{
	"PS_STOPED",
	"PS_RUNNING"
};


static const char *sta[] = 
{
	"ST_DIS",
	"ST_ENA"
};


void
ps_get_info( void )
{
	uchar i, n;
	STATION_T *p;

	for( n = i = 0, p = stations; i < PS_NUM_STATIONS; ++i, ++p )
		if( p->state != ST_UNAVAILABLE )
			++n;

	psdprint(( 0, "--- PS INFO ---\n" ));
	psdprint(( 0, "protocol state = %s (%d)\n", 
									pss[ prot.state ], prot.state ));
	psdprint(( 0, "# active stations = %d/%d\n", n, PS_NUM_STATIONS ));
	psdprint(( 0, "current station = %d\n", prot.station ));

	psdprint(( 0, "--- STATIONS ---\n"  ));
	for( i = 0, p = stations; i < PS_NUM_STATIONS; ++i, ++p )
	{
		psdprint(( 0, "\tstate[%d] = %s (%d)\n", 
									i, sts[ p->state ], p->state ));
		psdprint(( 0, "\tnstatus[%d] = %s (%d)\n", 
									i, sta[ p->cstatus ], p->cstatus ));
		psdprint(( 0, "\tostatus[%d] = %s (%d)\n", 
									i, sta[ p->ostatus ], p->ostatus ));
		psdprint(( 0, "\tretry[%d] = %d\n", i, p->retry ));
	}
}
#endif


#if PS_EN_START_STATION == 1
void 
ps_start_station( ST_T station )
{
	STATION_T *p;

	p = ps_verify_station( station );
	ps_enter_critical();

	if( ps_is_station_active( p ) )
	{
		ps_exit_critical();
		return;
	}

	p->cstatus = ST_ENA;
	if( !ps_is_active() )
		ps_start();
	ps_exit_critical();
}
#endif


#if PS_EN_STOP_STATION == 1
void 
ps_stop_station( ST_T station )
{
	STATION_T *p;

	p = ps_verify_station( station );
	ps_enter_critical();

	if( ps_is_station_active( p ) )
		p->cstatus = ST_DIS;

	ps_exit_critical();
}
#endif


#if PS_EN_SETQUEUE_STATION == 1
void 
ps_setqueue_station( ST_T station, uchar qd )
{
	STATION_T *p;

	p = ps_verify_station( station );
	ps_enter_critical();

	if( ps_is_station_active( p ) )
	{
		ps_exit_critical();
		return;
	}

	p->qd = ( PSQD_T )qd;
	ps_exit_critical();
}
#endif


void 
ps_send_station( ST_T station, PS_PLBUFF_T *pl )
{
	STATION_T *p;
	
	(void)pl;

	p = ps_verify_station( station );
	ps_enter_critical();

	if( ps_is_station_active( p ) )
		PSQ_INSERT( p->qd, pl );

	ps_exit_critical();
}


void 
ps_timeout( void )
{
	static const PSM_TOUT_T m = { PS_SIG_TOUT };

	ps_dispatch_to_scheduler( ( PSM_T* )&m );
}


void 
ps_rcv_packet( PSM_PKT_T *pm )
{
	ps_dispatch_to_scheduler( ( PSM_T* )pm );
}
