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
#include <string.h>
#include <stdio.h>
#include "YFrame.h"
#include "conmgr.h"
#include "BatChr.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define YFRAME_ID               "!"
#define YFRAME_MARK             "|"
#define YFRAME_SEPARATOR        ","

#define FLG_GPS_VALID           1
#define FLG_HISTORY             2
#define FLG_MOVING              4

static const char *frameType[] = {"0", "1"};

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
ruint 
YFrame_make(GStatus *from, char *to)
{
    ruint size;
    char *frame, temp[16];
    Geo *position;
    IOStatus *io;
    CBOX_STR *dev;
    rui8_t flags;

    size = 0;
    frame = to;
    position = &from->position;
    io = &from->io;
    dev = &from->dev;

    if ((from != (GStatus *)0) && (to != (char *)0))
    {
        strcat(frame, YFRAME_ID);
        strcat(frame, frameType[YFRAME_SGP_TYPE]);
        strcat(frame, YFRAME_MARK);
        (rInt)YFrame_getFlags(from, &flags, YFRAME_SGP_TYPE);
        sprintf(temp, "%02x", flags);
        strcat(frame, temp);
        strcat(frame, ConMgr_Imei());
        strcat(frame, YFRAME_SEPARATOR);
        strcat(frame, position->utc);
        strcat(frame, YFRAME_SEPARATOR);
        strcat(frame, position->latInd);
        strcat(frame, position->latitude);
        strcat(frame, YFRAME_SEPARATOR);
        strcat(frame, position->longInd);
        strcat(frame, position->longitude);
        strcat(frame, YFRAME_SEPARATOR);
        strcat(frame, position->speed);
        strcat(frame, YFRAME_SEPARATOR);
        strcat(frame, position->course);
        strcat(frame, YFRAME_SEPARATOR);
        strcat(frame, position->date);
        strcat(frame, YFRAME_SEPARATOR);
        sprintf(temp, "%02X%02X,", io->digOut, io->digIn);
        strcat(frame, temp);
        sprintf(temp, "%04X,", dev->h.hoard);
        strcat(frame, temp);
        sprintf(temp, "%02X,", dev->h.pqty);
        strcat(frame, temp);
        sprintf(temp, "%02X,", dev->hum);
        strcat(frame, temp);
        sprintf(temp, "%04X,", (rui16_t)(dev->a.x));
        strcat(frame, temp);
        sprintf(temp, "%04X,", (rui16_t)(dev->a.y));
        strcat(frame, temp);
        sprintf(temp, "%04X,", (rui16_t)(dev->a.z));
        strcat(frame, temp);
        sprintf(temp, "%d", from->batChr);
        strcat(frame, temp);
        strcat(frame, YFRAME_MARK);
        size = strlen(frame);
    }
    else
    {
    }
    return size;
}

rInt 
YFrame_getFlags(GStatus *from, rui8_t *flags, rui8_t type)
{
    rInt res;
    rui8_t flag;

    res = 0;
    flag = 0;
    if ((from != (GStatus *)0) && (flags != (rui8_t *)0))
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
