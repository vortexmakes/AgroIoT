/**
 *  \file       GStatus.h
 *  \brief      Specifies the interface of GStatus module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.25.01  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __GSTATUS_H__
#define __GSTATUS_H__

/* ----------------------------- Include files ----------------------------- */
#include "Geo.h"
#include "cbox.h"
#include "IOStatus.h"
#include "BatChr.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define INOUT_LENGTH    4
#define ACCELBR_LENGHT  4

/* ------------------------------- Data types ------------------------------ */
typedef struct GStatus GStatus;
struct GStatus
{
    Geo position;
    CBOX_STR devData;
    IOStatus ioStatus;
    BatChrStatus batChrStatus;
};

/**
 * \warning It must be deprecated in future versions
 */
typedef struct GPS_STR GPS_STR;
struct GPS_STR
{
    char utc[UTC_LENGTH + 1];
    char status[STATUS_LENGTH + 1];
    char latdeg[LATDEG_LENGTH + 1];
    char latmin[LATMIN_LENGTH + 1];
    char lat_ind[LAT_IND_LENGTH + 1];
    char longdeg[LONGDEG_LENGTH + 1];
    char longmin[LONGMIN_LENGTH + 1];
    char long_ind[LONG_IND_LENGTH + 1];
    char speed[SPEED_LENGTH + 1];
    char course[COURSE_LENGTH + 1];
    char date[DATE_LENGTH + 1];
    char in_out_st[INOUT_LENGTH + 1];
    char acbk_st[ACCELBR_LENGHT + 1];
    CBOX_STR cbox;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
rInt GStatus_toGpsStr(GStatus *from, GPS_STR *to);
rInt GStatus_fromGpsStr(GPS_STR *from, GStatus *to);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
