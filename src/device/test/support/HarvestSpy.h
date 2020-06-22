/**
 *  \file       HarvestSpy.h
 *  \brief      Specifies the interface of Harvest spy
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
#include "Harvest.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
uint16_t HarvestSpy_getHoard(void);
uint16_t HarvestSpy_getNPail(void);
uint16_t HarvestSpy_getFlow(void);
Device *HarvestSpy_getObj(void);
JobCond *HarvestSpy_getJobCondObj(void);
DevVtbl *HarvestSpy_getVtbl(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
