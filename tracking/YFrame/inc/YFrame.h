/**
 *  \file       YFrame.h
 *  \brief      Specifies the interface of Yipies frame (remote protocol) 
 *              module
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.02  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __YFRAME_H__
#define __YFRAME_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"
#include "RawData.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define YFRAME_SGP_TYPE        0
#define YFRAME_MGP_TYPE        1

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
ruint YFrame_makeFrame(RawData *from, char *to);
rInt YFrame_getFlags(RawData *from, rui8_t *flags, rui8_t type);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
