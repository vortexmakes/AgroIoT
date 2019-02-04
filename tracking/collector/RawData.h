/**
 *  \file       RawData.h
 *  \brief      Specifies the interface of RawData module.
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
#ifndef __RAWDATA_H__
#define __RAWDATA_H__

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
/* ................................ Signals ................................ */
/* ................................. Events ................................ */
/* ........................ Declares active object ......................... */
typedef struct RawData RawData;
struct RawData 
{
    Geo position;
    CBOX_STR dev;
    IOStatus io;
    BatChrStatus batChr;
};

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
