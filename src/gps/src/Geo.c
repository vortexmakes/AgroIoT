/**
 *  \file Geo.c
 *
 *	This module contains functions for manipulating GPS serial port and
 *	process NMEA and Ublox frames
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.09.06  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"
#include "rkhassert.h"
#include "rkhitl.h"     /* It's needed to include platform files */
#include "ssp.h"
#include "Geo.h"
#include "rmc.h"
#include "ubxm8parser.h"
#include "SecString.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static SSP gpsParser;
static GeoErrorHandler errorHandler = (GeoErrorHandler)0;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
doSearch(unsigned char c)
{
    ssp_doSearch(&gpsParser, c);
}

static char *
getAttribute(char *attribute, size_t bufSize)
{
    char *pos;

    pos = SecString_strchk(attribute, bufSize);
    if ((pos == (char *)0) && (errorHandler != (GeoErrorHandler)0))
    {
        errorHandler(INDEX_OUT_OF_RANGE);
    }
    else
    {
        pos = attribute;
    }
    return pos;
}

/* ---------------------------- Global functions --------------------------- */
GpsRcvHandler
gps_parserInit(void)
{
    RKH_SR_ALLOC();

    RKH_ENTER_CRITICAL_();
    ssp_init(&gpsParser, &rootGpsParser);
    RKH_EXIT_CRITICAL_();
    return &doSearch;
}

rbool_t
Geo_isValid(Geo *const me)
{
    return me->status[0] == RMC_StatusValid;
}

void 
Geo_init(GeoErrorHandler errHandler)
{
    errorHandler = errHandler;
}

char *
Geo_getLatitude(Geo *const me)
{
    return getAttribute(me->latitude, LATITUDE_LENGTH + 1);
}

char *
Geo_getUtc(Geo *const me)
{
    return getAttribute(me->utc, UTC_LENGTH + 1);
}

char *
Geo_getLongitude(Geo *const me)
{
    return getAttribute(me->longitude, LONGITUDE_LENGTH + 1);
}

char *
Geo_getSpeed(Geo *const me)
{
    return getAttribute(me->speed, SPEED_LENGTH + 1);
}

char *
Geo_getCourse(Geo *const me)
{
    return getAttribute(me->course, COURSE_LENGTH + 1);
}

char *
Geo_getDate(Geo *const me)
{
    return getAttribute(me->date, DATE_LENGTH + 1);
}

/* ------------------------------ End of file ------------------------------ */
