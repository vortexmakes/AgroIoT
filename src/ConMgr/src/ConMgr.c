/**
 *  \file       ConMgr.c
 *  \brief      Active object implementation.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "signals.h"
#include "rkhtmr.h"
#include "ConMgr.h"
#include "bsp.h"
#include "ConMgrRequired.h"

/* ----------------------------- Local macros ------------------------------ */
#define WaitTime0	RKH_TIME_SEC(5)
#define WaitTime1	RKH_TIME_SEC(60)
#define WaitTime2	RKH_TIME_SEC(2)
#define WaitTime3	RKH_TIME_SEC(2)
#define WaitTime4	RKH_TIME_SEC(2)
#define WaitTime5	RKH_TIME_SEC(2)
#define WaitTime6	RKH_TIME_SEC(5)
#define WaitTime7	RKH_TIME_SEC(2)

/* ......................... Declares active object ........................ */
typedef struct ConMgr ConMgr;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE ConMgr_Inactive, ConMgr_Sync, ConMgr_Init, ConMgr_Pin, ConMgr_SetPin, ConMgr_EnableNetTime, ConMgr_GetImei, ConMgr_SetManualGet, ConMgr_CipShutdown, ConMgr_ConFailure, ConMgr_Unregistered, ConMgr_LocalTime, ConMgr_WaitingServer, ConMgr_Idle, ConMgr_receiving, ConMgr_GetStatus, ConMgr_waitOk, ConMgr_WaitPrompt, ConMgr_Restarting, ConMgr_WaitReopen, ConMgr_WaitRetryConfig, ConMgr_WaitNetClockSync, ConMgr_SetAPN, ConMgr_EnableNetwork, ConMgr_CheckIP, ConMgr_GetOper, ConMgr_WaitRetryConnect, ConMgr_Disconnecting;
RKH_DCLR_COMP_STATE ConMgr_Active, ConMgr_Initialize, ConMgr_Registered, ConMgr_Connecting, ConMgr_Connected, ConMgr_Sending, ConMgr_Configure;
RKH_DCLR_CHOICE_STATE ConMgr_C0, ConMgr_C1, ConMgr_C2;
RKH_DCLR_FINAL_STATE ConMgr_InitializeFinal, ConMgr_SendingFinal, ConMgr_ConfigureFinal, ConMgr_RegisteredFinal, ConMgr_ActiveFinal;
RKH_DCLR_SHIST_STATE ConMgr_ConfigureHist;

