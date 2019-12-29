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
void CommMgr_CurrentToWaitSyncExt5(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_CurrentToWaitSyncExt6(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_ReceivingStatusAckToC0Ext9(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_C0ToCurrentFinalExt12(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_C0ToC6Ext35(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_HistoryToWaitSyncExt13(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_HistoryToWaitSyncExt14(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_ToC1Ext16(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_SendingHistToC2Ext18(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_ReceivingMsgAckToC3Ext19(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_C2ToSendingHistExt31(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_C3ToHistoryFinalExt24(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_C3ToC4Ext25(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_C3ToC5Ext32(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_C4ToC1Ext27(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_ActiveToActiveLoc0(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_SendingStartOfHistToSendingHistExt30(CommMgr *const me,
                                                  RKH_EVT_T *pe);
void CommMgr_SendingEndOfHistToReceivingMsgAckExt34(CommMgr *const me,
                                                    RKH_EVT_T *pe);
void CommMgr_SendingStatusToReceivingiStatusAckExt37(CommMgr *const me,
                                                     RKH_EVT_T *pe);
void CommMgr_C5ToHistoryFinalExt38(CommMgr *const me, RKH_EVT_T *pe);
void CommMgr_C6ToCurrentFinalExt39(CommMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
void CommMgr_enWaitSync(CommMgr *const me);
void CommMgr_enSendingStatus(CommMgr *const me);
void CommMgr_enReceivingStatusAck(CommMgr *const me);
void CommMgr_enSendingEndOfHist(CommMgr *const me);
void CommMgr_enSendingHist(CommMgr *const me);
void CommMgr_enReceivingMsgAck(CommMgr *const me);
void CommMgr_enSendingStartOfHist(CommMgr *const me);

/* ......................... Declares exit actions ......................... */
void CommMgr_exWaitSync(CommMgr *const me);

/* ............................ Declares guards ............................ */
rbool_t CommMgr_isCondC0ToHistory11(CommMgr *const me, RKH_EVT_T *pe);
rbool_t CommMgr_isCondC0ToC628(CommMgr *const me, RKH_EVT_T *pe);
rbool_t CommMgr_isCondC1ToSendingHist20(CommMgr *const me, RKH_EVT_T *pe);
rbool_t CommMgr_isCondC2ToSendingEndOfHist23(CommMgr *const me, RKH_EVT_T *pe);
rbool_t CommMgr_isCondC3ToC425(CommMgr *const me, RKH_EVT_T *pe);
rbool_t CommMgr_isCondC3ToC529(CommMgr *const me, RKH_EVT_T *pe);
rbool_t CommMgr_isCondC4ToCurrent26(CommMgr *const me, RKH_EVT_T *pe);
rbool_t CommMgr_isCondC5ToHistoryFinal33(CommMgr *const me, RKH_EVT_T *pe);
rbool_t CommMgr_isCondC6ToCurrentFinal36(CommMgr *const me, RKH_EVT_T *pe);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
