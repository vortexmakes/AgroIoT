/**
 *  \file       GStatus.c
 *  \brief      Implements general status module
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.08.02  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include <stdio.h>
#include "GStatus.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
rInt 
GStatus_toGpsStr(GStatus *from, GPS_STR *to)
{
    rInt res;
    Geo *pos;
    char temp[16], *token;

    res = 0;
    if ((to != (GPS_STR *)0) && (from != (GStatus *)0))
    {
        pos = &(from->position);
        strcpy(to->utc, pos->utc);
        strcpy(to->status, pos->status);
        strcpy(temp, pos->latitude);
        token = strchr(temp, '.');
        *token = '\0';
        strcpy(to->latdeg, temp);
        strcpy(to->latmin, (char *)(token + 1));
        strcpy(to->lat_ind, pos->latInd);
        strcpy(temp, pos->longitude);
        token = strchr(temp, '.');
        *token = '\0';
        strcpy(to->longdeg, temp);
        strcpy(to->longmin, (char *)(token + 1));
        strcpy(to->long_ind, pos->longInd);
        strcpy(to->speed, pos->speed);
        strcpy(to->course, pos->course);
        strcpy(to->date, pos->date);
        sprintf(to->in_out_st, "%02X%02X", from->io.digOut, from->io.digIn);
        sprintf(to->acbk_st, "%d", from->batChr);
        to->cbox = from->dev;
    }
    else
    {
        res = 1;
    }
    return res;
}

rInt 
GStatus_fromGpsStr(GPS_STR *from, GStatus *to)
{
    rInt res;
    Geo *pos;
    char temp[32];

    res = 0;
    if ((to != (GStatus *)0) && (from != (GPS_STR *)0))
    {
        pos = &(to->position);
        strcpy(pos->utc, from->utc);
        strcpy(pos->status, from->status);
        sprintf(temp, "%s.%s", from->latdeg, from->latmin);
        strcpy(pos->latitude, temp);
    }
    else
    {
        res = 1;
    }
    return res;
}

/* ------------------------------ End of file ------------------------------ */
