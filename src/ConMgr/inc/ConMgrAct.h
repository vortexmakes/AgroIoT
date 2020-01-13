/**
 *  \file       ConMgr.h
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
#ifndef __CONMGRACT_H__
#define __CONMGRACT_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "ConMgr.h"

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
void ConMgr_ToConMgr_InactiveExt0(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_InactiveToConMgr_ActiveExt1(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_ActiveToConMgr_InactiveExt2(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ToConMgr_InitializeExt4(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_GetImeiToConMgr_CipShutdownExt18(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_RegisteredToConMgr_Registered_FinalExt25(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_C1ToConMgr_Registered_FinalExt28(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_LocalTimeToConMgr_ConfigureExt29(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_ConnectingToConMgr_DisconnectingExt33(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ToConMgr_WaitingServerExt34(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_WaitingServerToConMgr_WaitingServerExt35(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_ConnectedToConMgr_RestartingExt37(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_IdleToConMgr_GetStatusExt40(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_IdleToConMgr_SendingExt41(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_IdleToConMgr_receivingExt42(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_receivingToConMgr_IdleExt43(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_receivingToConMgr_IdleExt44(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_GetStatusToConMgr_IdleExt45(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_GetStatusToConMgr_IdleExt46(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_SendingToConMgr_IdleExt47(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_waitOkToConMgr_Sending_FinalExt50(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_WaitPromptToConMgr_waitOkExt51(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_WaitRetryConfigToConMgr_HConfigureExt54(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_WaitNetClockSyncToConMgr_LocalTimeExt55(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_ConfigureToConMgr_ConnectingExt58(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ToConMgr_GetOperExt59(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_CheckIPToConMgr_CheckIPExt64(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_GetOperToConMgr_SetAPNExt65(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_C2ToConMgr_Registered_FinalExt67(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_WaitRetryConnectToConMgr_ConnectingExt68(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_InactiveToConMgr_InactiveLoc0(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_InactiveToConMgr_InactiveLoc1(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_ActiveToConMgr_ActiveLoc2(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_ActiveToConMgr_ActiveLoc3(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_ActiveToConMgr_ActiveLoc4(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_UnregisteredToUnregisteredLoc16(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_UnregisteredToUnregisteredLoc17(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_GetStatusToConMgr_GetStatusLoc23(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ConMgr_GetStatusToConMgr_GetStatusLoc24(ConMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
void ConMgr_enConMgr_Sync(ConMgr *const me);
void ConMgr_enConMgr_Init(ConMgr *const me);
void ConMgr_enConMgr_Pin(ConMgr *const me);
void ConMgr_enConMgr_SetPin(ConMgr *const me);
void ConMgr_enConMgr_EnableNetTime(ConMgr *const me);
void ConMgr_enConMgr_GetImei(ConMgr *const me);
void ConMgr_enConMgr_CipShutdown(ConMgr *const me);
void ConMgr_enConMgr_SetManualGet(ConMgr *const me);
void ConMgr_enConFailure(ConMgr *const me);
void ConMgr_enUnregistered(ConMgr *const me);
void ConMgr_enConMgr_Registered(ConMgr *const me);
void ConMgr_enConMgr_WaitingServer(ConMgr *const me);
void ConMgr_enConMgr_Connected(ConMgr *const me);
void ConMgr_enConMgr_Idle(ConMgr *const me);
void ConMgr_enConMgr_WaitReopen(ConMgr *const me);
void ConMgr_enConMgr_WaitRetryConfig(ConMgr *const me);
void ConMgr_enConMgr_WaitNetClockSync(ConMgr *const me);
void ConMgr_enConMgr_SetAPN(ConMgr *const me);
void ConMgr_enConMgr_EnableNetwork(ConMgr *const me);
void ConMgr_enConMgr_CheckIP(ConMgr *const me);
void ConMgr_enConMgr_GetOper(ConMgr *const me);
void ConMgr_enConMgr_WaitRetryConnect(ConMgr *const me);

/* ......................... Declares exit actions ......................... */
void ConMgr_exConFailure(ConMgr *const me);
void ConMgr_exUnregistered(ConMgr *const me);
void ConMgr_exConMgr_Registered(ConMgr *const me);
void ConMgr_exConMgr_WaitingServer(ConMgr *const me);
void ConMgr_exConMgr_Connected(ConMgr *const me);
void ConMgr_exConMgr_Idle(ConMgr *const me);
void ConMgr_exConMgr_GetStatus(ConMgr *const me);
void ConMgr_exConMgr_WaitReopen(ConMgr *const me);
void ConMgr_exConMgr_WaitRetryConfig(ConMgr *const me);
void ConMgr_exConMgr_WaitNetClockSync(ConMgr *const me);
void ConMgr_exConMgr_WaitRetryConnect(ConMgr *const me);

/* ............................ Declares guards ............................ */
rbool_t ConMgr_isCondConMgr_GetStatusToConMgr_Idle45(ConMgr *const me, RKH_EVT_T *pe);
rbool_t ConMgr_isCondC0ToConMgr_Sync10(ConMgr *const me, RKH_EVT_T *pe);
rbool_t ConMgr_isCondC1ToConMgr_WaitRetryConnect27(ConMgr *const me, RKH_EVT_T *pe);
rbool_t ConMgr_isCondC2ToConMgr_WaitRetryConfig66(ConMgr *const me, RKH_EVT_T *pe);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
