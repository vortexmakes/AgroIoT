/**
 *  \file       GsmMgr.c
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
#include "GsmMgr.h"
#include "bsp.h"
#include "GsmMgrRequired.h"

/* ----------------------------- Local macros ------------------------------ */
#define WaitTime0	RKH_TIME_SEC(5)
#define WaitTime1	RKH_TIME_SEC(60)
#define WaitTime2	RKH_TIME_SEC(5)
#define WaitTime3	RKH_TIME_SEC(2)
#define WaitTime4	RKH_TIME_SEC(2)
#define WaitTime5	RKH_TIME_SEC(2)
#define WaitTime6	RKH_TIME_SEC(2)
#define WaitTime7	RKH_TIME_SEC(2)

/* ......................... Declares active object ........................ */
typedef struct GsmMgr GsmMgr;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE GsmMgr_Inactive, GsmMgr_Sync, GsmMgr_Init, GsmMgr_Pin, GsmMgr_SetPin, GsmMgr_EnableNetTime, GsmMgr_GetImei, GsmMgr_SetManualGet, GsmMgr_CipShutdown, GsmMgr_ConFailure, GsmMgr_Unregistered, GsmMgr_Registered, GsmMgr_WaitingClose, GsmMgr_WaitingServer, GsmMgr_Idle, GsmMgr_Receiving, GsmMgr_GetStatus, GsmMgr_WaitOk, GsmMgr_WaitPrompt, GsmMgr_Restarting, GsmMgr_WaitReopen, GsmMgr_WaitRetryConfig, GsmMgr_SetAPN, GsmMgr_EnableNetwork, GsmMgr_CheckIP, GsmMgr_GetOper, GsmMgr_WaitRetryConnect, GsmMgr_Disconnecting, GsmMgr_Disconnected, GsmMgr_Socket_WaitGsmReady, GsmMgr_SMS_WaitGsmReady, GsmMgr_SMSReady;
RKH_DCLR_COMP_STATE GsmMgr_Active, GsmMgr_Initialize, GsmMgr_Connecting, GsmMgr_Connected, GsmMgr_Sending, GsmMgr_Configure;
RKH_DCLR_CHOICE_STATE GsmMgr_C0, GsmMgr_C1, GsmMgr_C2;
RKH_DCLR_FINAL_STATE GsmMgr_InitializeFinal, GsmMgr_ActiveFinal, GsmMgr_SendingFinal, GsmMgr_ConfigureFinal;
RKH_DCLR_SHIST_STATE GsmMgr_ConfigureHist;

