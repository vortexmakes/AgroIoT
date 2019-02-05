/**
 *  \file       YFrame.c
 *  \brief      Implements Yipies frame (remote protocol) module
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.02  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "YFrame.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define FLG_GPS_VALID   1
#define FLG_HISTORY     2
#define FLG_MOVING      4

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
ruint 
YFrame_makeFrame(RawData *from, char *to)
{
    ruint size;

    if ((from != (RawData *)0) && (to != (char *)0))
    {
        size += 1;
    }
    else
    {
        size = 0;
    }
    return size;
}

rInt 
YFrame_getFlags(RawData *from, rui8_t *flags, rui8_t type)
{
    rInt res;
    rui8_t flag;

    res = 0;
    flag = 0;
    if ((from != (RawData *)0) && (flags != (rui8_t *)0))
    {
	    flag |= (Geo_isValid(&from->position) == 1) ? FLG_GPS_VALID : 0;
    	flag |= (type == YFRAME_MGP_TYPE) ? FLG_HISTORY : 0;
	    flag |= (cbox_isMoving(&from->dev) == 1) ? FLG_MOVING : 0;
    	flag |= (BatChr_getStatus() << 3);
        *flags = flag;
    }
    else
    {
        res = 1;
    }
    return res;
}

/* ------------------------------ End of file ------------------------------ */
