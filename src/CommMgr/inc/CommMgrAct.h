/**
 *  \file       CommMgr.h
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
#ifndef __COMMMGRACT_H__
#define __COMMMGRACT_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "CommMgr.h"

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
void CommMgr_ToIdleExt0(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_IdleToActiveExt1(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_ReceivingAckToC1Ext6(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_ReceivingAckToEndCycleExt7(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_SendingEndOfHistToEndCycleExt9(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_SendingHistToC0Ext12(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_SendingHistToEndCycleExt13(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_C1ToEndCycleExt14(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_C3ToSendingHistExt18(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_ActiveToActiveLoc0(CommMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
void CommMgr_enWaitSync(CommMgr *const me);
void CommMgr_enSendingStatus(CommMgr *const me);
void CommMgr_enReceivingAck(CommMgr *const me);
void CommMgr_enSendingEndOfHist(CommMgr *const me);
void CommMgr_enSendingHist(CommMgr *const me);

/* ......................... Declares exit actions ......................... */
void CommMgr_exWaitSync(CommMgr *const me);

/* ............................ Declares guards ............................ */
rbool_t CommMgr_isCondC0ToSendingHist11(CommMgr *const me, RKH_EVT_T *pe);
rbool_t CommMgr_isCondC1ToC215(CommMgr *const me, RKH_EVT_T *pe);
rbool_t CommMgr_isCondC2ToSendingStatus16(CommMgr *const me, RKH_EVT_T *pe);
rbool_t CommMgr_isCondC3ToSendingHist18(CommMgr *const me, RKH_EVT_T *pe);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
