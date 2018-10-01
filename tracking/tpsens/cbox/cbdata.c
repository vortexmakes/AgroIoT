
#include "tplink.h"
#include "tplhal.h"
#include "ps.h"
#include "cboxcmd.h"
#include "sleep.h"
#include "gps.h"
#include <stdio.h>
#include "cbdata.h"

#define SIZE_CBDATA_BUFF	10

static char cbdata_buff[SIZE_CBDATA_BUFF];

uchar
get_movdet_stat(void)
{
	GPS_STR *p;
	p = (GPS_STR *)pstr;

	return p->cbox.m != 0;
}

char *
get_cb_horde(void)
{
	GPS_STR *p;
	p = (GPS_STR *)pstr;

	sprintf( cbdata_buff, "%04X", p->cbox.h.hoard );
	return cbdata_buff;
}

char *
get_cb_paddles( void *pstr )
{
	GPS_STR *p;
	p = (GPS_STR *)pstr;

	sprintf( cbdata_buff, "%02X", p->cbox.h.pqty );
	return cbdata_buff;
}

char *
get_cb_hum( void *pstr )
{
	GPS_STR *p;
	p = (GPS_STR *)pstr;

	sprintf( cbdata_buff, "%02X", p->cbox.hum );
	return cbdata_buff;
}

char *
get_cb_ax( void *pstr )
{
	GPS_STR *p;
	p = (GPS_STR *)pstr;

	sprintf( cbdata_buff, "%03X", p->cbox.a.x );
	return cbdata_buff;
}

char *
get_cb_ay( void *pstr )
{
	GPS_STR *p;
	p = (GPS_STR *)pstr;

	sprintf( cbdata_buff, "%03X", p->cbox.a.y );
	return cbdata_buff;
}

char *
get_cb_az( void *pstr )
{
	GPS_STR *p;
	p = (GPS_STR *)pstr;

	sprintf( cbdata_buff, "%03X", p->cbox.a.z );
	//sprintf( cbdata_buff, "%04X", p->cbox.h.flow );
	return cbdata_buff;
}