/* ........................ Declares effect actions ........................ */
void ToInactiveExt0(GsmMgr *const me, RKH_EVT_T *pe);
void InactiveToActiveExt1(GsmMgr *const me, RKH_EVT_T *pe);
void ActiveToInactiveExt2(GsmMgr *const me, RKH_EVT_T *pe);
void ToInitializeExt4(GsmMgr *const me, RKH_EVT_T *pe);
void GetImeiToCipShutdownExt18(GsmMgr *const me, RKH_EVT_T *pe);
void UnregisteredToRegisteredExt23(GsmMgr *const me, RKH_EVT_T *pe);
void RegisteredToRegisteredExt25(GsmMgr *const me, RKH_EVT_T *pe);
void C1ToSocket_WaitGsmReadyExt30(GsmMgr *const me, RKH_EVT_T *pe);
void ConnectingToDisconnectingExt33(GsmMgr *const me, RKH_EVT_T *pe);
void ConnectingToSocket_WaitGsmReadyExt34(GsmMgr *const me, RKH_EVT_T *pe);
void ToWaitingServerExt35(GsmMgr *const me, RKH_EVT_T *pe);
void WaitingServerToWaitingServerExt36(GsmMgr *const me, RKH_EVT_T *pe);
void ConnectedToRestartingExt38(GsmMgr *const me, RKH_EVT_T *pe);
void IdleToGetStatusExt41(GsmMgr *const me, RKH_EVT_T *pe);
void IdleToSendingExt42(GsmMgr *const me, RKH_EVT_T *pe);
void IdleToReceivingExt43(GsmMgr *const me, RKH_EVT_T *pe);
void ReceivingToIdleExt44(GsmMgr *const me, RKH_EVT_T *pe);
void ReceivingToIdleExt45(GsmMgr *const me, RKH_EVT_T *pe);
void GetStatusToIdleExt46(GsmMgr *const me, RKH_EVT_T *pe);
void GetStatusToIdleExt47(GsmMgr *const me, RKH_EVT_T *pe);
void SendingToIdleExt48(GsmMgr *const me, RKH_EVT_T *pe);
void WaitOkToSendingFinalExt51(GsmMgr *const me, RKH_EVT_T *pe);
void WaitPromptToWaitOkExt52(GsmMgr *const me, RKH_EVT_T *pe);
void WaitRetryConfigToConfigureHistExt55(GsmMgr *const me, RKH_EVT_T *pe);
void ConfigureToSocket_WaitGsmReadyExt57(GsmMgr *const me, RKH_EVT_T *pe);
void ConfigureToConnectingExt58(GsmMgr *const me, RKH_EVT_T *pe);
void ToGetOperExt59(GsmMgr *const me, RKH_EVT_T *pe);
void CheckIPToCheckIPExt64(GsmMgr *const me, RKH_EVT_T *pe);
void GetOperToSetAPNExt65(GsmMgr *const me, RKH_EVT_T *pe);
void C2ToSocket_WaitGsmReadyExt67(GsmMgr *const me, RKH_EVT_T *pe);
void WaitRetryConnectToConnectingExt68(GsmMgr *const me, RKH_EVT_T *pe);
void InactiveToInactiveLoc0(GsmMgr *const me, RKH_EVT_T *pe);
void InactiveToInactiveLoc1(GsmMgr *const me, RKH_EVT_T *pe);
void ActiveToActiveLoc2(GsmMgr *const me, RKH_EVT_T *pe);
void ActiveToActiveLoc3(GsmMgr *const me, RKH_EVT_T *pe);
void ActiveToActiveLoc4(GsmMgr *const me, RKH_EVT_T *pe);
void UnregisteredToUnregisteredLoc16(GsmMgr *const me, RKH_EVT_T *pe);
void UnregisteredToUnregisteredLoc17(GsmMgr *const me, RKH_EVT_T *pe);
void RegisteredToRegisteredLoc20(GsmMgr *const me, RKH_EVT_T *pe);
void RegisteredToRegisteredLoc21(GsmMgr *const me, RKH_EVT_T *pe);
void GetStatusToGetStatusLoc25(GsmMgr *const me, RKH_EVT_T *pe);
void GetStatusToGetStatusLoc26(GsmMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
void enSync(GsmMgr *const me);
void enInit(GsmMgr *const me);
void enPin(GsmMgr *const me);
void enSetPin(GsmMgr *const me);
void enEnableNetTime(GsmMgr *const me);
void enGetImei(GsmMgr *const me);
void enSetManualGet(GsmMgr *const me);
void enCipShutdown(GsmMgr *const me);
void enConFailure(GsmMgr *const me);
void enUnregistered(GsmMgr *const me);
void enRegistered(GsmMgr *const me);
void enWaitingServer(GsmMgr *const me);
void enConnected(GsmMgr *const me);
void enIdle(GsmMgr *const me);
void enWaitReopen(GsmMgr *const me);
void enWaitRetryConfig(GsmMgr *const me);
void enSetAPN(GsmMgr *const me);
void enEnableNetwork(GsmMgr *const me);
void enCheckIP(GsmMgr *const me);
void enGetOper(GsmMgr *const me);
void enWaitRetryConnect(GsmMgr *const me);

/* ......................... Declares exit actions ......................... */
void exConFailure(GsmMgr *const me);
void exUnregistered(GsmMgr *const me);
void exRegistered(GsmMgr *const me);
void exWaitingServer(GsmMgr *const me);
void exConnected(GsmMgr *const me);
void exIdle(GsmMgr *const me);
void exGetStatus(GsmMgr *const me);
void exWaitReopen(GsmMgr *const me);
void exWaitRetryConfig(GsmMgr *const me);
void exWaitRetryConnect(GsmMgr *const me);

/* ............................ Declares guards ............................ */
rbool_t isCondGetStatusToIdle46(GsmMgr *const me, RKH_EVT_T *pe);
rbool_t isCondC0ToSync10(GsmMgr *const me, RKH_EVT_T *pe);
rbool_t isCondC1ToWaitRetryConnect29(GsmMgr *const me, RKH_EVT_T *pe);
rbool_t isCondC2ToWaitRetryConfig66(GsmMgr *const me, RKH_EVT_T *pe);

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(GsmMgr_Inactive, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Sync, enSync, NULL, &GsmMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Init, enInit, NULL, &GsmMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Pin, enPin, NULL, &GsmMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_SetPin, enSetPin, NULL, &GsmMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_EnableNetTime, enEnableNetTime, NULL, &GsmMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_GetImei, enGetImei, NULL, &GsmMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_SetManualGet, enSetManualGet, NULL, &GsmMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_CipShutdown, enCipShutdown, NULL, &GsmMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_ConFailure, enConFailure, exConFailure, &GsmMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Unregistered, enUnregistered, exUnregistered, &GsmMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Registered, enRegistered, exRegistered, &GsmMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_WaitingClose, NULL, NULL, &GsmMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_WaitingServer, enWaitingServer, exWaitingServer, &GsmMgr_Connecting, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Idle, enIdle, exIdle, &GsmMgr_Connected, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Receiving, NULL, NULL, &GsmMgr_Connected, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_GetStatus, NULL, exGetStatus, &GsmMgr_Connected, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_WaitOk, NULL, NULL, &GsmMgr_Sending, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_WaitPrompt, NULL, NULL, &GsmMgr_Sending, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Restarting, NULL, NULL, &GsmMgr_Connecting, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_WaitReopen, enWaitReopen, exWaitReopen, &GsmMgr_Connecting, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_WaitRetryConfig, enWaitRetryConfig, exWaitRetryConfig, &GsmMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_SetAPN, enSetAPN, NULL, &GsmMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_EnableNetwork, enEnableNetwork, NULL, &GsmMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_CheckIP, enCheckIP, NULL, &GsmMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_GetOper, enGetOper, NULL, &GsmMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_WaitRetryConnect, enWaitRetryConnect, exWaitRetryConnect, &GsmMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Disconnecting, NULL, NULL, &GsmMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Disconnected, NULL, NULL, &GsmMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Socket_WaitGsmReady, NULL, NULL, &GsmMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_SMS_WaitGsmReady, NULL, NULL, &GsmMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_SMSReady, NULL, NULL, &GsmMgr_Active, NULL);

RKH_CREATE_COMP_REGION_STATE(GsmMgr_Active, NULL, NULL, RKH_ROOT, &GsmMgr_Initialize, ToInitializeExt4, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(GsmMgr_Initialize, NULL, NULL, &GsmMgr_Active, &GsmMgr_Sync, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(GsmMgr_Connecting, NULL, NULL, &GsmMgr_Active, &GsmMgr_WaitingServer, ToWaitingServerExt35, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(GsmMgr_Connected, enConnected, exConnected, &GsmMgr_Connecting, &GsmMgr_Idle, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(GsmMgr_Sending, NULL, NULL, &GsmMgr_Connected, &GsmMgr_WaitPrompt, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_HISTORY_STORAGE(GsmMgr_Configure);
RKH_CREATE_COMP_REGION_STATE(GsmMgr_Configure, NULL, NULL, &GsmMgr_Active, &GsmMgr_GetOper, ToGetOperExt59, RKH_SHISTORY, NULL, NULL, NULL, RKH_GET_HISTORY_STORAGE(GsmMgr_Configure));

RKH_CREATE_TRANS_TABLE(GsmMgr_Inactive)
	RKH_TRREG(evOpen, NULL, InactiveToActiveExt1, &GsmMgr_Active),
	RKH_TRINT(evRecv, NULL, InactiveToInactiveLoc0),
	RKH_TRINT(evSend, NULL, InactiveToInactiveLoc1),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Active)
	RKH_TRREG(evClose, NULL, ActiveToInactiveExt2, &GsmMgr_Inactive),
	RKH_TRCOMPLETION(NULL, NULL, &GsmMgr_Inactive),
	RKH_TRINT(evSend, NULL, ActiveToActiveLoc2),
	RKH_TRINT(evRecv, NULL, ActiveToActiveLoc3),
	RKH_TRINT(evSigLevel, NULL, ActiveToActiveLoc4),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Initialize)
	RKH_TRREG(evNoResponse, NULL, NULL, &GsmMgr_ConFailure),
	RKH_TRREG(evError, NULL, NULL, &GsmMgr_ConFailure),
	RKH_TRCOMPLETION(NULL, NULL, &GsmMgr_ConFailure),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Sync)
	RKH_TRREG(evNoResponse, NULL, NULL, &GsmMgr_C0),
	RKH_TRREG(evError, NULL, NULL, &GsmMgr_C0),
	RKH_TRREG(evOk, NULL, NULL, &GsmMgr_Init),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Init)
	RKH_TRREG(evOk, NULL, NULL, &GsmMgr_Pin),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Pin)
	RKH_TRREG(evSimError, NULL, NULL, &GsmMgr_InitializeFinal),
	RKH_TRREG(evSimPin, NULL, NULL, &GsmMgr_SetPin),
	RKH_TRREG(evSimReady, NULL, NULL, &GsmMgr_EnableNetTime),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_SetPin)
	RKH_TRREG(evOk, NULL, NULL, &GsmMgr_Pin),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_EnableNetTime)
	RKH_TRREG(evOk, NULL, NULL, &GsmMgr_GetImei),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_GetImei)
	RKH_TRREG(evImei, NULL, GetImeiToCipShutdownExt18, &GsmMgr_CipShutdown),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_SetManualGet)
	RKH_TRREG(evOk, NULL, NULL, &GsmMgr_Unregistered),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_CipShutdown)
	RKH_TRREG(evOk, NULL, NULL, &GsmMgr_SetManualGet),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_ConFailure)
	RKH_TRREG(evTout0, NULL, NULL, &GsmMgr_Initialize),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Unregistered)
	RKH_TRREG(evTout1, NULL, NULL, &GsmMgr_ConFailure),
	RKH_TRREG(evReg, NULL, UnregisteredToRegisteredExt23, &GsmMgr_Registered),
	RKH_TRINT(evRegStatusTout, NULL, UnregisteredToUnregisteredLoc16),
	RKH_TRINT(evNoReg, NULL, UnregisteredToUnregisteredLoc17),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Registered)
	RKH_TRREG(evNoReg, NULL, NULL, &GsmMgr_Unregistered),
	RKH_TRREG(evTout2, NULL, RegisteredToRegisteredExt25, &GsmMgr_Registered),
	RKH_TRREG(evClose, NULL, NULL, &GsmMgr_WaitingClose),
	RKH_TRREG(evGsmError, NULL, NULL, &GsmMgr_ConFailure),
	RKH_TRINT(evNetClockSync, NULL, RegisteredToRegisteredLoc20),
	RKH_TRINT(evLocalTime, NULL, RegisteredToRegisteredLoc21),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitingClose)
	RKH_TRREG(evDisconnected, NULL, NULL, &GsmMgr_ActiveFinal),
	RKH_TRREG(evNoResponse, NULL, NULL, &GsmMgr_ActiveFinal),
	RKH_TRREG(evGsmError, NULL, NULL, &GsmMgr_ActiveFinal),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Connecting)
	RKH_TRREG(evNoResponse, NULL, NULL, &GsmMgr_C1),
	RKH_TRREG(evClosed, NULL, NULL, &GsmMgr_C1),
	RKH_TRREG(evIPStatus, NULL, NULL, &GsmMgr_C1),
	RKH_TRREG(evError, NULL, NULL, &GsmMgr_C1),
	RKH_TRREG(evIPStart, NULL, NULL, &GsmMgr_Configure),
	RKH_TRREG(evIPInitial, NULL, NULL, &GsmMgr_Configure),
	RKH_TRREG(evIPGprsAct, NULL, NULL, &GsmMgr_Configure),
	RKH_TRREG(evClose, NULL, ConnectingToDisconnectingExt33, &GsmMgr_Disconnecting),
	RKH_TRREG(evError, NULL, ConnectingToSocket_WaitGsmReadyExt34, &GsmMgr_Socket_WaitGsmReady),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitingServer)
	RKH_TRREG(evTout3, NULL, WaitingServerToWaitingServerExt36, &GsmMgr_WaitingServer),
	RKH_TRREG(evConnected, NULL, NULL, &GsmMgr_Connected),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Connected)
	RKH_TRREG(evRestart, NULL, ConnectedToRestartingExt38, &GsmMgr_Restarting),
	RKH_TRREG(evClosed, NULL, NULL, &GsmMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Idle)
	RKH_TRREG(evTout4, NULL, IdleToGetStatusExt41, &GsmMgr_GetStatus),
	RKH_TRREG(evSend, NULL, IdleToSendingExt42, &GsmMgr_Sending),
	RKH_TRREG(evRecv, NULL, IdleToReceivingExt43, &GsmMgr_Receiving),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Receiving)
	RKH_TRREG(evNoResponse, NULL, ReceivingToIdleExt44, &GsmMgr_Idle),
	RKH_TRREG(evError, NULL, ReceivingToIdleExt44, &GsmMgr_Idle),
	RKH_TRREG(evReceived, NULL, ReceivingToIdleExt45, &GsmMgr_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_GetStatus)
	RKH_TRREG(evNoResponse, isCondGetStatusToIdle46, GetStatusToIdleExt46, &GsmMgr_Idle),
	RKH_TRREG(evConnected, NULL, GetStatusToIdleExt47, &GsmMgr_Idle),
	RKH_TRINT(evSend, NULL, GetStatusToGetStatusLoc25),
	RKH_TRINT(evRecv, NULL, GetStatusToGetStatusLoc26),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Sending)
	RKH_TRREG(evNoResponse, NULL, SendingToIdleExt48, &GsmMgr_Idle),
	RKH_TRREG(evError, NULL, SendingToIdleExt48, &GsmMgr_Idle),
	RKH_TRCOMPLETION(NULL, NULL, &GsmMgr_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitOk)
	RKH_TRREG(evOk, NULL, WaitOkToSendingFinalExt51, &GsmMgr_SendingFinal),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitPrompt)
	RKH_TRREG(evOk, NULL, WaitPromptToWaitOkExt52, &GsmMgr_WaitOk),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Restarting)
	RKH_TRREG(evDisconnected, NULL, NULL, &GsmMgr_WaitReopen),
	RKH_TRREG(evNoResponse, NULL, NULL, &GsmMgr_WaitReopen),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitReopen)
	RKH_TRREG(evTout5, NULL, NULL, &GsmMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitRetryConfig)
	RKH_TRREG(evTout6, NULL, WaitRetryConfigToConfigureHistExt55, &GsmMgr_ConfigureHist),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Configure)
	RKH_TRREG(evNoResponse, NULL, NULL, &GsmMgr_C2),
	RKH_TRREG(evError, NULL, ConfigureToSocket_WaitGsmReadyExt57, &GsmMgr_Socket_WaitGsmReady),
	RKH_TRCOMPLETION(NULL, ConfigureToConnectingExt58, &GsmMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_SetAPN)
	RKH_TRREG(evOk, NULL, NULL, &GsmMgr_EnableNetwork),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_EnableNetwork)
	RKH_TRREG(evOk, NULL, NULL, &GsmMgr_CheckIP),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_CheckIP)
	RKH_TRREG(evIPStatus, NULL, NULL, &GsmMgr_ConfigureFinal),
	RKH_TRREG(evIP, NULL, NULL, &GsmMgr_CheckIP),
	RKH_TRREG(evIPInitial, NULL, NULL, &GsmMgr_CheckIP),
	RKH_TRREG(evIPStart, NULL, NULL, &GsmMgr_CheckIP),
	RKH_TRREG(evIPGprsAct, NULL, CheckIPToCheckIPExt64, &GsmMgr_CheckIP),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_GetOper)
	RKH_TRREG(evOper, NULL, GetOperToSetAPNExt65, &GsmMgr_SetAPN),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitRetryConnect)
	RKH_TRREG(evTout7, NULL, WaitRetryConnectToConnectingExt68, &GsmMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Disconnecting)
	RKH_TRREG(evDisconnected, NULL, NULL, &GsmMgr_Disconnected),
	RKH_TRREG(evNoResponse, NULL, NULL, &GsmMgr_Disconnected),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Disconnected)
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Socket_WaitGsmReady)
	RKH_TRREG(evGsmReady, NULL, NULL, &GsmMgr_Configure),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_SMS_WaitGsmReady)
	RKH_TRREG(evGsmReady, NULL, NULL, &GsmMgr_SMSReady),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_SMSReady)
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(GsmMgr_C0);
RKH_CREATE_CHOICE_STATE(GsmMgr_C1);
RKH_CREATE_CHOICE_STATE(GsmMgr_C2);