/* ........................ Declares effect actions ........................ */
void ToInactiveExt0(ConMgr *const me, RKH_EVT_T *pe);
void InactiveToActiveExt1(ConMgr *const me, RKH_EVT_T *pe);
void ActiveToInactiveExt2(ConMgr *const me, RKH_EVT_T *pe);
void ToInitializeExt4(ConMgr *const me, RKH_EVT_T *pe);
void GetImeiToCipShutdownExt18(ConMgr *const me, RKH_EVT_T *pe);
void RegisteredToRegisteredFinalExt25(ConMgr *const me, RKH_EVT_T *pe);
void C1ToRegisteredFinalExt28(ConMgr *const me, RKH_EVT_T *pe);
void LocalTimeToConfigureExt29(ConMgr *const me, RKH_EVT_T *pe);
void ConnectingToDisconnectingExt33(ConMgr *const me, RKH_EVT_T *pe);
void ToWaitingServerExt34(ConMgr *const me, RKH_EVT_T *pe);
void WaitingServerToWaitingServerExt35(ConMgr *const me, RKH_EVT_T *pe);
void ConnectedToRestartingExt37(ConMgr *const me, RKH_EVT_T *pe);
void IdleToGetStatusExt40(ConMgr *const me, RKH_EVT_T *pe);
void IdleToSendingExt41(ConMgr *const me, RKH_EVT_T *pe);
void IdleToreceivingExt42(ConMgr *const me, RKH_EVT_T *pe);
void receivingToIdleExt43(ConMgr *const me, RKH_EVT_T *pe);
void receivingToIdleExt44(ConMgr *const me, RKH_EVT_T *pe);
void GetStatusToIdleExt45(ConMgr *const me, RKH_EVT_T *pe);
void GetStatusToIdleExt46(ConMgr *const me, RKH_EVT_T *pe);
void SendingToIdleExt47(ConMgr *const me, RKH_EVT_T *pe);
void waitOkToSendingFinalExt50(ConMgr *const me, RKH_EVT_T *pe);
void WaitPromptTowaitOkExt51(ConMgr *const me, RKH_EVT_T *pe);
void WaitRetryConfigToConfigureHistExt54(ConMgr *const me, RKH_EVT_T *pe);
void WaitNetClockSyncToLocalTimeExt55(ConMgr *const me, RKH_EVT_T *pe);
void ConfigureToConnectingExt58(ConMgr *const me, RKH_EVT_T *pe);
void ToGetOperExt59(ConMgr *const me, RKH_EVT_T *pe);
void CheckIPToCheckIPExt64(ConMgr *const me, RKH_EVT_T *pe);
void GetOperToSetAPNExt65(ConMgr *const me, RKH_EVT_T *pe);
void C2ToRegisteredFinalExt67(ConMgr *const me, RKH_EVT_T *pe);
void WaitRetryConnectToConnectingExt68(ConMgr *const me, RKH_EVT_T *pe);
void InactiveToInactiveLoc0(ConMgr *const me, RKH_EVT_T *pe);
void InactiveToInactiveLoc1(ConMgr *const me, RKH_EVT_T *pe);
void ActiveToActiveLoc2(ConMgr *const me, RKH_EVT_T *pe);
void ActiveToActiveLoc3(ConMgr *const me, RKH_EVT_T *pe);
void ActiveToActiveLoc4(ConMgr *const me, RKH_EVT_T *pe);
void UnregisteredToUnregisteredLoc16(ConMgr *const me, RKH_EVT_T *pe);
void UnregisteredToUnregisteredLoc17(ConMgr *const me, RKH_EVT_T *pe);
void GetStatusToGetStatusLoc23(ConMgr *const me, RKH_EVT_T *pe);
void GetStatusToGetStatusLoc24(ConMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
void enSync(ConMgr *const me);
void enInit(ConMgr *const me);
void enPin(ConMgr *const me);
void enSetPin(ConMgr *const me);
void enEnableNetTime(ConMgr *const me);
void enGetImei(ConMgr *const me);
void enSetManualGet(ConMgr *const me);
void enCipShutdown(ConMgr *const me);
void enConFailure(ConMgr *const me);
void enUnregistered(ConMgr *const me);
void enRegistered(ConMgr *const me);
void enWaitingServer(ConMgr *const me);
void enConnected(ConMgr *const me);
void enIdle(ConMgr *const me);
void enWaitReopen(ConMgr *const me);
void enWaitRetryConfig(ConMgr *const me);
void enWaitNetClockSync(ConMgr *const me);
void enSetAPN(ConMgr *const me);
void enEnableNetwork(ConMgr *const me);
void enCheckIP(ConMgr *const me);
void enGetOper(ConMgr *const me);
void enWaitRetryConnect(ConMgr *const me);

/* ......................... Declares exit actions ......................... */
void exConFailure(ConMgr *const me);
void exUnregistered(ConMgr *const me);
void exRegistered(ConMgr *const me);
void exWaitingServer(ConMgr *const me);
void exConnected(ConMgr *const me);
void exIdle(ConMgr *const me);
void exGetStatus(ConMgr *const me);
void exWaitReopen(ConMgr *const me);
void exWaitRetryConfig(ConMgr *const me);
void exWaitNetClockSync(ConMgr *const me);
void exWaitRetryConnect(ConMgr *const me);

/* ............................ Declares guards ............................ */
rbool_t isCondGetStatusToIdle45(ConMgr *const me, RKH_EVT_T *pe);
rbool_t isCondC0ToSync10(ConMgr *const me, RKH_EVT_T *pe);
rbool_t isCondC1ToWaitRetryConnect27(ConMgr *const me, RKH_EVT_T *pe);
rbool_t isCondC2ToWaitRetryConfig66(ConMgr *const me, RKH_EVT_T *pe);

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(ConMgr_Inactive, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_Sync, enSync, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_Init, enInit, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_Pin, enPin, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_SetPin, enSetPin, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_EnableNetTime, enEnableNetTime, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_GetImei, enGetImei, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_SetManualGet, enSetManualGet, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_CipShutdown, enCipShutdown, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_ConFailure, enConFailure, exConFailure, &ConMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_Unregistered, enUnregistered, exUnregistered, &ConMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_LocalTime, NULL, NULL, &ConMgr_Registered, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_WaitingServer, enWaitingServer, exWaitingServer, &ConMgr_Connecting, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_Idle, enIdle, exIdle, &ConMgr_Connected, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_receiving, NULL, NULL, &ConMgr_Connected, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_GetStatus, NULL, exGetStatus, &ConMgr_Connected, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_waitOk, NULL, NULL, &ConMgr_Sending, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_WaitPrompt, NULL, NULL, &ConMgr_Sending, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_Restarting, NULL, NULL, &ConMgr_Connecting, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_WaitReopen, enWaitReopen, exWaitReopen, &ConMgr_Connecting, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_WaitRetryConfig, enWaitRetryConfig, exWaitRetryConfig, &ConMgr_Registered, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_WaitNetClockSync, enWaitNetClockSync, exWaitNetClockSync, &ConMgr_Registered, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_SetAPN, enSetAPN, NULL, &ConMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_EnableNetwork, enEnableNetwork, NULL, &ConMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_CheckIP, enCheckIP, NULL, &ConMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_GetOper, enGetOper, NULL, &ConMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_WaitRetryConnect, enWaitRetryConnect, exWaitRetryConnect, &ConMgr_Registered, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_Disconnecting, NULL, NULL, &ConMgr_Registered, NULL);

RKH_CREATE_COMP_REGION_STATE(ConMgr_Active, NULL, NULL, RKH_ROOT, &ConMgr_Initialize, ToInitializeExt4, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(ConMgr_Initialize, NULL, NULL, &ConMgr_Active, &ConMgr_Sync, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(ConMgr_Registered, enRegistered, exRegistered, &ConMgr_Active, &ConMgr_WaitNetClockSync, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(ConMgr_Connecting, NULL, NULL, &ConMgr_Registered, &ConMgr_WaitingServer, ToWaitingServerExt34, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(ConMgr_Connected, enConnected, exConnected, &ConMgr_Connecting, &ConMgr_Idle, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(ConMgr_Sending, NULL, NULL, &ConMgr_Connected, &ConMgr_WaitPrompt, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_HISTORY_STORAGE(ConMgr_Configure);
RKH_CREATE_COMP_REGION_STATE(ConMgr_Configure, NULL, NULL, &ConMgr_Registered, &ConMgr_GetOper, ToGetOperExt59, RKH_SHISTORY, NULL, NULL, NULL, RKH_GET_HISTORY_STORAGE(ConMgr_Configure));

RKH_CREATE_TRANS_TABLE(ConMgr_Inactive)
    RKH_TRREG(evOpen, NULL, InactiveToActiveExt1, &ConMgr_Active),
    RKH_TRINT(evRecv, NULL, InactiveToInactiveLoc0),
    RKH_TRINT(evSend, NULL, InactiveToInactiveLoc1),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Active)
    RKH_TRREG(evClose, NULL, ActiveToInactiveExt2, &ConMgr_Inactive),
    RKH_TRCOMPLETION(NULL, NULL, &ConMgr_Inactive),
    RKH_TRINT(evSend, NULL, ActiveToActiveLoc2),
    RKH_TRINT(evRecv, NULL, ActiveToActiveLoc3),
    RKH_TRINT(evSigLevel, NULL, ActiveToActiveLoc4),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Initialize)
    RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_ConFailure),
    RKH_TRREG(evError, NULL, NULL, &ConMgr_ConFailure),
    RKH_TRCOMPLETION(NULL, NULL, &ConMgr_ConFailure),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Sync)
    RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_C0),
    RKH_TRREG(evError, NULL, NULL, &ConMgr_C0),
    RKH_TRREG(evOk, NULL, NULL, &ConMgr_Init),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Init)
    RKH_TRREG(evOk, NULL, NULL, &ConMgr_Pin),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Pin)
    RKH_TRREG(evSimError, NULL, NULL, &ConMgr_InitializeFinal),
    RKH_TRREG(evSimPin, NULL, NULL, &ConMgr_SetPin),
    RKH_TRREG(evSimReady, NULL, NULL, &ConMgr_EnableNetTime),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_SetPin)
    RKH_TRREG(evOk, NULL, NULL, &ConMgr_Pin),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_EnableNetTime)
    RKH_TRREG(evOk, NULL, NULL, &ConMgr_GetImei),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_GetImei)
    RKH_TRREG(evImei, NULL, GetImeiToCipShutdownExt18, &ConMgr_CipShutdown),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_SetManualGet)
    RKH_TRREG(evOk, NULL, NULL, &ConMgr_Unregistered),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_CipShutdown)
    RKH_TRREG(evOk, NULL, NULL, &ConMgr_SetManualGet),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_ConFailure)
    RKH_TRREG(evTout0, NULL, NULL, &ConMgr_Active),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Unregistered)
    RKH_TRREG(evTout1, NULL, NULL, &ConMgr_ConFailure),
    RKH_TRREG(evReg, NULL, NULL, &ConMgr_Registered),
    RKH_TRINT(evRegStatusTout, NULL, UnregisteredToUnregisteredLoc16),
    RKH_TRINT(evNoReg, NULL, UnregisteredToUnregisteredLoc17),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Registered)
    RKH_TRREG(evNoReg, NULL, NULL, &ConMgr_Unregistered),
    RKH_TRREG(evError, NULL, RegisteredToRegisteredFinalExt25, &ConMgr_RegisteredFinal),
    RKH_TRCOMPLETION(NULL, NULL, &ConMgr_ConFailure),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_LocalTime)
    RKH_TRREG(evLocalTime, NULL, LocalTimeToConfigureExt29, &ConMgr_Configure),
    RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_Configure),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Connecting)
    RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_C1),
    RKH_TRREG(evClosed, NULL, NULL, &ConMgr_C1),
    RKH_TRREG(evIPStatus, NULL, NULL, &ConMgr_C1),
    RKH_TRREG(evError, NULL, NULL, &ConMgr_C1),
    RKH_TRREG(evIPStart, NULL, NULL, &ConMgr_Configure),
    RKH_TRREG(evIPInitial, NULL, NULL, &ConMgr_Configure),
    RKH_TRREG(evIPGprsAct, NULL, NULL, &ConMgr_Configure),
    RKH_TRREG(evClose, NULL, ConnectingToDisconnectingExt33, &ConMgr_Disconnecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_WaitingServer)
    RKH_TRREG(evTout2, NULL, WaitingServerToWaitingServerExt35, &ConMgr_WaitingServer),
    RKH_TRREG(evConnected, NULL, NULL, &ConMgr_Connected),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Connected)
    RKH_TRREG(evRestart, NULL, ConnectedToRestartingExt37, &ConMgr_Restarting),
    RKH_TRREG(evClosed, NULL, NULL, &ConMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Idle)
    RKH_TRREG(evTout3, NULL, IdleToGetStatusExt40, &ConMgr_GetStatus),
    RKH_TRREG(evSend, NULL, IdleToSendingExt41, &ConMgr_Sending),
    RKH_TRREG(evRecv, NULL, IdleToreceivingExt42, &ConMgr_receiving),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_receiving)
    RKH_TRREG(evNoResponse, NULL, receivingToIdleExt43, &ConMgr_Idle),
    RKH_TRREG(evError, NULL, receivingToIdleExt43, &ConMgr_Idle),
    RKH_TRREG(evReceived, NULL, receivingToIdleExt44, &ConMgr_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_GetStatus)
    RKH_TRREG(evNoResponse, isCondGetStatusToIdle45, GetStatusToIdleExt45, &ConMgr_Idle),
    RKH_TRREG(evConnected, NULL, GetStatusToIdleExt46, &ConMgr_Idle),
    RKH_TRINT(evSend, NULL, GetStatusToGetStatusLoc23),
    RKH_TRINT(evRecv, NULL, GetStatusToGetStatusLoc24),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Sending)
    RKH_TRREG(evNoResponse, NULL, SendingToIdleExt47, &ConMgr_Idle),
    RKH_TRREG(evError, NULL, SendingToIdleExt47, &ConMgr_Idle),
    RKH_TRCOMPLETION(NULL, NULL, &ConMgr_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_waitOk)
    RKH_TRREG(evOk, NULL, waitOkToSendingFinalExt50, &ConMgr_SendingFinal),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_WaitPrompt)
    RKH_TRREG(evOk, NULL, WaitPromptTowaitOkExt51, &ConMgr_waitOk),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Restarting)
    RKH_TRREG(evDisconnected, NULL, NULL, &ConMgr_WaitReopen),
    RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_WaitReopen),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_WaitReopen)
    RKH_TRREG(evTout4, NULL, NULL, &ConMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_WaitRetryConfig)
    RKH_TRREG(evTout5, NULL, WaitRetryConfigToConfigureHistExt54, &ConMgr_ConfigureHist),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_WaitNetClockSync)
    RKH_TRREG(evNetClockSync, NULL, WaitNetClockSyncToLocalTimeExt55, &ConMgr_LocalTime),
    RKH_TRREG(evTout6, NULL, NULL, &ConMgr_Configure),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Configure)
    RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_C2),
    RKH_TRCOMPLETION(NULL, ConfigureToConnectingExt58, &ConMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_SetAPN)
    RKH_TRREG(evOk, NULL, NULL, &ConMgr_EnableNetwork),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_EnableNetwork)
    RKH_TRREG(evOk, NULL, NULL, &ConMgr_CheckIP),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_CheckIP)
    RKH_TRREG(evIPStatus, NULL, NULL, &ConMgr_ConfigureFinal),
    RKH_TRREG(evIP, NULL, NULL, &ConMgr_CheckIP),
    RKH_TRREG(evIPInitial, NULL, NULL, &ConMgr_CheckIP),
    RKH_TRREG(evIPStart, NULL, NULL, &ConMgr_CheckIP),
    RKH_TRREG(evIPGprsAct, NULL, CheckIPToCheckIPExt64, &ConMgr_CheckIP),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_GetOper)
    RKH_TRREG(evOper, NULL, GetOperToSetAPNExt65, &ConMgr_SetAPN),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_WaitRetryConnect)
    RKH_TRREG(evTout7, NULL, WaitRetryConnectToConnectingExt68, &ConMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Disconnecting)
    RKH_TRREG(evDisconnected, NULL, NULL, &ConMgr_ActiveFinal),
    RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_ActiveFinal),
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(ConMgr_C0);
RKH_CREATE_CHOICE_STATE(ConMgr_C1);
RKH_CREATE_CHOICE_STATE(ConMgr_C2);

