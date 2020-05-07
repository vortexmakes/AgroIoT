/**
 *  \file       Geo.c
 *  \brief      Trivial stub for frame converter program
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.07.05  LeFr  v1.0.00  ---
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "Geo.h"
#include "rmc.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
rbool_t 
Geo_isValid(Geo *const me)
{
    return me->status[0] == RMC_StatusValid;
}

char *
Geo_getUtc(Geo *const me)
{
    return me->utc;
}

char *
Geo_getLatInd(Geo *const me)
{
    return me->latInd;
}

char *
Geo_getLatitude(Geo *const me)
{
    return me->latitude;
}

char *
Geo_getLongInd(Geo *const me)
{
    return me->longInd;
}

char *
Geo_getLongitude(Geo *const me)
{
    return me->longitude;
}

char *
Geo_getSpeed(Geo *const me)
{
    return me->speed;
}

char *
Geo_getCourse(Geo *const me)
{
    return me->course;
}

char *
Geo_getDate(Geo *const me)
{
    return me->date;
}

/* ------------------------------ End of file ------------------------------ */
