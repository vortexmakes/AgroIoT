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
void ConMgr_ToinactiveExt0(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_inactiveToActiveExt1(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ActiveToinactiveExt2(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ToinitializeExt4(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_registeredToregistered_FinalExt25(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_C1Toregistered_FinalExt28(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_localTimeToconfigureExt29(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_connectingTodisconnectingExt33(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_TowaitingServerExt34(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_waitingServerTowaitingServerExt35(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_connectedTorestartingExt37(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_idleTogetStatusExt40(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_idleTosendingExt41(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_idleToreceivingExt42(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_receivingToidleExt43(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_receivingToidleExt44(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_getStatusToidleExt45(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_getStatusToidleExt46(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_sendingToidleExt47(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_waitOkTosending_FinalExt50(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_waitPromptTowaitOkExt51(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_waitRetryConfigToHConfigureExt54(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_waitNetClockSyncTolocalTimeExt55(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_configureToconnectingExt58(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_TogetOperExt59(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_checkIPTocheckIPExt64(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_getOperTosetAPNExt65(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_C2Toregistered_FinalExt67(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_waitRetryConnectToconnectingExt68(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_inactiveToinactiveLoc0(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_inactiveToinactiveLoc1(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ActiveToActiveLoc2(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ActiveToActiveLoc3(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_ActiveToActiveLoc4(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_unregisteredTounregisteredLoc16(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_unregisteredTounregisteredLoc17(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_getStatusTogetStatusLoc22(ConMgr *const me, RKH_EVT_T *pe);
void ConMgr_getStatusTogetStatusLoc22(ConMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
void ConMgr_ensync(ConMgr *const me);
void ConMgr_eninit(ConMgr *const me);
void ConMgr_enpin(ConMgr *const me);
void ConMgr_ensetPin(ConMgr *const me);
void ConMgr_enenableNetTime(ConMgr *const me);
void ConMgr_engetImei(ConMgr *const me);
void ConMgr_encipShutdown(ConMgr *const me);
void ConMgr_ensetManualGet(ConMgr *const me);
void ConMgr_enfailure(ConMgr *const me);
void ConMgr_enunregistered(ConMgr *const me);
void ConMgr_enregistered(ConMgr *const me);
void ConMgr_enwaitingServer(ConMgr *const me);
void ConMgr_enconnected(ConMgr *const me);
void ConMgr_enidle(ConMgr *const me);
void ConMgr_enwReopen(ConMgr *const me);
void ConMgr_enwaitRetryConfig(ConMgr *const me);
void ConMgr_enwaitNetClockSync(ConMgr *const me);
void ConMgr_ensetAPN(ConMgr *const me);
void ConMgr_enenableNetwork(ConMgr *const me);
void ConMgr_encheckIP(ConMgr *const me);
void ConMgr_engetOper(ConMgr *const me);
void ConMgr_enwaitRetryConnect(ConMgr *const me);

/* ......................... Declares exit actions ......................... */
void ConMgr_exfailure(ConMgr *const me);
void ConMgr_exunregistered(ConMgr *const me);
void ConMgr_exregistered(ConMgr *const me);
void ConMgr_exwaitingServer(ConMgr *const me);
void ConMgr_exconnected(ConMgr *const me);
void ConMgr_exidle(ConMgr *const me);
void ConMgr_exwReopen(ConMgr *const me);
void ConMgr_exwaitRetryConfig(ConMgr *const me);
void ConMgr_exwaitNetClockSync(ConMgr *const me);
void ConMgr_exwaitRetryConnect(ConMgr *const me);

/* ............................ Declares guards ............................ */
rbool_t ConMgr_isCondgetStatusToidle45(ConMgr *const me, RKH_EVT_T *pe);
rbool_t ConMgr_isCondC0Tosync10(ConMgr *const me, RKH_EVT_T *pe);
rbool_t ConMgr_isCondC1TowaitRetryConnect27(ConMgr *const me, RKH_EVT_T *pe);
rbool_t ConMgr_isCondC2TowaitRetryConfig66(ConMgr *const me, RKH_EVT_T *pe);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
