/**
 *  \file       geoMgr.h
 *  \brief      Geolocalization Context Manager.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.02  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Bali�a db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __GEOMGR_H__
#define __GEOMGR_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "settings.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define GEO_INVALID_UTC             "120000"
#define GEO_INVALID_LATITUDE        "38.0030396"
#define GEO_INVALID_LATITUDE_IND    "-"
#define GEO_INVALID_LONGITUDE       "057.3266218"
#define GEO_INVALID_LONGITUDE_IND   "-"
#define GEO_INVALID_SPEED           "000.000"
#define GEO_INVALID_COURSE          "000"
#define GEO_INVALID_DATE            AGROIOT_VERSION_DATE

#define GEO_INVALID_GEOSTAMP    \
                { \
                    {GEO_INVALID_UTC}, {RMC_StatusInvalid}, \
                    {GEO_INVALID_LATITUDE}, {GEO_INVALID_LATITUDE_IND}, \
                    {GEO_INVALID_LONGITUDE}, {GEO_INVALID_LONGITUDE_IND}, \
                    {GEO_INVALID_SPEED}, {GEO_INVALID_COURSE}, \
                    {GEO_INVALID_DATE} \
                }

/* ................................ Signals ................................ */
/* ........................ Declares active object ......................... */
RKH_SMA_DCLR(geoMgr);

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
