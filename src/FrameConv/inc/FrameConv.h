/**
 *  \file       FrameConv.h
 *  \brief      Specifies the interface of FramConv module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.06.05  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __FRAMECONV_H__
#define __FRAMECONV_H__

/* ----------------------------- Include files ----------------------------- */
#include "GStatus.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define SIZEOF_GSTATUS_STM32    (80 + 4)    /* offsetof(checksum) + */
                                            /* sizeof(checksum) */

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
int FrameConv_STM32ToX86(GStatus *to, uint8_t from[], size_t size);
int FrameConv_GStatusToFrame(uint8_t *to, GStatus *from);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
