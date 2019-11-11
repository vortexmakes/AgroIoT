/**
 *  \file       dOut.h
 *  \brief      Specification of Digital Outputs HAL.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.17  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __DOUT_H__
#define __DOUT_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
#define DOUT_TIME(t)        (t/DOUT_BASE_TIME)

/* -------------------------------- Constants ------------------------------ */
#define DOUT_BASE_TIME      100

/* ------------------------------- Data types ------------------------------ */
typedef enum DigOutSignalId
{
    dOut1, dOut2,
    NUM_DOUT_SIGNALS
}DigOutSignalId;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void dOut_init(void);
void dOut_set(DigOutSignalId out, ruint val, rui16_t tmr);
ruint dOut_get(DigOutSignalId out);
void dOut_process(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
