/**
 *  \file       DeviceMgr.h
 *  \brief      TpSensor Externals Devices Server
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.02  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __DEVICESERVER_H__
#define __DEVICESERVER_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define DEV_MAX_NUM_TRIES       10
#define DEV_MAX_NUM_BACKOFF     5

/* ................................ Signals ................................ */
/* ........................ Declares active object ......................... */
typedef struct DeviceMgr DeviceMgr;
struct DeviceMgr
{
    RKH_SMA_T ao;       /* Base structure */
    RKHTmEvt tmr;
    uint32_t pollCycle;
    uint32_t tries;
    uint32_t backoff;
};

RKH_SMA_DCLR_TYPE(DeviceMgr, deviceMgr);

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