RKH_CREATE_BRANCH_TABLE(ConMgr_C0)
    RKH_BRANCH(isCondC0ToSync10, NULL, &ConMgr_Sync),
    RKH_BRANCH(ELSE, NULL, &ConMgr_InitializeFinal),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(ConMgr_C1)
    RKH_BRANCH(isCondC1ToWaitRetryConnect27, NULL, &ConMgr_WaitRetryConnect),
    RKH_BRANCH(ELSE, C1ToRegisteredFinalExt28, &ConMgr_RegisteredFinal),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(ConMgr_C2)
    RKH_BRANCH(isCondC2ToWaitRetryConfig66, NULL, &ConMgr_WaitRetryConfig),
    RKH_BRANCH(ELSE, C2ToRegisteredFinalExt67, &ConMgr_RegisteredFinal),
RKH_END_BRANCH_TABLE


/* ............................. Active object ............................. */
struct ConMgr
{
    RKH_SMA_T sma;      /* base structure */
    RKHTmEvt tmEvtObj0;
    RKHTmEvt tmEvtObj1;
    RKHTmEvt tmEvtObj2;
    RKHTmEvt tmEvtObj3;
    RKHTmEvt tmEvtObj4;
    RKHTmEvt tmEvtObj5;
    RKHTmEvt tmEvtObj6;
    RKHTmEvt tmEvtObj7;
    rInt sigLevel;
    char * imei;
    char * oper;
    char * protocol;
    char * domain;
    char * port;
    rInt retryCount;
};

