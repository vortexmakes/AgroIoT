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
#define RMC_TIME_INT_LEN        6
#define RMC_TIME_DEC_LEN        2
#define RMC_LATITUDE_INT_LEN    4
#define RMC_LATITUDE_DEC_LEN    5
#define RMC_LONGITUDE_INT_LEN   5
#define RMC_LONGITUDE_DEC_LEN   5
#define RMC_SPEED_INT_LEN       3
#define RMC_SPEED_DEC_LEN       3
#define RMC_TRACK_LEN           5
#define RMC_DATE_LEN            6
#define RMC_MAGVAR_LEN          5

#define RMC_StatusVoid      'V'
#define RMC_StatusActive    'A'

/* ------------------------------- Data types ------------------------------ */
typedef struct
{
    char timeInt[RMC_TIME_INT_LEN+1];
    char timeDec[RMC_TIME_DEC_LEN+1];
    char status;
    char latitudeInt[RMC_LATITUDE_INT_LEN+1];
    char latitudeDec[RMC_LATITUDE_DEC_LEN+1];
    char latitudeInd;
    char longitudeInt[RMC_LONGITUDE_INT_LEN+1];
    char longitudeDec[RMC_LONGITUDE_DEC_LEN+1];
    char speedInt[RMC_SPEED_INT_LEN+1];
    char speedDec[RMC_SPEED_DEC_LEN+1];
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
