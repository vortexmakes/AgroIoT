/**
 *  \file       IOStatus.h
 *  \brief      Specifies the interface of IOStatus module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.28.01  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __IOSTATUS_H_
#define __IOSTATUS_H_

/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
typedef struct IOStatus IOStatus;
struct IOStatus 
{
    rui32_t dIn;
    rui32_t dOut;
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
