/**
 *  \file Geo.h
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
 *  DaBa  Dario Balina db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __GEO_H__
#define __GEO_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define DEG_LENGTH          7
#define MIN_LENGTH          8
#define UTC_LENGTH          6
#define STATUS_LENGTH       1
#define LATITUDE_LENGTH     10
#define LAT_IND_LENGTH      1
#define LONGDEG_LENGTH      DEG_LENGTH
#define LONGMIN_LENGTH      MIN_LENGTH
#define LATDEG_LENGTH       DEG_LENGTH
#define LATMIN_LENGTH       MIN_LENGTH
#define LONGITUDE_LENGTH    11
#define LONG_IND_LENGTH     1
#define SPEED_LENGTH        7
#define COURSE_LENGTH       3
#define DATE_LENGTH         6

/* ------------------------------- Data types ------------------------------ */
typedef struct Geo Geo;
struct Geo
{
    char utc[UTC_LENGTH + 1];
    char status[STATUS_LENGTH + 1];
    char latitude[LATITUDE_LENGTH + 1];
    char latInd[LAT_IND_LENGTH + 1];
    char longitude[LONGITUDE_LENGTH + 1];
    char longInd[LONG_IND_LENGTH + 1];
    char speed[SPEED_LENGTH + 1];
    char course[COURSE_LENGTH + 1];
    char date[DATE_LENGTH + 1];
};

typedef enum GeoErrorCode GeoErrorCode;
enum GeoErrorCode
{
    NO_ERRORS,
    INDEX_OUT_OF_RANGE
};

typedef void (*GpsRcvHandler)(unsigned char c);
typedef void (*GeoErrorHandler)(GeoErrorCode errCode);

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
GpsRcvHandler gps_parserInit(void);
rbool_t Geo_isValid(Geo *const me);
void Geo_init(GeoErrorHandler errHandler);
char *Geo_getLatitude(Geo *const me);
char *Geo_getUtc(Geo *const me);
char *Geo_getLongitude(Geo *const me);
char *Geo_getSpeed(Geo *const me);
char *Geo_getCourse(Geo *const me);
char *Geo_getDate(Geo *const me);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
