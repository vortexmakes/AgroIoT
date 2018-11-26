/**
 *  \file       sprayerSpy.h
 *  \brief      Specifies the interface of sprayer spy
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.18.10  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __SPRAYERSPY_H__
#define __SPRAYERSPY_H__

/* ----------------------------- Include files ----------------------------- */
#include "sprayer.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
int sprayerSpy_getNSection(void);
int sprayerSpy_getDose(void);
int sprayerSpy_getNSectionMax(void);
Device *sprayerSpy_getObj(void);
JobCond *sprayerSpy_getJobCondObj(void);
DevVtbl *sprayerSpy_getVtbl(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
