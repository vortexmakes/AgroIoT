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
    }
    else
    {
        res = 1;
    }
    return res;
}

/* ------------------------------ End of file ------------------------------ */
