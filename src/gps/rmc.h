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
#define RMC_UTC_LEN             9
#define RMC_LATITUDE_LEN        10
#define RMC_LATITUDE_DEG_LEN    2
#define RMC_LONGITUDE_LEN       11
#define RMC_LONGITUDE_DEG_LEN   3
#define RMC_SOG_LEN             7
#define RMC_COG_LEN             7
#define RMC_DATE_LEN            6
#define RMC_MAGVAR_LEN          7
#define RMC_INDICATOR_LEN       1

#define RMC_StatusInvalid   'V'
#define RMC_StatusValid     'A'

/* ------------------------------- Data types ------------------------------ */
typedef enum
{
    rmcUtc,
    rmcStatus,
    rmcLatitude,
    rmcNorthingIndicator,
    rmcLongitude,
    rmcEastingIndicator,
    rmcSog,
    rmcCog,
    rmcDate,
    rmcMagneticVariation,
    rmcMagneticVarIndicator,
    rmcModeIndicator,
    rmcNavigationalStatus,

    RMC_FIELDS_NUM
}RMC_fields;

typedef struct
{
    /* Universal time coordinated hhmmss.ss*/
    char utc[RMC_UTC_LEN + 1];

    /* Status A=Valid, V=Invalid */
    char status[RMC_INDICATOR_LEN + 1];

    /* Latitude ddmm.mmmmm */
    char latitude[RMC_LATITUDE_LEN + 1];

    /* Northing Indicator N=North, S=South */
    char northingIndicator[RMC_INDICATOR_LEN + 1];

    /* Longitud dddmm.mmmmm */
    char longitude[RMC_LONGITUDE_LEN + 1];

    /* Easting Indicator E=East, W=West */
    char eastingIndicator[RMC_INDICATOR_LEN + 1];

    /* Speed Over Ground [knots] sss.sss */
    char sog[RMC_SOG_LEN + 1];

    /* Course Over Ground [degrees] ddd.ddd */
    char cog[RMC_COG_LEN + 1];

    /* Universarl date coordinate ddmmyy */
    char date[RMC_DATE_LEN + 1];

    /* Magnetic Variation [degrees] */
    char magneticVariation[RMC_MAGVAR_LEN + 1];

    /* Magnetic Variation Indicator E=East, W=West */
    char magneticVarIndicator[RMC_INDICATOR_LEN + 1];

    /* Mode Indicator
     * A=Autonomus, D=Differential, R=Fixed RTK,
     * F=Float RTK, E=DeadReckoning, N=None
     */
    char modeIndicator[RMC_INDICATOR_LEN + 1];

    /* Navigational Status Indicator
     * S=Safe, C=Caution, U=Unsafe, V=NotValid
     */
    char navigationalStatus[RMC_INDICATOR_LEN + 1];
}Rmc;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
char rmc_status(Rmc *p);

int rmc_timeUpdate(Rmc *p);

Time *rmc_timeGet(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