RKH_SMA_CREATE(ConMgr, conMgr, 1, HCAL, &ConMgr_Inactive, ToInactiveExt0, NULL);
RKH_SMA_DEF_PTR(conMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct ConMgrInternal
{
    ConMgr *pConMgr;
    
    char Imei[IMEI_BUF_SIZE];
    char Oper[OPER_BUF_SIZE];
    char Domain[DOMAIN_BUF_SIZE];
    char Port[PORT_BUF_SIZE];

    RKH_QUEUE_T qDefer;
    RKH_EVT_T *qDefer_sto[SIZEOF_QDEFER];

    SendEvt *psend;
    RKHTmEvt tmEvtRegStatus;
} ConMgrInternal;

typedef struct Apn
{
    char *addr;
    char *usr;
    char *psw;
}Apn;

typedef struct Operator
{
    char *netCode;
    Apn apn;
}Operator;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_ROM_STATIC_EVENT(e_NetConnected, evNetConnected);
static RKH_ROM_STATIC_EVENT(e_NetDisconnected, evNetDisconnected);
static RKH_ROM_STATIC_EVENT(e_Sent,     evSent);
static RKH_ROM_STATIC_EVENT(e_SendFail, evSendFail);
static RKH_ROM_STATIC_EVENT(e_RecvFail, evRecvFail);

static Operator operTable[] =
{
    {MOVISTAR_OPERATOR,
     {MOVISTAR_APN_ADDR, MOVISTAR_APN_USER, MOVISTAR_APN_PASS}},
    {CLARO_OPERATOR,
     {CLARO_APN_ADDR, CLARO_APN_USER, CLARO_APN_PASS}},
    {PERSONAL_OPERATOR,
     {PERSONAL_APN_ADDR, PERSONAL_APN_USER, PERSONAL_APN_PASS}},
    {NULL}
};

static Apn *defaultAPN = &(operTable[0].apn);


static ConMgrInternal ConMgrInt;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static Apn *
getAPNbyOper(char *oper)
{
    Operator *pOper;

    for (pOper = &operTable[0]; pOper->netCode != NULL; ++pOper)
    {
        if (strcmp(oper, pOper->netCode) == 0)
        {
            return &(pOper->apn);
        }
    }
    return defaultAPN;
}

static void
init(ConMgr *const me)
{
    topic_subscribe(TCPConnection, me);
    topic_subscribe(ModURC, me);

    rkh_queue_init(&ConMgrInt.qDefer, (const void **)ConMgrInt.qDefer_sto, 
                    SIZEOF_QDEFER, CV(0));

    ConMgrInt.pConMgr = me;

    me->imei = ConMgrInt.Imei;
    me->oper = ConMgrInt.Oper;
    me->protocol = CONNECTION_PROT;
    me->domain = ConMgrInt.Domain;
    me->port = ConMgrInt.Port;


    bsp_SIMSelect(MainSIM);
}

static void
storeImei(ConMgr *const me, RKH_EVT_T *pe)
{
    ImeiEvt *p;

    (void)me;

    p = RKH_UPCAST(ImeiEvt, pe);
    strcpy(me->imei, p->buf);
}

static void
storeOper(ConMgr *const me, RKH_EVT_T *pe)
{
    OperEvt *p;

    p = RKH_UPCAST(OperEvt, pe);
    strcpy(me->oper, p->buf);
}

static void
setupAPN(ConMgr *const me)
{
    Apn *apn;

    apn = getAPNbyOper(me->oper);
    ModCmd_setupAPN(apn->addr, apn->usr, apn->psw);
}

static void
startRegStatusTimer(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&ConMgrInt.tmEvtRegStatus, evRegStatusTout);
	RKH_TMR_INIT(&ConMgrInt.tmEvtRegStatus.tmr, 
                    RKH_UPCAST(RKH_EVT_T, &ConMgrInt.tmEvtRegStatus), NULL);
	RKH_TMR_ONESHOT(&ConMgrInt.tmEvtRegStatus.tmr, RKH_UPCAST(RKH_SMA_T, me), 
                        CHECK_REG_STATUS_PERIOD);
}

static void
sendRequest(RKH_EVT_T *pe)
{
    ConMgrInt.psend = RKH_UPCAST(SendEvt, pe);

    ModCmd_sendDataRequest((rui16_t)(ConMgrInt.psend->size));
}

static void
sendOk(ConMgr *const me)
{
    topic_publish(TCPConnection, &e_Sent, me);
}

static void
sendFail(ConMgr *const me)
{
    topic_publish(TCPConnection, &e_SendFail, me);

    ModCmd_init();
}

static void
flushData(void)
{
    ModCmd_sendData(ConMgrInt.psend->buf, ConMgrInt.psend->size);
}

static void
readData(void)
{
    ModCmd_readData();
}

static void
recvOk(ConMgr *const me, RKH_EVT_T *pe)
{
    topic_publish(TCPConnection, pe, me);

    bsp_recvOk();
}

static void
recvFail(ConMgr *const me)
{
    topic_publish(TCPConnection, &e_RecvFail, me);

    ModCmd_init();
}

static void
setSigLevel(ConMgr *const me, RKH_EVT_T *pe)
{
    SigLevelEvt *p;

    p = RKH_UPCAST(SigLevelEvt, pe);
    me->sigLevel = p->value;
}

static void
rtimeSync(RKH_EVT_T *pe)
{
    rtime_set(&(RKH_UPCAST(LocalTimeEvt, pe)->time));
}

