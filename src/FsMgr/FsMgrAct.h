/**
 *  \file       FsMgr.h
 *  \brief      Active object's action specifications.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __FSMGRACT_H__
#define __FSMGRACT_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "FsMgr.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* ........................ Declares effect actions ........................ */
void FsMgr_DiskDisconnectedToDiskConnectedExt1(FsMgr *const me, RKH_EVT_T *pe);
void FsMgr_FsActiveToC0Ext3(FsMgr *const me, RKH_EVT_T *pe);
void FsMgr_FsActiveToC0Ext4(FsMgr *const me, RKH_EVT_T *pe);
void FsMgr_FsActiveToC0Ext5(FsMgr *const me, RKH_EVT_T *pe);
void FsMgr_FsActiveToC0Ext6(FsMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */

/* ......................... Declares exit actions ......................... */

/* ............................ Declares guards ............................ */
rbool_t FsMgr_isCondC0ToFsActive9(FsMgr *const me, RKH_EVT_T *pe);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