RKH_CREATE_BRANCH_TABLE(GsmMgr_C0)
	RKH_BRANCH(isCondC0ToSync10, NULL, &GsmMgr_Sync),
	RKH_BRANCH(ELSE, NULL, &GsmMgr_InitializeFinal),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(GsmMgr_C1)
	RKH_BRANCH(isCondC1ToWaitRetryConnect29, NULL, &GsmMgr_WaitRetryConnect),
	RKH_BRANCH(ELSE, C1ToSocket_WaitGsmReadyExt30, &GsmMgr_Socket_WaitGsmReady),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(GsmMgr_C2)
	RKH_BRANCH(isCondC2ToWaitRetryConfig66, NULL, &GsmMgr_WaitRetryConfig),
	RKH_BRANCH(ELSE, C2ToSocket_WaitGsmReadyExt67, &GsmMgr_Socket_WaitGsmReady),
RKH_END_BRANCH_TABLE


/* ............................. Active object ............................. */
struct GsmMgr
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

RKH_SMA_CREATE(GsmMgr, gsmMgr, 1, HCAL, &GsmMgr_Inactive, ToInactiveExt0, NULL);
RKH_SMA_DEF_PTR(gsmMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct GsmMgrInternal
{
    GsmMgr *pGsmMgr;
    
    char Imei[IMEI_BUF_SIZE];
    char Oper[OPER_BUF_SIZE];
    char Domain[DOMAIN_BUF_SIZE];
    char Port[PORT_BUF_SIZE];

    RKH_QUEUE_T qDefer;
    RKH_EVT_T *qDefer_sto[SIZEOF_QDEFER];

    SendEvt *psend;
    RKHTmEvt tmEvtRegStatus;
} GsmMgrInternal;

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


static GsmMgrInternal GsmMgrInt;

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
init(GsmMgr *const me)
{
    topic_subscribe(TCPConnection, me);
    topic_subscribe(ModURC, me);

    rkh_queue_init(&GsmMgrInt.qDefer, (const void **)GsmMgrInt.qDefer_sto, 
                    SIZEOF_QDEFER, CV(0));

    GsmMgrInt.pGsmMgr = me;

    me->imei = GsmMgrInt.Imei;
    me->oper = GsmMgrInt.Oper;
    me->protocol = CONNECTION_PROT;
    me->domain = GsmMgrInt.Domain;
    me->port = GsmMgrInt.Port;


    bsp_SIMSelect(MainSIM);
}

static void
storeImei(GsmMgr *const me, RKH_EVT_T *pe)
{
    ImeiEvt *p;

    (void)me;

    p = RKH_UPCAST(ImeiEvt, pe);
    strcpy(me->imei, p->buf);
}

static void
storeOper(GsmMgr *const me, RKH_EVT_T *pe)
{
    OperEvt *p;

    p = RKH_UPCAST(OperEvt, pe);
    strcpy(me->oper, p->buf);
}

static void
setupAPN(GsmMgr *const me)
{
    Apn *apn;

    apn = getAPNbyOper(me->oper);
    ModCmd_setupAPN(apn->addr, apn->usr, apn->psw);
}

static void
startRegStatusTimer(GsmMgr *const me)
{
    RKH_SET_STATIC_EVENT(&GsmMgrInt.tmEvtRegStatus, evRegStatusTout);
    RKH_TMR_INIT(&GsmMgrInt.tmEvtRegStatus.tmr, 
                    RKH_UPCAST(RKH_EVT_T, &GsmMgrInt.tmEvtRegStatus), NULL);
    RKH_TMR_ONESHOT(&GsmMgrInt.tmEvtRegStatus.tmr, RKH_UPCAST(RKH_SMA_T, me), 
                        CHECK_REG_STATUS_PERIOD);
}

static void
sendRequest(RKH_EVT_T *pe)
{
    GsmMgrInt.psend = RKH_UPCAST(SendEvt, pe);

    ModCmd_sendDataRequest((rui16_t)(GsmMgrInt.psend->size));
}

static void
sendOk(GsmMgr *const me)
{
    topic_publish(TCPConnection, &e_Sent, me);
}

static void
sendFail(GsmMgr *const me)
{
    topic_publish(TCPConnection, &e_SendFail, me);

    ModCmd_init();
}

static void
flushData(void)
{
    ModCmd_sendData(GsmMgrInt.psend->buf, GsmMgrInt.psend->size);
}

static void
readData(void)
{
    ModCmd_readData();
}

static void
recvOk(GsmMgr *const me, RKH_EVT_T *pe)
{
    topic_publish(TCPConnection, pe, me);

    bsp_recvOk();
}

static void
recvFail(GsmMgr *const me)
{
    topic_publish(TCPConnection, &e_RecvFail, me);

    ModCmd_init();
}

static void
setSigLevel(GsmMgr *const me, RKH_EVT_T *pe)
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
    if (rkh_queue_is_full(&GsmMgrInt.qDefer) != RKH_TRUE)
    {
        rkh_sma_defer(&GsmMgrInt.qDefer, pe);
    }
}

