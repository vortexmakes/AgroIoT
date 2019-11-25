/**
 *  \file settings.h
 *
 *	This module contains application constant declarations
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
/* --------------------------------- Module -------------------------------- */
#ifndef __SETTINGS_H__
#define __SETTINGS_H__

/* ----------------------------- Include files ----------------------------- */
/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define AGROIOT_VERSION_DATE    "190918"
#define ACCELERATION_PERIOD     3
#define MAX_NFRAMES_TOSEND      100

/* Default configurations */
#define ACLIMIT_DFT             6
#define BRLIMIT_DFT             20
#define IP_DFT                  "\"216.75.55.101\""
#define PORT_DFT                "33499"
#define CONNTIME_DFT            60
#define TOTACTTIME              110
#define GPSTTIME                8
#define DIGOUT_DFT              0
#define SMPTIME_DFT             3

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
