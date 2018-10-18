/*
 * cbdata.h
 */
#ifndef __CBDATA_H__
#define __CBDATA_H__

typedef struct
{
	short x;
	short y;
	short z;
	unsigned char m;
}ACCEL_T;

typedef struct
{
	unsigned short hoard;	// grain hoard.
	unsigned short pqty;     // pails quantity.
	unsigned short flow;	// grain volume by second.
} GRSENS_T;

typedef struct
{
	unsigned char cmd;
	unsigned char m;
	GRSENS_T h;
	ACCEL_T a;
	unsigned char hum;
}CBOX_STR;

#define CBOX_NOT_PRESENT		0xFFFF
#define EQTYPE_HARVEST_WO_ACC	0xEEEE
#define EQTYPE_HARVEST_W_ACC	0x00FA
#define EQTYPE_HARVEST			0xFF05
#define EQTYPE_SAMPLER			0xFF06
#define EQTYPE_SPRAYER			0xFF07
#define EQTYPE_SPRAYERPRO		0xFF08

CBOX_STR *get_cbdata(void);

#endif