static void
reqRecall(GsmMgr *const me)
{
    rkh_sma_recall((RKH_SMA_T *)me, &GsmMgrInt.qDefer);
}

static void
socketConnected(GsmMgr *const me)
{
    topic_publish(TCPConnection, &e_NetConnected, me);

    bsp_netStatus(ConnectedSt);
}

static void
socketDisconnected(GsmMgr *const me)
{
    topic_publish(TCPConnection, &e_NetDisconnected, me);

    bsp_netStatus(DisconnectedSt);
}

static void
gsmReady(void)
{

}

static void
gsmInError(void)
{

}

/* ............................ Effect actions ............................. */
void 
ToInactiveExt0(GsmMgr *const me, RKH_EVT_T *pe)
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
	RKH_TR_FWK_STATE(me, &GsmMgr_Inactive);
	RKH_TR_FWK_STATE(me, &GsmMgr_Active);
	RKH_TR_FWK_STATE(me, &GsmMgr_Initialize);
	RKH_TR_FWK_STATE(me, &GsmMgr_Sync);
	RKH_TR_FWK_STATE(me, &GsmMgr_Init);
	RKH_TR_FWK_STATE(me, &GsmMgr_Pin);
	RKH_TR_FWK_STATE(me, &GsmMgr_SetPin);
	RKH_TR_FWK_STATE(me, &GsmMgr_EnableNetTime);
	RKH_TR_FWK_STATE(me, &GsmMgr_GetImei);
	RKH_TR_FWK_STATE(me, &GsmMgr_SetManualGet);
	RKH_TR_FWK_STATE(me, &GsmMgr_CipShutdown);
	RKH_TR_FWK_STATE(me, &GsmMgr_ConFailure);
	RKH_TR_FWK_STATE(me, &GsmMgr_Unregistered);
	RKH_TR_FWK_STATE(me, &GsmMgr_Registered);
	RKH_TR_FWK_STATE(me, &GsmMgr_WaitingClose);
	RKH_TR_FWK_STATE(me, &GsmMgr_Connecting);
	RKH_TR_FWK_STATE(me, &GsmMgr_WaitingServer);
	RKH_TR_FWK_STATE(me, &GsmMgr_Connected);
	RKH_TR_FWK_STATE(me, &GsmMgr_Idle);
	RKH_TR_FWK_STATE(me, &GsmMgr_Receiving);
	RKH_TR_FWK_STATE(me, &GsmMgr_GetStatus);
	RKH_TR_FWK_STATE(me, &GsmMgr_Sending);
	RKH_TR_FWK_STATE(me, &GsmMgr_WaitOk);
	RKH_TR_FWK_STATE(me, &GsmMgr_WaitPrompt);
	RKH_TR_FWK_STATE(me, &GsmMgr_Restarting);
	RKH_TR_FWK_STATE(me, &GsmMgr_WaitReopen);
	RKH_TR_FWK_STATE(me, &GsmMgr_WaitRetryConfig);
	RKH_TR_FWK_STATE(me, &GsmMgr_Configure);
	RKH_TR_FWK_STATE(me, &GsmMgr_SetAPN);
	RKH_TR_FWK_STATE(me, &GsmMgr_EnableNetwork);
	RKH_TR_FWK_STATE(me, &GsmMgr_CheckIP);
	RKH_TR_FWK_STATE(me, &GsmMgr_GetOper);
	RKH_TR_FWK_STATE(me, &GsmMgr_WaitRetryConnect);
	RKH_TR_FWK_STATE(me, &GsmMgr_Disconnecting);
	RKH_TR_FWK_STATE(me, &GsmMgr_Disconnected);
	RKH_TR_FWK_STATE(me, &GsmMgr_Socket_WaitGsmReady);
	RKH_TR_FWK_STATE(me, &GsmMgr_SMS_WaitGsmReady);
	RKH_TR_FWK_STATE(me, &GsmMgr_SMSReady);
	RKH_TR_FWK_PSTATE(me, &GsmMgr_ConfigureHist);
	RKH_TR_FWK_PSTATE(me, &GsmMgr_C0);
	RKH_TR_FWK_PSTATE(me, &GsmMgr_C1);
	RKH_TR_FWK_PSTATE(me, &GsmMgr_C2);
	RKH_TR_FWK_STATE(me, &GsmMgr_InitializeFinal);
	RKH_TR_FWK_STATE(me, &GsmMgr_ActiveFinal);
	RKH_TR_FWK_STATE(me, &GsmMgr_SendingFinal);
	RKH_TR_FWK_STATE(me, &GsmMgr_ConfigureFinal);
	RKH_TR_FWK_SIG(evOpen);
	RKH_TR_FWK_SIG(evClose);
	RKH_TR_FWK_SIG(evSend);
	RKH_TR_FWK_SIG(evRecv);
	RKH_TR_FWK_SIG(evSendFail);
	RKH_TR_FWK_SIG(evRecvFail);
	RKH_TR_FWK_SIG(evReceived);
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
	RKH_TR_FWK_SIG(evGsmError);
	RKH_TR_FWK_SIG(evGsmReady);
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
		RKH_TR_FWK_OBJ_NAME(UnregisteredToRegisteredExt23, "UnregisteredToRegisteredExt23");
		RKH_TR_FWK_OBJ_NAME(RegisteredToRegisteredExt25, "RegisteredToRegisteredExt25");
		RKH_TR_FWK_OBJ_NAME(C1ToSocket_WaitGsmReadyExt30, "C1ToSocket_WaitGsmReadyExt30");
		RKH_TR_FWK_OBJ_NAME(ConnectingToDisconnectingExt33, "ConnectingToDisconnectingExt33");
		RKH_TR_FWK_OBJ_NAME(ConnectingToSocket_WaitGsmReadyExt34, "ConnectingToSocket_WaitGsmReadyExt34");
		RKH_TR_FWK_OBJ_NAME(ToWaitingServerExt35, "ToWaitingServerExt35");
		RKH_TR_FWK_OBJ_NAME(WaitingServerToWaitingServerExt36, "WaitingServerToWaitingServerExt36");
		RKH_TR_FWK_OBJ_NAME(ConnectedToRestartingExt38, "ConnectedToRestartingExt38");
		RKH_TR_FWK_OBJ_NAME(IdleToGetStatusExt41, "IdleToGetStatusExt41");
		RKH_TR_FWK_OBJ_NAME(IdleToSendingExt42, "IdleToSendingExt42");
		RKH_TR_FWK_OBJ_NAME(IdleToReceivingExt43, "IdleToReceivingExt43");
		RKH_TR_FWK_OBJ_NAME(ReceivingToIdleExt44, "ReceivingToIdleExt44");
		RKH_TR_FWK_OBJ_NAME(ReceivingToIdleExt45, "ReceivingToIdleExt45");
		RKH_TR_FWK_OBJ_NAME(GetStatusToIdleExt46, "GetStatusToIdleExt46");
		RKH_TR_FWK_OBJ_NAME(GetStatusToIdleExt47, "GetStatusToIdleExt47");
		RKH_TR_FWK_OBJ_NAME(SendingToIdleExt48, "SendingToIdleExt48");
		RKH_TR_FWK_OBJ_NAME(WaitOkToSendingFinalExt51, "WaitOkToSendingFinalExt51");
		RKH_TR_FWK_OBJ_NAME(WaitPromptToWaitOkExt52, "WaitPromptToWaitOkExt52");
		RKH_TR_FWK_OBJ_NAME(WaitRetryConfigToConfigureHistExt55, "WaitRetryConfigToConfigureHistExt55");
		RKH_TR_FWK_OBJ_NAME(ConfigureToSocket_WaitGsmReadyExt57, "ConfigureToSocket_WaitGsmReadyExt57");
		RKH_TR_FWK_OBJ_NAME(ConfigureToConnectingExt58, "ConfigureToConnectingExt58");
		RKH_TR_FWK_OBJ_NAME(ToGetOperExt59, "ToGetOperExt59");
		RKH_TR_FWK_OBJ_NAME(CheckIPToCheckIPExt64, "CheckIPToCheckIPExt64");
		RKH_TR_FWK_OBJ_NAME(GetOperToSetAPNExt65, "GetOperToSetAPNExt65");
		RKH_TR_FWK_OBJ_NAME(C2ToSocket_WaitGsmReadyExt67, "C2ToSocket_WaitGsmReadyExt67");
		RKH_TR_FWK_OBJ_NAME(WaitRetryConnectToConnectingExt68, "WaitRetryConnectToConnectingExt68");
		RKH_TR_FWK_OBJ_NAME(InactiveToInactiveLoc0, "InactiveToInactiveLoc0");
		RKH_TR_FWK_OBJ_NAME(InactiveToInactiveLoc1, "InactiveToInactiveLoc1");
		RKH_TR_FWK_OBJ_NAME(ActiveToActiveLoc2, "ActiveToActiveLoc2");
		RKH_TR_FWK_OBJ_NAME(ActiveToActiveLoc3, "ActiveToActiveLoc3");
		RKH_TR_FWK_OBJ_NAME(ActiveToActiveLoc4, "ActiveToActiveLoc4");
		RKH_TR_FWK_OBJ_NAME(UnregisteredToUnregisteredLoc16, "UnregisteredToUnregisteredLoc16");
		RKH_TR_FWK_OBJ_NAME(UnregisteredToUnregisteredLoc17, "UnregisteredToUnregisteredLoc17");
		RKH_TR_FWK_OBJ_NAME(RegisteredToRegisteredLoc20, "RegisteredToRegisteredLoc20");
		RKH_TR_FWK_OBJ_NAME(RegisteredToRegisteredLoc21, "RegisteredToRegisteredLoc21");
		RKH_TR_FWK_OBJ_NAME(GetStatusToGetStatusLoc25, "GetStatusToGetStatusLoc25");
		RKH_TR_FWK_OBJ_NAME(GetStatusToGetStatusLoc26, "GetStatusToGetStatusLoc26");
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
		RKH_TR_FWK_OBJ_NAME(isCondGetStatusToIdle46, "isCondGetStatusToIdle46");
		RKH_TR_FWK_OBJ_NAME(isCondC0ToSync10, "isCondC0ToSync10");
		RKH_TR_FWK_OBJ_NAME(isCondC1ToWaitRetryConnect29, "isCondC1ToWaitRetryConnect29");
		RKH_TR_FWK_OBJ_NAME(isCondC2ToWaitRetryConfig66, "isCondC2ToWaitRetryConfig66");
	#endif
	
	init(me);
	me->retryCount = 0;
	Config_getConnectionDomain(me->domain);
	Config_getConnectionPort(me->port);
}

