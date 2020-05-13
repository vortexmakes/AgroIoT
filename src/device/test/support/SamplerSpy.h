/**
 *  \file       SamplerSpy.h
 *  \brief      Specifies the interface of Sampler spy
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.05.12  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __SAMPLERSPY_H__
#define __SAMPLERSPY_H__

/* ----------------------------- Include files ----------------------------- */
#include "Sampler.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
int SamplerSpy_getHarness(void);
int SamplerSpy_getDeep(void);
bool SamplerSpy_getIsValid(void);
Device *SamplerSpy_getObj(void);
JobCond *SamplerSpy_getJobCondObj(void);
DevVtbl *SamplerSpy_getVtbl(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
