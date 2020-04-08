/**
 *  \file       GsmMgr_sendSMS.h
 *  \brief      Example: Sending Test SMS using GsmMgr interface.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.25.01  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña  db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __GSMMGR_SENDSMS_H__
#define __GSMMGR_SENDSMS_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "GsmMgr.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void GsmMgr_sendTestSMS(RKH_SMA_T *ao);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