void 
InactiveToActiveExt1(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModMgr_open();
	powerOn();
}

void 
ActiveToInactiveExt2(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModMgr_close();
	powerOff();
}

void 
ToInitializeExt4(GsmMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount = 0;
}

void 
GetImeiToCipShutdownExt18(GsmMgr *const me, RKH_EVT_T *pe)
{
	storeImei(me, pe);
}

void 
UnregisteredToRegisteredExt23(GsmMgr *const me, RKH_EVT_T *pe)
{
	gsmIsReady();
}

void 
RegisteredToRegisteredExt25(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_getRegStatus();
}

void 
C1ToSocket_WaitGsmReadyExt30(GsmMgr *const me, RKH_EVT_T *pe)
{
	gsmInError();
}

void 
ConnectingToDisconnectingExt33(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_disconnect();
}

void 
ConnectingToSocket_WaitGsmReadyExt34(GsmMgr *const me, RKH_EVT_T *pe)
{
	gsmInError();
}

void 
ToWaitingServerExt35(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_connect(me->protocol, me->domain, me->port);
}

void 
WaitingServerToWaitingServerExt36(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_getConnStatus();
}

void 
ConnectedToRestartingExt38(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_disconnect();
}

void 
IdleToGetStatusExt41(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_getConnStatus();
}

