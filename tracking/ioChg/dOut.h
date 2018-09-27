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
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __DOUT_H__
#define __DOUT_H__

/* ----------------------------- Include files ----------------------------- */
/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
typedef enum dOutSignalId
{
    dOut0, dOut1, dOut2, dOut3, dOut4, dOut5,
    NUM_DOUT_SIGNALS
}dOutSignalId;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void dOut_init(void);
void dOut_set(rui8_t out, rui8_t val);
rui8_t dOut_get(rui8_t out);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