static void
reqDefer(RKH_EVT_T *pe)
{
    if (rkh_queue_is_full(&ConMgrInt.qDefer) != RKH_TRUE)
    {
        rkh_sma_defer(&ConMgrInt.qDefer, pe);
    }
}

static void
reqRecall(ConMgr *const me)
{
    rkh_sma_recall((RKH_SMA_T *)me, &ConMgrInt.qDefer);
}

static void
socketConnected(ConMgr *const me)
{
    topic_publish(TCPConnection, &e_NetConnected, me);

    bsp_netStatus(ConnectedSt);
}

static void
socketDisconnected(ConMgr *const me)
{
    topic_publish(TCPConnection, &e_NetDisconnected, me);

    bsp_netStatus(DisconnectedSt);
}

/* ............................ Effect actions ............................. */
void 
ToInactiveExt0(ConMgr *const me, RKH_EVT_T *pe)
{
    me->sigLevel = 0;
    me->imei = "";
    me->oper = "";
    me->protocol = "";
    me->domain = "";
    me->port = "";
    me->retryCount = 0;
    	
    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &ConMgr_Inactive);
    RKH_TR_FWK_STATE(me, &ConMgr_Active);
    RKH_TR_FWK_STATE(me, &ConMgr_Initialize);
    RKH_TR_FWK_STATE(me, &ConMgr_Sync);
    RKH_TR_FWK_STATE(me, &ConMgr_Init);
    RKH_TR_FWK_STATE(me, &ConMgr_Pin);
    RKH_TR_FWK_STATE(me, &ConMgr_SetPin);
    RKH_TR_FWK_STATE(me, &ConMgr_EnableNetTime);
    RKH_TR_FWK_STATE(me, &ConMgr_GetImei);
    RKH_TR_FWK_STATE(me, &ConMgr_SetManualGet);
    RKH_TR_FWK_STATE(me, &ConMgr_CipShutdown);
    RKH_TR_FWK_STATE(me, &ConMgr_ConFailure);
    RKH_TR_FWK_STATE(me, &ConMgr_Unregistered);
    RKH_TR_FWK_STATE(me, &ConMgr_Registered);
    RKH_TR_FWK_STATE(me, &ConMgr_LocalTime);
    RKH_TR_FWK_STATE(me, &ConMgr_Connecting);
    RKH_TR_FWK_STATE(me, &ConMgr_WaitingServer);
    RKH_TR_FWK_STATE(me, &ConMgr_Connected);
    RKH_TR_FWK_STATE(me, &ConMgr_Idle);
    RKH_TR_FWK_STATE(me, &ConMgr_receiving);
    RKH_TR_FWK_STATE(me, &ConMgr_GetStatus);
    RKH_TR_FWK_STATE(me, &ConMgr_Sending);
    RKH_TR_FWK_STATE(me, &ConMgr_waitOk);
    RKH_TR_FWK_STATE(me, &ConMgr_WaitPrompt);
    RKH_TR_FWK_STATE(me, &ConMgr_Restarting);
    RKH_TR_FWK_STATE(me, &ConMgr_WaitReopen);
    RKH_TR_FWK_STATE(me, &ConMgr_WaitRetryConfig);
    RKH_TR_FWK_STATE(me, &ConMgr_WaitNetClockSync);
    RKH_TR_FWK_STATE(me, &ConMgr_Configure);
    RKH_TR_FWK_STATE(me, &ConMgr_SetAPN);
    RKH_TR_FWK_STATE(me, &ConMgr_EnableNetwork);
    RKH_TR_FWK_STATE(me, &ConMgr_CheckIP);
    RKH_TR_FWK_STATE(me, &ConMgr_GetOper);
    RKH_TR_FWK_STATE(me, &ConMgr_WaitRetryConnect);
    RKH_TR_FWK_STATE(me, &ConMgr_Disconnecting);
    RKH_TR_FWK_PSTATE(me, &ConMgr_ConfigureHist);
    RKH_TR_FWK_PSTATE(me, &ConMgr_C0);
    RKH_TR_FWK_PSTATE(me, &ConMgr_C1);
    RKH_TR_FWK_PSTATE(me, &ConMgr_C2);
    RKH_TR_FWK_STATE(me, &ConMgr_InitializeFinal);
    RKH_TR_FWK_STATE(me, &ConMgr_SendingFinal);
    RKH_TR_FWK_STATE(me, &ConMgr_ConfigureFinal);
    RKH_TR_FWK_STATE(me, &ConMgr_RegisteredFinal);
    RKH_TR_FWK_STATE(me, &ConMgr_ActiveFinal);
    RKH_TR_FWK_SIG(evOpen);
    RKH_TR_FWK_SIG(evClose);
    RKH_TR_FWK_SIG(evSend);
    RKH_TR_FWK_SIG(evRecv);
    RKH_TR_FWK_SIG(evSendFail);
    RKH_TR_FWK_SIG(evRecvFail);
    RKH_TR_FWK_SIG(evSigLevel);
    RKH_TR_FWK_SIG(evOk);
    RKH_TR_FWK_SIG(evNoResponse);
    RKH_TR_FWK_SIG(evSimReady);
    RKH_TR_FWK_SIG(evSimPin);
    RKH_TR_FWK_SIG(evSimError);
    RKH_TR_FWK_SIG(evImei);
    RKH_TR_FWK_SIG(evReg);
    RKH_TR_FWK_SIG(evRegStatusTout);
    RKH_TR_FWK_SIG(evNoReg);
    RKH_TR_FWK_SIG(evNetClockSync);
    RKH_TR_FWK_SIG(evLocalTime);
    RKH_TR_FWK_SIG(evOper);
    RKH_TR_FWK_SIG(evIPStatus);
    RKH_TR_FWK_SIG(evIPInitial);
    RKH_TR_FWK_SIG(evIPStart);
    RKH_TR_FWK_SIG(evIPGprsAct);
    RKH_TR_FWK_SIG(evIP);
    RKH_TR_FWK_SIG(evConnected);
    RKH_TR_FWK_SIG(evDisconnected);
    RKH_TR_FWK_SIG(evRestart);
    RKH_TR_FWK_SIG(evClosed);
    RKH_TR_FWK_SIG(evError);
    RKH_TR_FWK_SIG(evStopSMS);
    RKH_TR_FWK_TIMER(&me->tmEvtObj0.tmr);
    RKH_TR_FWK_TIMER(&me->tmEvtObj1.tmr);
    RKH_TR_FWK_TIMER(&me->tmEvtObj2.tmr);
    RKH_TR_FWK_TIMER(&me->tmEvtObj3.tmr);
    RKH_TR_FWK_TIMER(&me->tmEvtObj4.tmr);
    RKH_TR_FWK_TIMER(&me->tmEvtObj5.tmr);
    RKH_TR_FWK_TIMER(&me->tmEvtObj6.tmr);
    RKH_TR_FWK_TIMER(&me->tmEvtObj7.tmr);
    #if 0
        RKH_TR_FWK_OBJ_NAME(ToInactiveExt0, "ToInactiveExt0");
        RKH_TR_FWK_OBJ_NAME(InactiveToActiveExt1, "InactiveToActiveExt1");
        RKH_TR_FWK_OBJ_NAME(ActiveToInactiveExt2, "ActiveToInactiveExt2");
        RKH_TR_FWK_OBJ_NAME(ToInitializeExt4, "ToInitializeExt4");
        RKH_TR_FWK_OBJ_NAME(GetImeiToCipShutdownExt18, "GetImeiToCipShutdownExt18");
        RKH_TR_FWK_OBJ_NAME(RegisteredToRegisteredFinalExt25, "RegisteredToRegisteredFinalExt25");
        RKH_TR_FWK_OBJ_NAME(C1ToRegisteredFinalExt28, "C1ToRegisteredFinalExt28");
        RKH_TR_FWK_OBJ_NAME(LocalTimeToConfigureExt29, "LocalTimeToConfigureExt29");
        RKH_TR_FWK_OBJ_NAME(ConnectingToDisconnectingExt33, "ConnectingToDisconnectingExt33");
        RKH_TR_FWK_OBJ_NAME(ToWaitingServerExt34, "ToWaitingServerExt34");
        RKH_TR_FWK_OBJ_NAME(WaitingServerToWaitingServerExt35, "WaitingServerToWaitingServerExt35");
        RKH_TR_FWK_OBJ_NAME(ConnectedToRestartingExt37, "ConnectedToRestartingExt37");
        RKH_TR_FWK_OBJ_NAME(IdleToGetStatusExt40, "IdleToGetStatusExt40");
        RKH_TR_FWK_OBJ_NAME(IdleToSendingExt41, "IdleToSendingExt41");
        RKH_TR_FWK_OBJ_NAME(IdleToreceivingExt42, "IdleToreceivingExt42");
        RKH_TR_FWK_OBJ_NAME(receivingToIdleExt43, "receivingToIdleExt43");
        RKH_TR_FWK_OBJ_NAME(receivingToIdleExt44, "receivingToIdleExt44");
        RKH_TR_FWK_OBJ_NAME(GetStatusToIdleExt45, "GetStatusToIdleExt45");
        RKH_TR_FWK_OBJ_NAME(GetStatusToIdleExt46, "GetStatusToIdleExt46");
        RKH_TR_FWK_OBJ_NAME(SendingToIdleExt47, "SendingToIdleExt47");
        RKH_TR_FWK_OBJ_NAME(waitOkToSendingFinalExt50, "waitOkToSendingFinalExt50");
        RKH_TR_FWK_OBJ_NAME(WaitPromptTowaitOkExt51, "WaitPromptTowaitOkExt51");
        RKH_TR_FWK_OBJ_NAME(WaitRetryConfigToConfigureHistExt54, "WaitRetryConfigToConfigureHistExt54");
        RKH_TR_FWK_OBJ_NAME(WaitNetClockSyncToLocalTimeExt55, "WaitNetClockSyncToLocalTimeExt55");
        RKH_TR_FWK_OBJ_NAME(ConfigureToConnectingExt58, "ConfigureToConnectingExt58");
        RKH_TR_FWK_OBJ_NAME(ToGetOperExt59, "ToGetOperExt59");
        RKH_TR_FWK_OBJ_NAME(CheckIPToCheckIPExt64, "CheckIPToCheckIPExt64");
        RKH_TR_FWK_OBJ_NAME(GetOperToSetAPNExt65, "GetOperToSetAPNExt65");
        RKH_TR_FWK_OBJ_NAME(C2ToRegisteredFinalExt67, "C2ToRegisteredFinalExt67");
        RKH_TR_FWK_OBJ_NAME(WaitRetryConnectToConnectingExt68, "WaitRetryConnectToConnectingExt68");
        RKH_TR_FWK_OBJ_NAME(InactiveToInactiveLoc0, "InactiveToInactiveLoc0");
        RKH_TR_FWK_OBJ_NAME(InactiveToInactiveLoc1, "InactiveToInactiveLoc1");
        RKH_TR_FWK_OBJ_NAME(ActiveToActiveLoc2, "ActiveToActiveLoc2");
        RKH_TR_FWK_OBJ_NAME(ActiveToActiveLoc3, "ActiveToActiveLoc3");
        RKH_TR_FWK_OBJ_NAME(ActiveToActiveLoc4, "ActiveToActiveLoc4");
        RKH_TR_FWK_OBJ_NAME(UnregisteredToUnregisteredLoc16, "UnregisteredToUnregisteredLoc16");
        RKH_TR_FWK_OBJ_NAME(UnregisteredToUnregisteredLoc17, "UnregisteredToUnregisteredLoc17");
        RKH_TR_FWK_OBJ_NAME(GetStatusToGetStatusLoc23, "GetStatusToGetStatusLoc23");
        RKH_TR_FWK_OBJ_NAME(GetStatusToGetStatusLoc24, "GetStatusToGetStatusLoc24");
        RKH_TR_FWK_OBJ_NAME(enSync, "enSync");
        RKH_TR_FWK_OBJ_NAME(enInit, "enInit");
        RKH_TR_FWK_OBJ_NAME(enPin, "enPin");
        RKH_TR_FWK_OBJ_NAME(enSetPin, "enSetPin");
        RKH_TR_FWK_OBJ_NAME(enEnableNetTime, "enEnableNetTime");
        RKH_TR_FWK_OBJ_NAME(enGetImei, "enGetImei");
        RKH_TR_FWK_OBJ_NAME(enSetManualGet, "enSetManualGet");
        RKH_TR_FWK_OBJ_NAME(enCipShutdown, "enCipShutdown");
        RKH_TR_FWK_OBJ_NAME(enConFailure, "enConFailure");
        RKH_TR_FWK_OBJ_NAME(enUnregistered, "enUnregistered");
        RKH_TR_FWK_OBJ_NAME(enRegistered, "enRegistered");
        RKH_TR_FWK_OBJ_NAME(enConnected, "enConnected");
        RKH_TR_FWK_OBJ_NAME(enSetAPN, "enSetAPN");
        RKH_TR_FWK_OBJ_NAME(enEnableNetwork, "enEnableNetwork");
        RKH_TR_FWK_OBJ_NAME(enCheckIP, "enCheckIP");
        RKH_TR_FWK_OBJ_NAME(enGetOper, "enGetOper");
        RKH_TR_FWK_OBJ_NAME(exConFailure, "exConFailure");
        RKH_TR_FWK_OBJ_NAME(exRegistered, "exRegistered");
        RKH_TR_FWK_OBJ_NAME(exConnected, "exConnected");
        RKH_TR_FWK_OBJ_NAME(exGetStatus, "exGetStatus");
        RKH_TR_FWK_OBJ_NAME(exWaitRetryConnect, "exWaitRetryConnect");
        RKH_TR_FWK_OBJ_NAME(isCondGetStatusToIdle45, "isCondGetStatusToIdle45");
        RKH_TR_FWK_OBJ_NAME(isCondC0ToSync10, "isCondC0ToSync10");
        RKH_TR_FWK_OBJ_NAME(isCondC1ToWaitRetryConnect27, "isCondC1ToWaitRetryConnect27");
        RKH_TR_FWK_OBJ_NAME(isCondC2ToWaitRetryConfig66, "isCondC2ToWaitRetryConfig66");
    #endif
	
	init(me);
	me->retryCount = 0;
	Config_getConnectionDomain(me->domain);
	Config_getConnectionPort(me->port);
}

