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
#define GET_RMC_FIELDS(r) \
                { \
                    { (r).utc, RMC_UTC_LEN },  \
                    { (r).status, RMC_INDICATOR_LEN },   \
                    { (r).latitude, RMC_LATITUDE_LEN },  \
                    { (r).northingIndicator, RMC_INDICATOR_LEN },  \
                    { (r).longitude, RMC_LONGITUDE_LEN },  \
                    { (r).eastingIndicator, RMC_INDICATOR_LEN },  \
                    { (r).sog, RMC_SOG_LEN },  \
                    { (r).cog, RMC_COG_LEN },  \
                    { (r).date, RMC_DATE_LEN },  \
                    { (r).magneticVariation, RMC_MAGVAR_LEN },  \
                    { (r).magneticVarIndicator, RMC_INDICATOR_LEN },  \
                    { (r).modeIndicator, RMC_INDICATOR_LEN },  \
                    { (r).navigationalStatus, RMC_INDICATOR_LEN }  \
                }

/* ------------------------------- Constants ------------------------------- */
#define NMEA_FRAME_MAX_SIZE     82 // NMEA 0183 specification

/* ---------------------------- Local data types --------------------------- */
typedef struct
{
    char *p;
    unsigned char size;
} RmcField_t;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static rui8_t ubxm8parser;
static RMC_t rmcCurrent, rmcLast;
static RmcEvt rmcEvt;
static unsigned char *p;
static char *pF;
static unsigned char nmeaFrame[NMEA_FRAME_MAX_SIZE];
static unsigned char checksum;

static const RmcField_t rmcFields[RMC_FIELDS_NUM] = GET_RMC_FIELDS(rmcCurrent);
static const RmcField_t *pRmcF;

SSP_DCLR_NORMAL_NODE rootGpsParser;
SSP_DCLR_TRN_NODE inNMEA, inRMC, inChk;

/* ----------------------- Local function prototypes ----------------------- */
static void nmeaStarts(unsigned char pos);
static void nmeaCollect(unsigned char c);
static void chkCollect(unsigned char c);
static void rmcStarts(unsigned char pos);
static void rmcCollect(unsigned char c);
static void rmcIncField(unsigned char pos);
static void chkCmpAndPublish(unsigned char pos);

/* ---------------------------- Local functions ---------------------------- */

SSP_CREATE_NORMAL_NODE(rootGpsParser);
SSP_CREATE_BR_TABLE(rootGpsParser)
	SSPBR("$",    nmeaStarts,  &inNMEA),
SSP_END_BR_TABLE

SSP_CREATE_TRN_NODE(inNMEA, nmeaCollect);
SSP_CREATE_BR_TABLE(inNMEA)
	SSPBR("GNRMC,",    rmcStarts, &inRMC),
	SSPBR("\n",       NULL,      &rootGpsParser),
SSP_END_BR_TABLE

SSP_CREATE_TRN_NODE(inRMC, rmcCollect);
SSP_CREATE_BR_TABLE(inRMC)
	SSPBR(",",    rmcIncField,   &inRMC),
	SSPBR("*",    NULL,  &inChk),
	SSPBR("\n",   NULL,  &rootGpsParser),
SSP_END_BR_TABLE

SSP_CREATE_TRN_NODE(inChk, chkCollect);
SSP_CREATE_BR_TABLE(inChk)
	SSPBR("\n",   chkCmpAndPublish,  &rootGpsParser),
SSP_END_BR_TABLE

static void
nmeaStarts(unsigned char pos)
{
    (void)pos;

    memset(&nmeaFrame, '\0', sizeof(nmeaFrame));
    p = nmeaFrame;
    checksum = 0;
}

static void
nmeaCollect(unsigned char c)
{
    if(p >= nmeaFrame + NMEA_FRAME_MAX_SIZE)
        return;

	*p++ = c;

    if (c == '*')
        return;

    checksum ^= c;
}

static void
chkCollect(unsigned char c)
{
    if (c == '\r' || c == '\n')
        return;

    if(p >= nmeaFrame + NMEA_FRAME_MAX_SIZE)
        return;

	*p++ = c;
}

static void
rmcStarts(unsigned char pos)
{
	(void)pos;

    pRmcF = &rmcFields[0];
    pF = pRmcF->p;
}

static void
rmcCollect(unsigned char c)
{
    nmeaCollect(c);

	if (pRmcF >= rmcFields + RMC_FIELDS_NUM)
		return;

    if( pF >= (pRmcF->p + pRmcF->size) )
        return;

    if (c == ',')
        return;

    *pF++ = c;

}

static void
rmcIncField(unsigned char pos)
{
	(void)pos;

    ++pRmcF;

	if(pRmcF >= rmcFields + RMC_FIELDS_NUM)
		return;

    pF = pRmcF->p;
}

static void
chkCmpAndPublish(unsigned char pos)
{
    unsigned char fchk;

	(void)pos;

	fchk = (unsigned char)strtol((const char *)(p-2), NULL, 16);

    if(fchk != checksum)
        return;

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
