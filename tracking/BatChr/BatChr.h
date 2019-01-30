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

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
typedef enum BatChrStatus BatChrStatus;
enum BatChrStatus 
{
    EXT_POWER_FAIL
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
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */