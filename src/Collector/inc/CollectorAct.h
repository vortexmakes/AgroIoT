/**
 *  \file       CollectorAct.h
 *  \brief      Active object's action specifications.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __COLLECTORACT_H__
#define __COLLECTORACT_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "Collector.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void Collector_ctor(void);

/* ........................ Declares effect actions ........................ */
void Collector_init(Collector *const me, RKH_EVT_T *pe);
void Collector_updatePosition(Collector *const me, RKH_EVT_T *pe);
void Collector_updateDigOut(Collector *const me, RKH_EVT_T *pe);
void Collector_publishCurrStatus(Collector *const me, RKH_EVT_T *pe);
void Collector_updateDigInTestDevNull(Collector *const me, RKH_EVT_T *pe);
void Collector_updateDigIn(Collector *const me, RKH_EVT_T *pe);
void Collector_updateAndTestDevData(Collector *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
void Collector_enActive(Collector *const me);
void Collector_initAndTestDevNull(Collector *const me);

/* ......................... Declares exit actions ......................... */
void Collector_exActive(Collector *const me);

/* ............................ Declares guards ............................ */

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
