/**
 *  \file       dIn.h
 *  \brief      Specification of Digital Inputs HAL and change detection.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.17  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __DIN_H__
#define __DIN_H__

/* ----------------------------- Include files ----------------------------- */
/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
typedef enum dInSignalId
{
    dIn1, dIn2,
    NUM_DIN_SIGNALS
}dInSignalId;

typedef enum dInPullSelct
{
    pullSelect1, pullSelect2,
    NUM_PULL_SELCT
}pullSelectId;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void dIn_init(void);
void dIn_scan(void);
void dIn_pullSelect(pullSelectId ps, uint8_t value);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