void 
IdleToSendingExt42(GsmMgr *const me, RKH_EVT_T *pe)
{
	sendRequest(pe);
}

void 
IdleToReceivingExt43(GsmMgr *const me, RKH_EVT_T *pe)
{
	readData();
}

void 
ReceivingToIdleExt44(GsmMgr *const me, RKH_EVT_T *pe)
{
	recvFail(me);
}

void 
ReceivingToIdleExt45(GsmMgr *const me, RKH_EVT_T *pe)
{
	recvOk(me, pe);
}

void 
GetStatusToIdleExt46(GsmMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount += 1;
	ModCmd_init();
}

void 
GetStatusToIdleExt47(GsmMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount = 0;
}

void 
SendingToIdleExt48(GsmMgr *const me, RKH_EVT_T *pe)
{
	sendFail(me);
}

void 
WaitOkToSendingFinalExt51(GsmMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount = 0;
	sendOk(me);
}

void 
WaitPromptToWaitOkExt52(GsmMgr *const me, RKH_EVT_T *pe)
{
	flushData();
}

void 
WaitRetryConfigToConfigureHistExt55(GsmMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount += 1;
	ModCmd_init();
}

void 
ConfigureToSocket_WaitGsmReadyExt57(GsmMgr *const me, RKH_EVT_T *pe)
{
	gsmInError();
}

