/**
 *  \file       Collector.h
 *  \brief      Specifies the interface of Collector module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.11.22  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __COLLECTOR_H__
#define __COLLECTOR_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhsma.h"
#include "GStatus.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ................................ Signals ................................ */
/* ................................. Events ................................ */
/* ........................ Declares active object ......................... */
typedef struct Device Device;
typedef struct Collector Collector;
struct Collector
{
    RKH_SMA_T base;
    RKHSmaVtbl vtbl;            /* Virtual table */
    GStatus status;
    Device *dev;
};

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
RKH_SMA_DCLR_TYPE(Collector, collector);

/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