void 
InactiveToActiveExt1(ConMgr *const me, RKH_EVT_T *pe)
{
    ModMgr_open();
    powerOn();
}

void 
ActiveToInactiveExt2(ConMgr *const me, RKH_EVT_T *pe)
{
    ModMgr_close();
    powerOff();
}

void 
ToInitializeExt4(ConMgr *const me, RKH_EVT_T *pe)
{
    me->retryCount = 0;
}

void 
GetImeiToCipShutdownExt18(ConMgr *const me, RKH_EVT_T *pe)
{
    storeImei(me, pe);
}

void 
RegisteredToRegisteredFinalExt25(ConMgr *const me, RKH_EVT_T *pe)
{
    stopSMS();
}

void 
C1ToRegisteredFinalExt28(ConMgr *const me, RKH_EVT_T *pe)
{
    stopSMS();
}

void 
LocalTimeToConfigureExt29(ConMgr *const me, RKH_EVT_T *pe)
{
    rtimeSync(pe);
}

void 
ConnectingToDisconnectingExt33(ConMgr *const me, RKH_EVT_T *pe)
{
    ModCmd_disconnect();
}

void 
ToWaitingServerExt34(ConMgr *const me, RKH_EVT_T *pe)
{
    ModCmd_connect(me->protocol, me->domain, me->port);
}

