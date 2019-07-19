/**
 *  \file       cbox.h
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
/* --------------------------------- Module -------------------------------- */
#ifndef __CBOX_H__
#define __CBOX_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define CBOX_NOT_PRESENT		0xFFFF
#define EQTYPE_HARVEST_WO_ACC	0xEEEE
#define EQTYPE_HARVEST_W_ACC	0x00FA
#define EQTYPE_HARVEST			0xFF05
#define EQTYPE_SAMPLER			0xFF06
#define EQTYPE_SPRAYER			0xFF07
#define EQTYPE_SPRAYERPRO		0xFF08

/* ------------------------------- Data types ------------------------------ */
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

/* -------------------------- External variables --------------------------- */
extern ruint tpSens;

/* -------------------------- Function prototypes -------------------------- */
CBOX_STR *cbox_getInstance(void);
rbool_t cbox_isMoving(CBOX_STR *const me);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */

