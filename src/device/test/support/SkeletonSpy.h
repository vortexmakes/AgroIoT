/**
 *  \file       SkeletonSpy.h
 *  \brief      Specifies the interface of Skeleton spy
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.18.06  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __SKELETONSPY_H__
#define __SKELETONSPY_H__

/* ----------------------------- Include files ----------------------------- */
#include "Skeleton.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
int SkeletonSpy_getX(void);
Device *SkeletonSpy_getObj(void);
JobCond *SkeletonSpy_getJobCondObj(void);
DevVtbl *SkeletonSpy_getVtbl(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
