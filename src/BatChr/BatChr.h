/**
 *  \file       BatChr.h
 *  \brief      Specifies the interface of battery charger module
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.29.01  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __BATCHR_H__
#define __BATCHR_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhevt.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
typedef enum BatChrStatus BatChrStatus;
enum BatChrStatus
{
    LINE_BATT,
    EXT_PWR_OK = LINE_BATT,
    LINE_BATTCHG,
    LINE_NOBATT,
    NOLINE_BATT,
    EXT_PWR_FAIL = NOLINE_BATT,
    NOLINE_BATTDIS
};

/* ------------------------------- Data types ------------------------------ */
typedef struct BatChrEvt BatChrEvt;
struct BatChrEvt
{
    RKH_EVT_T base;
    BatChrStatus status;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
BatChrStatus BatChr_getStatus(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