void 
ConfigureToConnectingExt58(GsmMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount = 0;
}

void 
ToGetOperExt59(GsmMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount = 0;
}

void 
CheckIPToCheckIPExt64(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_requestIP();
}

void 
GetOperToSetAPNExt65(GsmMgr *const me, RKH_EVT_T *pe)
{
	storeOper(me, pe);
}

void 
C2ToSocket_WaitGsmReadyExt67(GsmMgr *const me, RKH_EVT_T *pe)
{
	gsmInError();
}

void 
WaitRetryConnectToConnectingExt68(GsmMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount += 1;
}

void 
InactiveToInactiveLoc0(GsmMgr *const me, RKH_EVT_T *pe)
{
	recvFail(me);
}

void 
InactiveToInactiveLoc1(GsmMgr *const me, RKH_EVT_T *pe)
{
	sendFail(me);
}

void 
ActiveToActiveLoc2(GsmMgr *const me, RKH_EVT_T *pe)
{
	sendFail(me);
}

void 
ActiveToActiveLoc3(GsmMgr *const me, RKH_EVT_T *pe)
{
	recvFail(me);
}

void 
ActiveToActiveLoc4(GsmMgr *const me, RKH_EVT_T *pe)
{
	setSigLevel(me, pe);
}

void 
UnregisteredToUnregisteredLoc16(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_getRegStatus();
}

