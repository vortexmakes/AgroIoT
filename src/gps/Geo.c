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
#include "rkh.h"
#include "ssp.h"
#include "Geo.h"
#include "rmc.h"
#include "ubxm8parser.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static SSP gpsParser;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
doSearch(unsigned char c)
{
    ssp_doSearch(&gpsParser, c);
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

/* ------------------------------ End of file ------------------------------ */
