/**
 *  \file       StatQue.h
 *  \brief      Specifies the status queue's adapter.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.18.02  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __STATQUE_H__
#define __STATQUE_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"
#include "GStatus.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
rui16_t StatQue_init(void);
rui16_t StatQue_getNumElem(void);
rbool_t StatQue_isEmpty(void);
rInt StatQue_remove(GStatus *elem);
rInt StatQue_delete(rui16_t *nElem);
rInt StatQue_read(GStatus *elem);
rInt StatQue_put(GStatus *elem);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
