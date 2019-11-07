/**
 *  \file       sprayer.h
 *  \brief      Specifies the interface of sprayer concrete module
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.17.10  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __SPRAYER_H__
#define __SPRAYER_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhevt.h"
#include "device.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
typedef struct Sprayer Sprayer;
struct Sprayer
{
    Device base;
    int nSection;
    int dose;
};

typedef struct EvtSprayerData EvtSprayerData;
struct EvtSprayerData
{
    EvtDevData base;
    int nSection;
    int dose;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
Device *sprayer_ctor(int doseMax);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