void 
UnregisteredToUnregisteredLoc17(GsmMgr *const me, RKH_EVT_T *pe)
{
	startRegStatusTimer(me);
}

void 
RegisteredToRegisteredLoc20(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_getLocalTime();
}

void 
RegisteredToRegisteredLoc21(GsmMgr *const me, RKH_EVT_T *pe)
{
	rtimeSync(pe);
}

void 
GetStatusToGetStatusLoc25(GsmMgr *const me, RKH_EVT_T *pe)
{
	reqDefer(pe);
}

void 
GetStatusToGetStatusLoc26(GsmMgr *const me, RKH_EVT_T *pe)
{
	reqDefer(pe);
}

/* ............................. Entry actions ............................. */
void 
enSync(GsmMgr *const me)
{
	me->retryCount += 1;
	ModCmd_sync();
}

void 
enInit(GsmMgr *const me)
{
	modemFound();
	ModCmd_initStr();
}

void 
enPin(GsmMgr *const me)
{
	ModCmd_getPinStatus();
}

void 
enSetPin(GsmMgr *const me)
{
	ModCmd_setPin(Config_getSIMPinNumber());
}

void 
enEnableNetTime(GsmMgr *const me)
{
	ModCmd_enableNetTime();
}

void 
enGetImei(GsmMgr *const me)
{
	ModCmd_getImei();
}

void 
enSetManualGet(GsmMgr *const me)
{
	ModCmd_setManualGet();
}

void 
enCipShutdown(GsmMgr *const me)
{
	ModCmd_cipShutdown();
}

void 
enConFailure(GsmMgr *const me)
{
	powerOff();
	RKH_SET_STATIC_EVENT(&me->tmEvtObj0, evTout0);
	RKH_TMR_INIT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj0), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime0);
}

void 
enUnregistered(GsmMgr *const me)
{
	ModCmd_getRegStatus();
	RKH_SET_STATIC_EVENT(&me->tmEvtObj1, evTout1);
	RKH_TMR_INIT(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj1), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime1);
}

void 
enRegistered(GsmMgr *const me)
{
	registered();
	RKH_SET_STATIC_EVENT(&me->tmEvtObj2, evTout2);
	RKH_TMR_INIT(&me->tmEvtObj2.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj2), NULL);
	RKH_TMR_PERIODIC(&me->tmEvtObj2.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime2, WaitTime2);
}

void 
enWaitingServer(GsmMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj3, evTout3);
	RKH_TMR_INIT(&me->tmEvtObj3.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj3), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj3.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime3);
}

void 
enConnected(GsmMgr *const me)
{
	socketConnected(me);
}

void 
enIdle(GsmMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj4, evTout4);
	RKH_TMR_INIT(&me->tmEvtObj4.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj4), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj4.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime4);
}

void 
enWaitReopen(GsmMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj5, evTout5);
	RKH_TMR_INIT(&me->tmEvtObj5.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj5), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj5.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime5);
}

void 
enWaitRetryConfig(GsmMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj6, evTout6);
	RKH_TMR_INIT(&me->tmEvtObj6.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj6), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj6.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime6);
}

void 
enSetAPN(GsmMgr *const me)
{
	setupAPN(me);
}

void 
enEnableNetwork(GsmMgr *const me)
{
	ModCmd_startNetwork();
}

void 
enCheckIP(GsmMgr *const me)
{
	ModCmd_getIpStatus();
}

void 
enGetOper(GsmMgr *const me)
{
	ModCmd_getOper();
}

void 
enWaitRetryConnect(GsmMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj7, evTout7);
	RKH_TMR_INIT(&me->tmEvtObj7.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj7), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj7.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime7);
}

/* ............................. Exit actions .............................. */
void 
exConFailure(GsmMgr *const me)
{
	powerOn();
	me->retryCount = 0;
	rkh_tmr_stop(&me->tmEvtObj0.tmr);
}

void 
exUnregistered(GsmMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj1.tmr);
}

void 
exRegistered(GsmMgr *const me)
{
	unregistered();
	rkh_tmr_stop(&me->tmEvtObj2.tmr);
}

void 
exWaitingServer(GsmMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj3.tmr);
}

void 
exConnected(GsmMgr *const me)
{
	socketDisconnected(me);
}

void 
exIdle(GsmMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj4.tmr);
}

void 
exGetStatus(GsmMgr *const me)
{
	reqRecall(me);
}

void 
exWaitReopen(GsmMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj5.tmr);
}

void 
exWaitRetryConfig(GsmMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj6.tmr);
}

void 
exWaitRetryConnect(GsmMgr *const me)
{
	ModCmd_init();
	rkh_tmr_stop(&me->tmEvtObj7.tmr);
}

/* ................................ Guards ................................. */
rbool_t 
isCondGetStatusToIdle46(GsmMgr *const me, RKH_EVT_T *pe)
{
	return ((me->retryCount < 5)) ? true : false;
}

rbool_t 
isCondC0ToSync10(GsmMgr *const me, RKH_EVT_T *pe)
{
	return ((me->retryCount < 5)) ? true : false;
}

rbool_t 
isCondC1ToWaitRetryConnect29(GsmMgr *const me, RKH_EVT_T *pe)
{
	return ((me->retryCount < Config_getMaxNumConnectRetries())) ? true : false;
}

rbool_t 
isCondC2ToWaitRetryConfig66(GsmMgr *const me, RKH_EVT_T *pe)
{
	return ((me->retryCount < 5)) ? true : false;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