void 
WaitingServerToWaitingServerExt35(ConMgr *const me, RKH_EVT_T *pe)
{
    ModCmd_getConnStatus();
}

void 
ConnectedToRestartingExt37(ConMgr *const me, RKH_EVT_T *pe)
{
    ModCmd_disconnect();
}

void 
IdleToGetStatusExt40(ConMgr *const me, RKH_EVT_T *pe)
{
    ModCmd_getConnStatus();
}

void 
IdleToSendingExt41(ConMgr *const me, RKH_EVT_T *pe)
{
    sendRequest(pe);
}

void 
IdleToreceivingExt42(ConMgr *const me, RKH_EVT_T *pe)
{
    readData();
}

void 
receivingToIdleExt43(ConMgr *const me, RKH_EVT_T *pe)
{
    recvFail(me);
}

void 
receivingToIdleExt44(ConMgr *const me, RKH_EVT_T *pe)
{
    recvOk(me, pe);
}

void 
GetStatusToIdleExt45(ConMgr *const me, RKH_EVT_T *pe)
{
    me->retryCount += 1;
    ModCmd_init();
}

void 
GetStatusToIdleExt46(ConMgr *const me, RKH_EVT_T *pe)
{
    me->retryCount = 0;
}

void 
SendingToIdleExt47(ConMgr *const me, RKH_EVT_T *pe)
{
    sendFail(me);
}

void 
waitOkToSendingFinalExt50(ConMgr *const me, RKH_EVT_T *pe)
{
    me->retryCount = 0;
    sendOk(me);
}

void 
WaitPromptTowaitOkExt51(ConMgr *const me, RKH_EVT_T *pe)
{
    flushData();
}

void 
WaitRetryConfigToConfigureHistExt54(ConMgr *const me, RKH_EVT_T *pe)
{
    me->retryCount += 1;
    ModCmd_init();
}

void 
WaitNetClockSyncToLocalTimeExt55(ConMgr *const me, RKH_EVT_T *pe)
{
    ModCmd_getLocalTime();
}

void 
ConfigureToConnectingExt58(ConMgr *const me, RKH_EVT_T *pe)
{
    me->retryCount = 0;
}

void 
ToGetOperExt59(ConMgr *const me, RKH_EVT_T *pe)
{
    me->retryCount = 0;
}

void 
CheckIPToCheckIPExt64(ConMgr *const me, RKH_EVT_T *pe)
{
    ModCmd_requestIP();
}

void 
GetOperToSetAPNExt65(ConMgr *const me, RKH_EVT_T *pe)
{
    storeOper(me, pe);
}

void 
C2ToRegisteredFinalExt67(ConMgr *const me, RKH_EVT_T *pe)
{
    stopSMS();
}

void 
WaitRetryConnectToConnectingExt68(ConMgr *const me, RKH_EVT_T *pe)
{
    me->retryCount += 1;
}

void 
InactiveToInactiveLoc0(ConMgr *const me, RKH_EVT_T *pe)
{
    recvFail(me);
}

void 
InactiveToInactiveLoc1(ConMgr *const me, RKH_EVT_T *pe)
{
    sendFail(me);
}

void 
ActiveToActiveLoc2(ConMgr *const me, RKH_EVT_T *pe)
{
    sendFail(me);
}

void 
ActiveToActiveLoc3(ConMgr *const me, RKH_EVT_T *pe)
{
    recvFail(me);
}

void 
ActiveToActiveLoc4(ConMgr *const me, RKH_EVT_T *pe)
{
    setSigLevel(me, pe);
}

void 
UnregisteredToUnregisteredLoc16(ConMgr *const me, RKH_EVT_T *pe)
{
    ModCmd_getRegStatus();
}

void 
UnregisteredToUnregisteredLoc17(ConMgr *const me, RKH_EVT_T *pe)
{
    startRegStatusTimer(me);
}

