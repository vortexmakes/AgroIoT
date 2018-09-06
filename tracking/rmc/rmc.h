/**
 *  \file rmc.h
 *
 *	This module contains functions for manipulating RMC NMEA Frame,
 *	including functions for determining what the current time is convert
 *	geolocalization and check it´s reliability.
 */
/* -------------------------- Development history -------------------------- */
/*
 *  2018.09.05  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __RMC_H__
#define __RMC_H__

/* ----------------------------- Include files ----------------------------- */
#include "epoch.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define RMC_TIME_LEN        6
#define RMC_LATITUDE_LEN    9
#define RMC_LONGITUDE_LEN   9
#define RMC_SPEED_LEN       5
#define RMC_TRACK_LEN       5
#define RMC_DATE_LEN        6
#define RMC_MAGVAR_LEN      5

#define RMC_StatusVoid      'V'
#define RMC_StatusActive    'A'

/* ------------------------------- Data types ------------------------------ */
typedef struct
{
    char time[RMC_TIME_LEN+1];
    char status;
    char latitude[RMC_LATITUDE_LEN+1];
    char latitudeInd;
    char longitude[RMC_LONGITUDE_LEN+1];
    char speed[RMC_SPEED_LEN+1];
    char track[RMC_TRACK_LEN+1];
    char date[RMC_DATE_LEN+1];
    char MagneticOff[RMC_MAGVAR_LEN+1];
    char MagneticOffInd;
}RMC_t;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
#define rmc_status(x)   'A'
//char rmc_status( RMC_t *p);

#define rmc_time(x)     -1
//Epoch rmc_time(RMC_t *p);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
