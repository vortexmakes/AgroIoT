/**
 *  \file       ubxm8parser.c
 *  \brief      ...
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.09.07  Daba  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "ubxm8parser.h"
#include "geoMgr.h"
#include "signals.h"
#include "events.h"
#include <stdlib.h>

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static rui8_t ubxm8parser;
static RMC_t rmcCurrent, rmcLast;
static RmcEvt rmcEvt;
static char *p;

SSP_DCLR_NORMAL_NODE rootGpsParser;
SSP_DCLR_TRN_NODE rmcTimeInt, rmcTimeDec, rmcStatus, 
                  rmcLongitudeInt, rmcLongitudeDec, rmcLongitudInd, 
                  rmcLatitudeInt, rmcLatitudeDec, rmcLatitudeInd,
                  rmcSpeedInt, rmcSpeedDec, rmcTrack, rmcDate,
                  rmcMagneticOff, rmcMagneticInd, rmcCheckSum;

/* ----------------------- Local function prototypes ----------------------- */
static void rmcUpdate(unsigned char pos);
static void toRmcTimeInt(unsigned char pos);
static void rmcTimeIntCollect(unsigned char c);
static void toRmcTimeDec(unsigned char pos);
static void rmcTimeDecCollect(unsigned char c);
static void toRmcStatus(unsigned char pos);
static void rmcStatusCollect(unsigned char c);
static void toRmcLongitudeInt(unsigned char pos);
static void rmcLongitudeIntCollect(unsigned char c);
static void toRmcLongitudeDec(unsigned char pos);
static void toRmcLongitudeInd(unsigned char pos);
static void toRmcLatitudeInt(unsigned char pos);
static void toRmcLatitudeDec(unsigned char pos);
static void toRmcLatitudeInd(unsigned char pos);
static void toSpeed(unsigned char pos);
static void toTrack(unsigned char pos);
static void toDate(unsigned char pos);
static void toMagneticOff(unsigned char pos);
static void toMagneticOffInd(unsigned char pos);

/* ---------------------------- Local functions ---------------------------- */

SSP_CREATE_NORMAL_NODE(rootGpsParser);
SSP_CREATE_BR_TABLE(rootGpsParser)
	//SSPBR("$GNRMC,",      rmcUpdate,      &rootGpsParser),
	SSPBR("$GNRMC,",    toRmcTimeInt,  &rmcTimeInt),
SSP_END_BR_TABLE

SSP_CREATE_TRN_NODE(rmcTimeInt, rmcTimeIntCollect);
SSP_CREATE_BR_TABLE(rmcTimeInt)
	SSPBR(".",      toRmcTimeDec,  &rmcTimeDec),
	SSPBR("\r\n",   toRmcTimeDec,  &rootGpsParser),
SSP_END_BR_TABLE

SSP_CREATE_TRN_NODE(rmcTimeDec, rmcTimeDecCollect);
SSP_CREATE_BR_TABLE(rmcTimeDec)
	SSPBR(",",      toRmcStatus,  &rmcStatus),
	SSPBR("\r\n",   toRmcTimeDec,  &rootGpsParser),
SSP_END_BR_TABLE

SSP_CREATE_TRN_NODE(rmcStatus, rmcStatusCollect);
SSP_CREATE_BR_TABLE(rmcStatus)
	SSPBR(",",   toRmcLongitudeInt,  &rmcLongitudeInt),
	SSPBR("\r\n",   toRmcTimeDec,  &rootGpsParser),
SSP_END_BR_TABLE

SSP_CREATE_TRN_NODE(rmcLongitudeInt, rmcLongitudeIntCollect);
SSP_CREATE_BR_TABLE(rmcLongitudeInt)
SSP_END_BR_TABLE

static void
toRmcTimeInt(unsigned char pos)
{
    (void)pos;

    memset(&rmcCurrent, '\0', sizeof(rmcCurrent));
    p = rmcCurrent.timeInt;
}

static void
rmcTimeIntCollect(unsigned char c)
{
    (void)c;
}

static void
toRmcTimeDec(unsigned char pos)
{
    (void)pos;

}

static void
rmcTimeDecCollect(unsigned char c)
{
    (void)c;
}


static void
toRmcStatus(unsigned char pos)
{
    (void)pos;

    p = &rmcCurrent.status;
}

static void
rmcStatusCollect(unsigned char pos)
{
    (void)pos;

    p = &rmcCurrent.status;
}

static void
toRmcLongitudeInt(unsigned char pos)
{
    (void)pos;

}

static void
rmcLongitudeIntCollect(unsigned char pos)
{
    (void)pos;

}
static void
rmcLast_update(unsigned char pos)
{
    (void)pos;

    rmcLast = rmcCurrent;

    rmcEvt.p = &rmcLast;

    RKH_SET_STATIC_EVENT(RKH_UPCAST(RKH_EVT_T, &rmcEvt), evRMC);

    RKH_SMA_POST_FIFO(geoMgr, RKH_UPCAST(RKH_EVT_T, &rmcEvt), &ubxm8parser);
}


/* ---------------------------- Global functions --------------------------- */
void
ubx8mparser_init(void)
{
    RKH_TR_FWK_ACTOR(&ubxm8parser, "ubxm8parser");
}

/* ------------------------------ End of file ------------------------------ */