void 
GetStatusToGetStatusLoc23(ConMgr *const me, RKH_EVT_T *pe)
{
    reqDefer(pe);
}

void 
GetStatusToGetStatusLoc24(ConMgr *const me, RKH_EVT_T *pe)
{
    reqDefer(pe);
}

/* ............................. Entry actions ............................. */
void 
enSync(ConMgr *const me)
{
    me->retryCount += 1;
    ModCmd_sync();
}

void 
enInit(ConMgr *const me)
{
    modemFound();
    ModCmd_initStr();
}

void 
enPin(ConMgr *const me)
{
    ModCmd_getPinStatus();
}

void 
enSetPin(ConMgr *const me)
{
    ModCmd_setPin(Config_getSIMPinNumber());
}

void 
enEnableNetTime(ConMgr *const me)
{
    ModCmd_enableNetTime();
}

void 
enGetImei(ConMgr *const me)
{
    ModCmd_getImei();
}

void 
enSetManualGet(ConMgr *const me)
{
    ModCmd_setManualGet();
}

void 
enCipShutdown(ConMgr *const me)
{
    ModCmd_cipShutdown();
}

void 
enConFailure(ConMgr *const me)
{
    powerOff();
    RKH_SET_STATIC_EVENT(&me->tmEvtObj0, evTout0);
    RKH_TMR_INIT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj0), NULL);
    RKH_TMR_ONESHOT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime0);
}

void 
enUnregistered(ConMgr *const me)
{
    ModCmd_getRegStatus();
    RKH_SET_STATIC_EVENT(&me->tmEvtObj1, evTout1);
    RKH_TMR_INIT(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj1), NULL);
    RKH_TMR_ONESHOT(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime1);
}

void 
enRegistered(ConMgr *const me)
{
    registered();
}

void 
enWaitingServer(ConMgr *const me)
{
    RKH_SET_STATIC_EVENT(&me->tmEvtObj2, evTout2);
    RKH_TMR_INIT(&me->tmEvtObj2.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj2), NULL);
    RKH_TMR_ONESHOT(&me->tmEvtObj2.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime2);
}

void 
enConnected(ConMgr *const me)
{
    socketConnected(me);
}

void 
enIdle(ConMgr *const me)
{
    RKH_SET_STATIC_EVENT(&me->tmEvtObj3, evTout3);
    RKH_TMR_INIT(&me->tmEvtObj3.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj3), NULL);
    RKH_TMR_ONESHOT(&me->tmEvtObj3.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime3);
}

void 
enWaitReopen(ConMgr *const me)
{
    RKH_SET_STATIC_EVENT(&me->tmEvtObj4, evTout4);
    RKH_TMR_INIT(&me->tmEvtObj4.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj4), NULL);
    RKH_TMR_ONESHOT(&me->tmEvtObj4.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime4);
}

void 
enWaitRetryConfig(ConMgr *const me)
{
    RKH_SET_STATIC_EVENT(&me->tmEvtObj5, evTout5);
    RKH_TMR_INIT(&me->tmEvtObj5.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj5), NULL);
    RKH_TMR_ONESHOT(&me->tmEvtObj5.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime5);
}

void 
enWaitNetClockSync(ConMgr *const me)
{
    RKH_SET_STATIC_EVENT(&me->tmEvtObj6, evTout6);
    RKH_TMR_INIT(&me->tmEvtObj6.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj6), NULL);
    RKH_TMR_ONESHOT(&me->tmEvtObj6.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime6);
}

void 
enSetAPN(ConMgr *const me)
{
    setupAPN(me);
}

void 
enEnableNetwork(ConMgr *const me)
{
    ModCmd_startNetwork();
}

void 
enCheckIP(ConMgr *const me)
{
    ModCmd_getIpStatus();
}

void 
enGetOper(ConMgr *const me)
{
    ModCmd_getOper();
}

void 
enWaitRetryConnect(ConMgr *const me)
{
    RKH_SET_STATIC_EVENT(&me->tmEvtObj7, evTout7);
    RKH_TMR_INIT(&me->tmEvtObj7.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj7), NULL);
    RKH_TMR_ONESHOT(&me->tmEvtObj7.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime7);
}

/* ............................. Exit actions .............................. */
void 
exConFailure(ConMgr *const me)
{
    powerOn();
    rkh_tmr_stop(&me->tmEvtObj0.tmr);
}

void 
exUnregistered(ConMgr *const me)
{
    rkh_tmr_stop(&me->tmEvtObj1.tmr);
}

void 
exRegistered(ConMgr *const me)
{
    unregistered();
}

void 
exWaitingServer(ConMgr *const me)
{
    rkh_tmr_stop(&me->tmEvtObj2.tmr);
}

void 
exConnected(ConMgr *const me)
{
    socketDisconnected(me);
}

void 
exIdle(ConMgr *const me)
{
    rkh_tmr_stop(&me->tmEvtObj3.tmr);
}

void 
exGetStatus(ConMgr *const me)
{
    reqRecall(me);
}

void 
exWaitReopen(ConMgr *const me)
{
    rkh_tmr_stop(&me->tmEvtObj4.tmr);
}

void 
exWaitRetryConfig(ConMgr *const me)
{
    rkh_tmr_stop(&me->tmEvtObj5.tmr);
}

void 
exWaitNetClockSync(ConMgr *const me)
{
    rkh_tmr_stop(&me->tmEvtObj6.tmr);
}

void 
exWaitRetryConnect(ConMgr *const me)
{
    ModCmd_init();
    rkh_tmr_stop(&me->tmEvtObj7.tmr);
}

/* ................................ Guards ................................. */
rbool_t 
isCondGetStatusToIdle45(ConMgr *const me, RKH_EVT_T *pe)
{
    return ((me->retryCount < 5)) ? true : false;
}

rbool_t 
isCondC0ToSync10(ConMgr *const me, RKH_EVT_T *pe)
{
    return ((me->retryCount < 5)) ? true : false;
}

rbool_t 
isCondC1ToWaitRetryConnect27(ConMgr *const me, RKH_EVT_T *pe)
{
    return ((me->retryCount < Config_getMaxNumConnectRetries())) ? true : false;
}

rbool_t 
isCondC2ToWaitRetryConfig66(ConMgr *const me, RKH_EVT_T *pe)
{
    return ((me->retryCount < 5)) ? true : false;
}

/* ---------------------------- Global functions --------------------------- */
char *
ConMgr_getImei(void)
{
    return ConMgrInt.Imei;
}

/* ------------------------------ End of file ------------------------------ */
