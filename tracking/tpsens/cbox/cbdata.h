/*
 * cbdata.h
 */
#ifndef __CBDATA_H__
#define __CBDATA_H__

#include "mytypes.h"

typedef struct
{
	short x;
	short y;
	short z;
	uchar m;
}ACCEL_T;

typedef struct
{
	ushort hoard;	// grain hoard.
	ushort pqty;     // pails quantity.
	ushort flow;	// grain volume by second.
} GRSENS_T;

typedef struct
{
	uchar cmd;
	uchar m;
	GRSENS_T h;
	ACCEL_T a;
	uchar hum;
}CBOX_STR;

#define CBOX_NOT_PRESENT		0xFFFF
#define EQTYPE_HARVEST_WO_ACC	0xEEEE
#define EQTYPE_HARVEST_W_ACC	0x00FA
#define EQTYPE_HARVEST			0xFF05
#define EQTYPE_SAMPLER			0xFF06
#define EQTYPE_SPRAYER			0xFF07
#define EQTYPE_SPRAYERPRO		0xFF08

extern MUInt cbfrm_count;

uchar get_movdet_stat( void *pstr );
char * get_cb_horde( void *pstr );
char * get_cb_paddles( void *pstr );
char * get_cb_hum( void *pstr );
char * get_cb_ax( void *pstr );
char * get_cb_ay( void *pstr );
char * get_cb_az( void *pstr );

#endif
