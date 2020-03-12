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
typedef struct Socket Socket;
typedef struct SMS SMS;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE GsmMgr_Inactive, GsmMgr_Sync, GsmMgr_Init, GsmMgr_Pin, GsmMgr_SetPin, GsmMgr_EnableNetTime, GsmMgr_GetImei, GsmMgr_SetManualGet, GsmMgr_CipShutdown, GsmMgr_ConFailure, GsmMgr_Unregistered, GsmMgr_Registered, GsmMgr_WaitingClose, GsmMgr_WaitRetryConfig, GsmMgr_WaitRetryConnect, GsmMgr_SetAPN, GsmMgr_EnableNetwork, GsmMgr_CheckIP, GsmMgr_GetOper, GsmMgr_Disconnecting, GsmMgr_WaitingServer, GsmMgr_Idle, GsmMgr_Receiving, GsmMgr_GetStatus, GsmMgr_WaitOk, GsmMgr_WaitPrompt, GsmMgr_Restarting, GsmMgr_WaitReopen, GsmMgr_Socket_WaitGsmReady, GsmMgr_SMSReady, GsmMgr_SMS_WaitGsmReady;
RKH_DCLR_COMP_STATE GsmMgr_Active, GsmMgr_Initialize, GsmMgr_Socket_GsmReady, GsmMgr_Configure, GsmMgr_Connecting, GsmMgr_Connected, GsmMgr_Sending; //GsmMgr_Socket, GsmMgr_SMS;
RKH_DCLR_CHOICE_STATE GsmMgr_C0, GsmMgr_C1, GsmMgr_C2;
RKH_DCLR_FINAL_STATE GsmMgr_InitializeFinal, GsmMgr_ActiveFinal, GsmMgr_ConfigureFinal, GsmMgr_SendingFinal, GsmMgr_Socket_GsmReadyFinal;
RKH_DCLR_SHIST_STATE GsmMgr_ConfigureHist;

/* ........................ Declares effect actions ........................ */
static void ToInactiveExt0(GsmMgr *const me, RKH_EVT_T *pe);
static void InactiveToActiveExt1(GsmMgr *const me, RKH_EVT_T *pe);
static void ActiveToInactiveExt2(GsmMgr *const me, RKH_EVT_T *pe);
static void ToInitializeExt4(GsmMgr *const me, RKH_EVT_T *pe);
static void GetImeiToCipShutdownExt18(GsmMgr *const me, RKH_EVT_T *pe);
static void UnregisteredToRegisteredExt23(GsmMgr *const me, RKH_EVT_T *pe);
static void RegisteredToRegisteredExt25(GsmMgr *const me, RKH_EVT_T *pe);
static void C1ToSocket_GsmReadyFinalExt31(Socket *const me, RKH_EVT_T *pe);
static void C2ToSocket_GsmReadyFinalExt33(Socket *const me, RKH_EVT_T *pe);
static void WaitRetryConfigToConfigureHistExt34(Socket *const me, RKH_EVT_T *pe);
static void WaitRetryConnectToConnectingExt35(Socket *const me, RKH_EVT_T *pe);
static void ConfigureToSocket_GsmReadyFinalExt38(Socket *const me, RKH_EVT_T *pe);
static void ConfigureToConnectingExt39(Socket *const me, RKH_EVT_T *pe);
static void ToGetOperExt40(Socket *const me, RKH_EVT_T *pe);
static void CheckIPToCheckIPExt45(Socket *const me, RKH_EVT_T *pe);
static void GetOperToSetAPNExt46(Socket *const me, RKH_EVT_T *pe);
static void ConnectingToDisconnectingExt50(Socket *const me, RKH_EVT_T *pe);
static void ToWaitingServerExt51(Socket *const me, RKH_EVT_T *pe);
static void WaitingServerToWaitingServerExt52(Socket *const me, RKH_EVT_T *pe);
static void ConnectedToRestartingExt54(Socket *const me, RKH_EVT_T *pe);
static void IdleToGetStatusExt57(Socket *const me, RKH_EVT_T *pe);
static void IdleToSendingExt58(Socket *const me, RKH_EVT_T *pe);
static void IdleToReceivingExt59(Socket *const me, RKH_EVT_T *pe);
static void ReceivingToIdleExt60(Socket *const me, RKH_EVT_T *pe);
static void ReceivingToIdleExt61(Socket *const me, RKH_EVT_T *pe);
static void GetStatusToIdleExt62(Socket *const me, RKH_EVT_T *pe);
static void GetStatusToIdleExt63(Socket *const me, RKH_EVT_T *pe);
static void SendingToIdleExt64(Socket *const me, RKH_EVT_T *pe);
static void WaitOkToSendingFinalExt67(Socket *const me, RKH_EVT_T *pe);
static void ToSocket_WaitGsmReadyExt72(Socket *const me, RKH_EVT_T *pe);
static void ActiveToActiveLoc0(GsmMgr *const me, RKH_EVT_T *pe);
static void UnregisteredToUnregisteredLoc12(GsmMgr *const me, RKH_EVT_T *pe);
static void UnregisteredToUnregisteredLoc13(GsmMgr *const me, RKH_EVT_T *pe);
static void RegisteredToRegisteredLoc16(GsmMgr *const me, RKH_EVT_T *pe);
static void RegisteredToRegisteredLoc17(GsmMgr *const me, RKH_EVT_T *pe);
static void Socket_GsmReadyToSocket_GsmReadyLoc18(Socket *const me, RKH_EVT_T *pe);
static void Socket_GsmReadyToSocket_GsmReadyLoc19(Socket *const me, RKH_EVT_T *pe);
static void GetStatusToGetStatusLoc28(Socket *const me, RKH_EVT_T *pe);
static void GetStatusToGetStatusLoc29(Socket *const me, RKH_EVT_T *pe);
static void Socket_WaitGsmReadyToSocket_WaitGsmReadyLoc30(Socket *const me, RKH_EVT_T *pe);
static void Socket_WaitGsmReadyToSocket_WaitGsmReadyLoc31(Socket *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void enSync(GsmMgr *const me);
static void enInit(GsmMgr *const me);
static void enPin(GsmMgr *const me);
static void enSetPin(GsmMgr *const me);
static void enEnableNetTime(GsmMgr *const me);
static void enGetImei(GsmMgr *const me);
static void enSetManualGet(GsmMgr *const me);
static void enCipShutdown(GsmMgr *const me);
static void enConFailure(GsmMgr *const me);
static void enUnregistered(GsmMgr *const me);
static void enRegistered(GsmMgr *const me);
static void enWaitRetryConfig(Socket *const me);
static void enWaitRetryConnect(Socket *const me);
static void enSetAPN(Socket *const me);
static void enEnableNetwork(Socket *const me);
static void enCheckIP(Socket *const me);
static void enGetOper(Socket *const me);
static void enWaitingServer(Socket *const me);
static void enConnected(Socket *const me);
static void enIdle(Socket *const me);
static void enWaitReopen(Socket *const me);

/* ......................... Declares exit actions ......................... */
static void exConFailure(GsmMgr *const me);
static void exUnregistered(GsmMgr *const me);
static void exRegistered(GsmMgr *const me);
static void exWaitRetryConfig(Socket *const me);
static void exWaitRetryConnect(Socket *const me);
static void exWaitingServer(Socket *const me);
static void exConnected(Socket *const me);
static void exIdle(Socket *const me);
static void exGetStatus(Socket *const me);
static void exWaitReopen(Socket *const me);

/* ............................ Declares guards ............................ */
static rbool_t isCondGetStatusToIdle62(Socket *const me, RKH_EVT_T *pe);
static rbool_t isCondC0ToSync10(GsmMgr *const me, RKH_EVT_T *pe);
static rbool_t isCondC1ToWaitRetryConfig30(Socket *const me, RKH_EVT_T *pe);
static rbool_t isCondC2ToWaitRetryConnect32(Socket *const me, RKH_EVT_T *pe);

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
RKH_CREATE_BASIC_STATE(GsmMgr_WaitRetryConfig, enWaitRetryConfig, exWaitRetryConfig, &GsmMgr_Socket_GsmReady, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_WaitRetryConnect, enWaitRetryConnect, exWaitRetryConnect, &GsmMgr_Socket_GsmReady, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_SetAPN, enSetAPN, NULL, &GsmMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_EnableNetwork, enEnableNetwork, NULL, &GsmMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_CheckIP, enCheckIP, NULL, &GsmMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_GetOper, enGetOper, NULL, &GsmMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Disconnecting, NULL, NULL, &GsmMgr_Socket_GsmReady, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_WaitingServer, enWaitingServer, exWaitingServer, &GsmMgr_Connecting, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Idle, enIdle, exIdle, &GsmMgr_Connected, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Receiving, NULL, NULL, &GsmMgr_Connected, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_GetStatus, NULL, exGetStatus, &GsmMgr_Connected, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_WaitOk, NULL, NULL, &GsmMgr_Sending, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_WaitPrompt, NULL, NULL, &GsmMgr_Sending, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Restarting, NULL, NULL, &GsmMgr_Connecting, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_WaitReopen, enWaitReopen, exWaitReopen, &GsmMgr_Connecting, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_Socket_WaitGsmReady, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_SMSReady, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_BASIC_STATE(GsmMgr_SMS_WaitGsmReady, NULL, NULL, RKH_ROOT, NULL);

RKH_CREATE_COMP_REGION_STATE(GsmMgr_Active, NULL, NULL, RKH_ROOT, &GsmMgr_Initialize, ToInitializeExt4, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(GsmMgr_Initialize, NULL, NULL, &GsmMgr_Active, &GsmMgr_Sync, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(GsmMgr_Socket_GsmReady, NULL, NULL, RKH_ROOT, &GsmMgr_Configure, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_HISTORY_STORAGE(GsmMgr_Configure);
RKH_CREATE_COMP_REGION_STATE(GsmMgr_Configure, NULL, NULL, &GsmMgr_Socket_GsmReady, &GsmMgr_GetOper, ToGetOperExt40, RKH_SHISTORY, NULL, NULL, NULL, RKH_GET_HISTORY_STORAGE(GsmMgr_Configure));
RKH_CREATE_COMP_REGION_STATE(GsmMgr_Connecting, NULL, NULL, &GsmMgr_Socket_GsmReady, &GsmMgr_WaitingServer, ToWaitingServerExt51, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(GsmMgr_Connected, enConnected, exConnected, &GsmMgr_Connecting, &GsmMgr_Idle, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(GsmMgr_Sending, NULL, NULL, &GsmMgr_Connected, &GsmMgr_WaitPrompt, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);

RKH_CREATE_TRANS_TABLE(GsmMgr_Inactive)
	RKH_TRREG(evOpen, NULL, InactiveToActiveExt1, &GsmMgr_Active),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Active)
	RKH_TRREG(evClose, NULL, ActiveToInactiveExt2, &GsmMgr_Inactive),
	RKH_TRCOMPLETION(NULL, NULL, &GsmMgr_Inactive),
	RKH_TRINT(evSigLevel, NULL, ActiveToActiveLoc0),
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
	RKH_TRINT(evRegStatusTout, NULL, UnregisteredToUnregisteredLoc12),
	RKH_TRINT(evNoReg, NULL, UnregisteredToUnregisteredLoc13),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Registered)
	RKH_TRREG(evNoReg, NULL, NULL, &GsmMgr_Unregistered),
	RKH_TRREG(evTout2, NULL, RegisteredToRegisteredExt25, &GsmMgr_Registered),
	RKH_TRREG(evClose, NULL, NULL, &GsmMgr_WaitingClose),
	RKH_TRREG(evGsmError, NULL, NULL, &GsmMgr_ConFailure),
	RKH_TRINT(evNetClockSync, NULL, RegisteredToRegisteredLoc16),
	RKH_TRINT(evLocalTime, NULL, RegisteredToRegisteredLoc17),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitingClose)
	RKH_TRREG(evDisconnected, NULL, NULL, &GsmMgr_ActiveFinal),
	RKH_TRREG(evNoResponse, NULL, NULL, &GsmMgr_ActiveFinal),
	RKH_TRREG(evGsmError, NULL, NULL, &GsmMgr_ActiveFinal),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Socket_GsmReady)
    RKH_TRCOMPLETION(NULL, NULL, &GsmMgr_Socket_WaitGsmReady),
    RKH_TRINT(evSend, NULL, Socket_GsmReadyToSocket_GsmReadyLoc18),
    RKH_TRINT(evRecv, NULL, Socket_GsmReadyToSocket_GsmReadyLoc19),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitRetryConfig)
    RKH_TRREG(evTout3, NULL, WaitRetryConfigToConfigureHistExt34, &GsmMgr_ConfigureHist),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitRetryConnect)
    RKH_TRREG(evTout4, NULL, WaitRetryConnectToConnectingExt35, &GsmMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Configure)
    RKH_TRREG(evNoResponse, NULL, NULL, &GsmMgr_C1),
    RKH_TRREG(evError, NULL, ConfigureToSocket_GsmReadyFinalExt38, &GsmMgr_Socket_GsmReadyFinal),
    RKH_TRCOMPLETION(NULL, ConfigureToConnectingExt39, &GsmMgr_Connecting),
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
    RKH_TRREG(evIPGprsAct, NULL, CheckIPToCheckIPExt45, &GsmMgr_CheckIP),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_GetOper)
    RKH_TRREG(evOper, NULL, GetOperToSetAPNExt46, &GsmMgr_SetAPN),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Disconnecting)
    RKH_TRREG(evDisconnected, NULL, NULL, &GsmMgr_Socket_GsmReadyFinal),
    RKH_TRREG(evNoResponse, NULL, NULL, &GsmMgr_Socket_GsmReadyFinal),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Connecting)
    RKH_TRREG(evNoResponse, NULL, NULL, &GsmMgr_C2),
    RKH_TRREG(evClosed, NULL, NULL, &GsmMgr_C2),
    RKH_TRREG(evIPStatus, NULL, NULL, &GsmMgr_C2),
    RKH_TRREG(evError, NULL, NULL, &GsmMgr_C2),
    RKH_TRREG(evIPStart, NULL, NULL, &GsmMgr_Configure),
    RKH_TRREG(evIPInitial, NULL, NULL, &GsmMgr_Configure),
    RKH_TRREG(evIPGprsAct, NULL, NULL, &GsmMgr_Configure),
    RKH_TRREG(evClose, NULL, ConnectingToDisconnectingExt50, &GsmMgr_Disconnecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitingServer)
    RKH_TRREG(evTout5, NULL, WaitingServerToWaitingServerExt52, &GsmMgr_WaitingServer),
    RKH_TRREG(evConnected, NULL, NULL, &GsmMgr_Connected),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Connected)
    RKH_TRREG(evRestart, NULL, ConnectedToRestartingExt54, &GsmMgr_Restarting),
    RKH_TRREG(evClosed, NULL, NULL, &GsmMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Idle)
    RKH_TRREG(evTout6, NULL, IdleToGetStatusExt57, &GsmMgr_GetStatus),
    RKH_TRREG(evSend, NULL, IdleToSendingExt58, &GsmMgr_Sending),
    RKH_TRREG(evRecv, NULL, IdleToReceivingExt59, &GsmMgr_Receiving),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Receiving)
    RKH_TRREG(evNoResponse, NULL, ReceivingToIdleExt60, &GsmMgr_Idle),
    RKH_TRREG(evError, NULL, ReceivingToIdleExt60, &GsmMgr_Idle),
    RKH_TRREG(evReceived, NULL, ReceivingToIdleExt61, &GsmMgr_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_GetStatus)
    RKH_TRREG(evNoResponse, isCondGetStatusToIdle62, GetStatusToIdleExt62, &GsmMgr_Idle),
    RKH_TRREG(evConnected, NULL, GetStatusToIdleExt63, &GsmMgr_Idle),
    RKH_TRINT(evSend, NULL, GetStatusToGetStatusLoc28),
    RKH_TRINT(evRecv, NULL, GetStatusToGetStatusLoc29),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Sending)
    RKH_TRREG(evNoResponse, NULL, SendingToIdleExt64, &GsmMgr_Idle),
    RKH_TRREG(evError, NULL, SendingToIdleExt64, &GsmMgr_Idle),
    RKH_TRCOMPLETION(NULL, NULL, &GsmMgr_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitOk)
    RKH_TRREG(evOk, NULL, WaitOkToSendingFinalExt67, &GsmMgr_SendingFinal),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitPrompt)
    RKH_TRREG(evOk, NULL, NULL, &GsmMgr_WaitOk),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Restarting)
    RKH_TRREG(evDisconnected, NULL, NULL, &GsmMgr_WaitReopen),
    RKH_TRREG(evNoResponse, NULL, NULL, &GsmMgr_WaitReopen),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_WaitReopen)
    RKH_TRREG(evTout7, NULL, NULL, &GsmMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_Socket_WaitGsmReady)
    RKH_TRREG(evGsmReady, NULL, NULL, &GsmMgr_Socket_GsmReady),
    RKH_TRINT(evRecv, NULL, Socket_WaitGsmReadyToSocket_WaitGsmReadyLoc30),
    RKH_TRINT(evSend, NULL, Socket_WaitGsmReadyToSocket_WaitGsmReadyLoc31),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_SMSReady)
    RKH_TRREG(evClose, NULL, NULL, &GsmMgr_SMS_WaitGsmReady),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(GsmMgr_SMS_WaitGsmReady)
    RKH_TRREG(evGsmReady, NULL, NULL, &GsmMgr_SMSReady),
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(GsmMgr_C0);
RKH_CREATE_CHOICE_STATE(GsmMgr_C1);
RKH_CREATE_CHOICE_STATE(GsmMgr_C2);

RKH_CREATE_BRANCH_TABLE(GsmMgr_C0)
	RKH_BRANCH(isCondC0ToSync10, NULL, &GsmMgr_Sync),
	RKH_BRANCH(ELSE, NULL, &GsmMgr_InitializeFinal),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(GsmMgr_C1)
    RKH_BRANCH(isCondC1ToWaitRetryConfig30, NULL, &GsmMgr_WaitRetryConfig),
    RKH_BRANCH(ELSE, C1ToSocket_GsmReadyFinalExt31, &GsmMgr_Socket_GsmReadyFinal),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(GsmMgr_C2)
    RKH_BRANCH(isCondC2ToWaitRetryConnect32, NULL, &GsmMgr_WaitRetryConnect),
    RKH_BRANCH(ELSE, C2ToSocket_GsmReadyFinalExt33, &GsmMgr_Socket_GsmReadyFinal),
RKH_END_BRANCH_TABLE


/* ............................. Active object ............................. */
struct Socket
{
    RKH_SM_T sm;        /* Orthogonal reagion */
    GsmMgr *itsGsmMgr;
    RKHTmEvt tmEvtObj3;
    RKHTmEvt tmEvtObj4;
    RKHTmEvt tmEvtObj5;
    RKHTmEvt tmEvtObj6;
    RKHTmEvt tmEvtObj7;
    char * oper;
    char * protocol;
    char * domain;
    char * port;
    rInt retryCount;
};

struct SMS
{
    RKH_SM_T sm;        /* Orthogonal reagion */
    GsmMgr *itsGsmMgr;
};

struct GsmMgr
{
    RKH_SMA_T sma;      /* base structure */
    RKHSmaVtbl vtbl;    /* Virtual table */
    SMS     itsSMS;
    Socket  itsSocket;
    RKHTmEvt tmEvtObj0;
    RKHTmEvt tmEvtObj1;
    RKHTmEvt tmEvtObj2;
    rInt sigLevel;
    char * imei;
    rInt retryCount;
};

RKH_SMA_CREATE(GsmMgr, gsmMgr, 1, HCAL, &GsmMgr_Inactive, ToInactiveExt0, NULL);
RKH_SMA_DEF_PTR(gsmMgr);

RKH_SM_CONST_CREATE(socket, 2, HCAL, &GsmMgr_Socket_WaitGsmReady, ToSocket_WaitGsmReadyExt72, NULL);
RKH_SM_CONST_CREATE(sms, 3, HCAL, &GsmMgr_SMS_WaitGsmReady, NULL, NULL);

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
RKH_DCLR_SM_CONST_GLOBAL(socket);
RKH_SM_T *gsmMgrSocket;

RKH_DCLR_SM_CONST_GLOBAL(sms);
RKH_SM_T *gsmMgrSMS;

/* ---------------------------- Local variables ---------------------------- */
static RKH_ROM_STATIC_EVENT(e_GsmReady, evGsmReady);
static RKH_ROM_STATIC_EVENT(e_GsmError, evGsmError);
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
static void
dispatch(RKH_SMA_T *me, void *arg)
{
    Socket *socket;
    SMS *sms;

    socket = &(RKH_DOWNCAST(GsmMgr, me)->itsSocket);
    sms = &(RKH_DOWNCAST(GsmMgr, me)->itsSMS);

    rkh_sm_dispatch((RKH_SM_T *)me, (RKH_EVT_T *)arg);

    //if(me->sm.state->parent == RKH_UPCAST(RKH_ST_T, &GsmMgr_Active))
    {
    	rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, socket), (RKH_EVT_T *)arg);
    	rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, sms), (RKH_EVT_T *)arg);
    }
}

static void
initGsmServices(GsmMgr *me)
{
    rkh_sm_init(RKH_UPCAST(RKH_SM_T, &me->itsSocket));
    rkh_sm_init(RKH_UPCAST(RKH_SM_T, &me->itsSMS));
}

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
    topic_subscribe(TCPConnection, RKH_UPCAST(RKH_SMA_T, me));
    topic_subscribe(ModURC, RKH_UPCAST(RKH_SMA_T, me));

    rkh_queue_init(&GsmMgrInt.qDefer, (const void **)GsmMgrInt.qDefer_sto, 
                    SIZEOF_QDEFER, CV(0));

    GsmMgrInt.pGsmMgr = me;

    me->imei = GsmMgrInt.Imei;
    me->itsSocket.oper = GsmMgrInt.Oper;
    me->itsSocket.protocol = CONNECTION_PROT;
    me->itsSocket.domain = GsmMgrInt.Domain;
    me->itsSocket.port = GsmMgrInt.Port;

    bsp_SIMSelect(MainSIM);

    RKH_FILTER_OFF_SMA(gsmMgrSocket);
    RKH_FILTER_OFF_SMA(gsmMgrSMS);

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
storeOper(Socket *const me, RKH_EVT_T *pe)
{
    OperEvt *p;

    p = RKH_UPCAST(OperEvt, pe);
    strcpy(me->oper, p->buf);
}

static void
setupAPN(Socket *const me)
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
sendData(RKH_EVT_T *pe)
{
    GsmMgrInt.psend = RKH_UPCAST(SendEvt, pe);

    ModCmd_sendDataRequest((rui16_t)(GsmMgrInt.psend->size));
    ModCmd_sendData(GsmMgrInt.psend->buf, GsmMgrInt.psend->size);
}

static void
sendOk(Socket *const me)
{
    topic_publish(TCPConnection, &e_Sent, RKH_UPCAST(RKH_SMA_T, me));
}

static void
sendFail(Socket *const me)
{
    topic_publish(TCPConnection, &e_SendFail, RKH_UPCAST(RKH_SMA_T, me));

    ModCmd_init();
}

static void
readData(void)
{
    ModCmd_readData();
}

static void
recvOk(Socket *const me, RKH_EVT_T *pe)
{
    topic_publish(TCPConnection, pe, RKH_UPCAST(RKH_SMA_T, me));

    bsp_recvOk();
}

static void
recvFail(Socket *const me)
{
    topic_publish(TCPConnection, &e_RecvFail, RKH_UPCAST(RKH_SMA_T, me));

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
reqRecall(Socket *const me)
{
    rkh_sma_recall((RKH_SMA_T *)me, &GsmMgrInt.qDefer);
}

static void
socketConnected(Socket *const me)
{
    topic_publish(TCPConnection, &e_NetConnected, RKH_UPCAST(RKH_SMA_T, me));

    bsp_netStatus(ConnectedSt);
}

static void
socketDisconnected(Socket *const me)
{
    topic_publish(TCPConnection, &e_NetDisconnected, RKH_UPCAST(RKH_SMA_T, me));

    bsp_netStatus(DisconnectedSt);
}

static void
gsmIsReady(GsmMgr *const me)
{
    RKH_SMA_POST_FIFO(gsmMgr, &e_GsmReady, RKH_UPCAST(RKH_SMA_T, me));
}

static void
gsmInError(void *const me)
{
    RKH_SMA_POST_FIFO(gsmMgr, &e_GsmError, RKH_UPCAST(RKH_SMA_T, me));
}

/* ............................ Effect actions ............................. */
static void 
ToInactiveExt0(GsmMgr *const me, RKH_EVT_T *pe)
{
	me->sigLevel = 0;
	me->imei = "";

	me->itsSocket.oper = "";
	me->itsSocket.protocol = "";
	me->itsSocket.domain = "";
	me->itsSocket.port = "";
	me->itsSocket.retryCount = 0;
		
	RKH_TR_FWK_AO(me);
	RKH_TR_FWK_OBJ(&me->itsSocket);
	RKH_TR_FWK_OBJ(&me->itsSMS);
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
    RKH_TR_FWK_STATE(me, &GsmMgr_Socket_GsmReady);
    RKH_TR_FWK_STATE(me, &GsmMgr_WaitRetryConfig);
    RKH_TR_FWK_STATE(me, &GsmMgr_WaitRetryConnect);
    RKH_TR_FWK_STATE(me, &GsmMgr_Configure);
    RKH_TR_FWK_STATE(me, &GsmMgr_SetAPN);
    RKH_TR_FWK_STATE(me, &GsmMgr_EnableNetwork);
    RKH_TR_FWK_STATE(me, &GsmMgr_CheckIP);
    RKH_TR_FWK_STATE(me, &GsmMgr_GetOper);
    RKH_TR_FWK_STATE(me, &GsmMgr_Disconnecting);
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
	RKH_TR_FWK_STATE(me, &GsmMgr_Socket_WaitGsmReady);
	RKH_TR_FWK_STATE(me, &GsmMgr_SMSReady);
	RKH_TR_FWK_STATE(me, &GsmMgr_SMS_WaitGsmReady);
	RKH_TR_FWK_PSTATE(me, &GsmMgr_ConfigureHist);
	RKH_TR_FWK_PSTATE(me, &GsmMgr_C0);
	RKH_TR_FWK_PSTATE(me, &GsmMgr_C1);
	RKH_TR_FWK_PSTATE(me, &GsmMgr_C2);
	RKH_TR_FWK_STATE(me, &GsmMgr_InitializeFinal);
	RKH_TR_FWK_STATE(me, &GsmMgr_ActiveFinal);
	RKH_TR_FWK_STATE(me, &GsmMgr_ConfigureFinal);
	RKH_TR_FWK_STATE(me, &GsmMgr_SendingFinal);
    RKH_TR_FWK_STATE(me, &GsmMgr_Socket_GsmReadyFinal);
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
	RKH_TR_FWK_SIG(evTout0);
	RKH_TR_FWK_SIG(evTout1);
	RKH_TR_FWK_SIG(evTout2);
	RKH_TR_FWK_SIG(evTout3);
	RKH_TR_FWK_SIG(evTout4);
	RKH_TR_FWK_SIG(evTout5);
	RKH_TR_FWK_SIG(evTout6);
	RKH_TR_FWK_SIG(evTout7);
	RKH_TR_FWK_TIMER(&me->tmEvtObj0.tmr);
	RKH_TR_FWK_TIMER(&me->tmEvtObj1.tmr);
	RKH_TR_FWK_TIMER(&me->tmEvtObj2.tmr);
	RKH_TR_FWK_TIMER(&me->itsSocket.tmEvtObj3.tmr);
	RKH_TR_FWK_TIMER(&me->itsSocket.tmEvtObj4.tmr);
	RKH_TR_FWK_TIMER(&me->itsSocket.tmEvtObj5.tmr);
	RKH_TR_FWK_TIMER(&me->itsSocket.tmEvtObj6.tmr);
	RKH_TR_FWK_TIMER(&me->itsSocket.tmEvtObj7.tmr);
	#if 0
        RKH_TR_FWK_OBJ_NAME(ToInactiveExt0, "ToInactiveExt0");
        RKH_TR_FWK_OBJ_NAME(InactiveToActiveExt1, "InactiveToActiveExt1");
        RKH_TR_FWK_OBJ_NAME(ActiveToInactiveExt2, "ActiveToInactiveExt2");
        RKH_TR_FWK_OBJ_NAME(ToInitializeExt4, "ToInitializeExt4");
        RKH_TR_FWK_OBJ_NAME(GetImeiToCipShutdownExt18, "GetImeiToCipShutdownExt18");
        RKH_TR_FWK_OBJ_NAME(UnregisteredToRegisteredExt23, "UnregisteredToRegisteredExt23");
        RKH_TR_FWK_OBJ_NAME(RegisteredToRegisteredExt25, "RegisteredToRegisteredExt25");
        RKH_TR_FWK_OBJ_NAME(C1ToSocket_GsmReadyFinalExt31, "C1ToSocket_GsmReadyFinalExt31");
        RKH_TR_FWK_OBJ_NAME(C2ToSocket_GsmReadyFinalExt33, "C2ToSocket_GsmReadyFinalExt33");
        RKH_TR_FWK_OBJ_NAME(WaitRetryConfigToConfigureHistExt34, "WaitRetryConfigToConfigureHistExt34");
        RKH_TR_FWK_OBJ_NAME(WaitRetryConnectToConnectingExt35, "WaitRetryConnectToConnectingExt35");
        RKH_TR_FWK_OBJ_NAME(ConfigureToSocket_GsmReadyFinalExt38, "ConfigureToSocket_GsmReadyFinalExt38");
        RKH_TR_FWK_OBJ_NAME(ConfigureToConnectingExt39, "ConfigureToConnectingExt39");
        RKH_TR_FWK_OBJ_NAME(ToGetOperExt40, "ToGetOperExt40");
        RKH_TR_FWK_OBJ_NAME(CheckIPToCheckIPExt45, "CheckIPToCheckIPExt45");
        RKH_TR_FWK_OBJ_NAME(GetOperToSetAPNExt46, "GetOperToSetAPNExt46");
        RKH_TR_FWK_OBJ_NAME(ConnectingToDisconnectingExt50, "ConnectingToDisconnectingExt50");
        RKH_TR_FWK_OBJ_NAME(ToWaitingServerExt51, "ToWaitingServerExt51");
        RKH_TR_FWK_OBJ_NAME(WaitingServerToWaitingServerExt52, "WaitingServerToWaitingServerExt52");
        RKH_TR_FWK_OBJ_NAME(ConnectedToRestartingExt54, "ConnectedToRestartingExt54");
        RKH_TR_FWK_OBJ_NAME(IdleToGetStatusExt57, "IdleToGetStatusExt57");
        RKH_TR_FWK_OBJ_NAME(IdleToSendingExt58, "IdleToSendingExt58");
        RKH_TR_FWK_OBJ_NAME(IdleToReceivingExt59, "IdleToReceivingExt59");
        RKH_TR_FWK_OBJ_NAME(ReceivingToIdleExt60, "ReceivingToIdleExt60");
        RKH_TR_FWK_OBJ_NAME(ReceivingToIdleExt61, "ReceivingToIdleExt61");
        RKH_TR_FWK_OBJ_NAME(GetStatusToIdleExt62, "GetStatusToIdleExt62");
        RKH_TR_FWK_OBJ_NAME(GetStatusToIdleExt63, "GetStatusToIdleExt63");
        RKH_TR_FWK_OBJ_NAME(SendingToIdleExt64, "SendingToIdleExt64");
        RKH_TR_FWK_OBJ_NAME(WaitOkToSendingFinalExt67, "WaitOkToSendingFinalExt67");
        RKH_TR_FWK_OBJ_NAME(ToSocket_WaitGsmReadyExt72, "ToSocket_WaitGsmReadyExt72");
        RKH_TR_FWK_OBJ_NAME(ActiveToActiveLoc0, "ActiveToActiveLoc0");
        RKH_TR_FWK_OBJ_NAME(UnregisteredToUnregisteredLoc12, "UnregisteredToUnregisteredLoc12");
        RKH_TR_FWK_OBJ_NAME(UnregisteredToUnregisteredLoc13, "UnregisteredToUnregisteredLoc13");
        RKH_TR_FWK_OBJ_NAME(RegisteredToRegisteredLoc16, "RegisteredToRegisteredLoc16");
        RKH_TR_FWK_OBJ_NAME(RegisteredToRegisteredLoc17, "RegisteredToRegisteredLoc17");
        RKH_TR_FWK_OBJ_NAME(Socket_GsmReadyToSocket_GsmReadyLoc18, "Socket_GsmReadyToSocket_GsmReadyLoc18");
        RKH_TR_FWK_OBJ_NAME(Socket_GsmReadyToSocket_GsmReadyLoc19, "Socket_GsmReadyToSocket_GsmReadyLoc19");
        RKH_TR_FWK_OBJ_NAME(GetStatusToGetStatusLoc28, "GetStatusToGetStatusLoc28");
        RKH_TR_FWK_OBJ_NAME(GetStatusToGetStatusLoc29, "GetStatusToGetStatusLoc29");
        RKH_TR_FWK_OBJ_NAME(Socket_WaitGsmReadyToSocket_WaitGsmReadyLoc30, "Socket_WaitGsmReadyToSocket_WaitGsmReadyLoc30");
        RKH_TR_FWK_OBJ_NAME(Socket_WaitGsmReadyToSocket_WaitGsmReadyLoc31, "Socket_WaitGsmReadyToSocket_WaitGsmReadyLoc31");
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
        RKH_TR_FWK_OBJ_NAME(enSetAPN, "enSetAPN");
        RKH_TR_FWK_OBJ_NAME(enEnableNetwork, "enEnableNetwork");
        RKH_TR_FWK_OBJ_NAME(enCheckIP, "enCheckIP");
        RKH_TR_FWK_OBJ_NAME(enGetOper, "enGetOper");
        RKH_TR_FWK_OBJ_NAME(enConnected, "enConnected");
        RKH_TR_FWK_OBJ_NAME(exConFailure, "exConFailure");
        RKH_TR_FWK_OBJ_NAME(exRegistered, "exRegistered");
        RKH_TR_FWK_OBJ_NAME(exWaitRetryConnect, "exWaitRetryConnect");
        RKH_TR_FWK_OBJ_NAME(exConnected, "exConnected");
        RKH_TR_FWK_OBJ_NAME(exGetStatus, "exGetStatus");
        RKH_TR_FWK_OBJ_NAME(isCondGetStatusToIdle62, "isCondGetStatusToIdle62");
        RKH_TR_FWK_OBJ_NAME(isCondC0ToSync10, "isCondC0ToSync10");
        RKH_TR_FWK_OBJ_NAME(isCondC1ToWaitRetryConfig30, "isCondC1ToWaitRetryConfig30");
        RKH_TR_FWK_OBJ_NAME(isCondC2ToWaitRetryConnect32, "isCondC2ToWaitRetryConnect32");
	#endif
	
	init(me);
	me->retryCount = 0;
}

static void 
InactiveToActiveExt1(GsmMgr *const me, RKH_EVT_T *pe)
{
    ModMgr_open();
    powerOn();
    initGsmServices(me);
}

static void 
ActiveToInactiveExt2(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModMgr_close();
	powerOff();
}

static void 
ToInitializeExt4(GsmMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount = 0;
}

static void 
GetImeiToCipShutdownExt18(GsmMgr *const me, RKH_EVT_T *pe)
{
	storeImei(me, pe);
}

static void 
UnregisteredToRegisteredExt23(GsmMgr *const me, RKH_EVT_T *pe)
{
    gsmIsReady(me);
}

static void 
RegisteredToRegisteredExt25(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_getRegStatus();
}

static void 
C1ToSocket_GsmReadyFinalExt31(Socket *const me, RKH_EVT_T *pe)
{
    gsmInError(me);
}

static void 
C2ToSocket_GsmReadyFinalExt33(Socket *const me, RKH_EVT_T *pe)
{
    gsmInError(me);
}

static void 
WaitRetryConfigToConfigureHistExt34(Socket *const me, RKH_EVT_T *pe)
{
    me->retryCount += 1;
    ModCmd_init();
}

static void 
WaitRetryConnectToConnectingExt35(Socket *const me, RKH_EVT_T *pe)
{
    me->retryCount += 1;
}

static void 
ConfigureToSocket_GsmReadyFinalExt38(Socket *const me, RKH_EVT_T *pe)
{
    gsmInError(me);
}

static void 
ConfigureToConnectingExt39(Socket *const me, RKH_EVT_T *pe)
{
    me->retryCount = 0;
}

static void 
ToGetOperExt40(Socket *const me, RKH_EVT_T *pe)
{
    me->retryCount = 0;
}

static void 
CheckIPToCheckIPExt45(Socket *const me, RKH_EVT_T *pe)
{
    ModCmd_requestIP();
}

static void 
GetOperToSetAPNExt46(Socket *const me, RKH_EVT_T *pe)
{
    storeOper(me, pe);
}

static void 
ConnectingToDisconnectingExt50(Socket *const me, RKH_EVT_T *pe)
{
    ModCmd_disconnect();
}

static void 
ToWaitingServerExt51(Socket *const me, RKH_EVT_T *pe)
{
	ModCmd_connect(me->protocol, me->domain, me->port);
}

static void 
WaitingServerToWaitingServerExt52(Socket *const me, RKH_EVT_T *pe)
{
	ModCmd_getConnStatus();
}

static void 
ConnectedToRestartingExt54(Socket *const me, RKH_EVT_T *pe)
{
	ModCmd_disconnect();
}

static void 
IdleToGetStatusExt57(Socket *const me, RKH_EVT_T *pe)
{
	ModCmd_getConnStatus();
}

static void 
IdleToSendingExt58(Socket *const me, RKH_EVT_T *pe)
{
	sendData(pe);
}

static void 
IdleToReceivingExt59(Socket *const me, RKH_EVT_T *pe)
{
	readData();
}

static void 
ReceivingToIdleExt60(Socket *const me, RKH_EVT_T *pe)
{
	recvFail(me);
}

static void 
ReceivingToIdleExt61(Socket *const me, RKH_EVT_T *pe)
{
	recvOk(me, pe);
}

static void 
GetStatusToIdleExt62(Socket *const me, RKH_EVT_T *pe)
{
	me->retryCount += 1;
	ModCmd_init();
}

static void 
GetStatusToIdleExt63(Socket *const me, RKH_EVT_T *pe)
{
	me->retryCount = 0;
}

static void 
SendingToIdleExt64(Socket *const me, RKH_EVT_T *pe)
{
	sendFail(me);
}

static void 
WaitOkToSendingFinalExt67(Socket *const me, RKH_EVT_T *pe)
{
	me->retryCount = 0;
	sendOk(me);
}

static void 
ToSocket_WaitGsmReadyExt72(Socket *const me, RKH_EVT_T *pe)
{
    Config_getConnectionDomain(me->domain);
    Config_getConnectionPort(me->port);
}

static void 
ActiveToActiveLoc0(GsmMgr *const me, RKH_EVT_T *pe)
{
	setSigLevel(me, pe);
}

static void 
UnregisteredToUnregisteredLoc12(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_getRegStatus();
}

static void 
UnregisteredToUnregisteredLoc13(GsmMgr *const me, RKH_EVT_T *pe)
{
	startRegStatusTimer(me);
}

static void 
RegisteredToRegisteredLoc16(GsmMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_getLocalTime();
}

static void 
RegisteredToRegisteredLoc17(GsmMgr *const me, RKH_EVT_T *pe)
{
	rtimeSync(pe);
}

static void 
Socket_GsmReadyToSocket_GsmReadyLoc18(Socket *const me, RKH_EVT_T *pe)
{
    sendFail(me);
}

static void 
Socket_GsmReadyToSocket_GsmReadyLoc19(Socket *const me, RKH_EVT_T *pe)
{
    recvFail(me);
}

static void 
GetStatusToGetStatusLoc28(Socket *const me, RKH_EVT_T *pe)
{
	reqDefer(pe);
}

static void 
GetStatusToGetStatusLoc29(Socket *const me, RKH_EVT_T *pe)
{
	reqDefer(pe);
}

static void 
Socket_WaitGsmReadyToSocket_WaitGsmReadyLoc30(Socket *const me, RKH_EVT_T *pe)
{
    recvFail(me);
}

static void 
Socket_WaitGsmReadyToSocket_WaitGsmReadyLoc31(Socket *const me, RKH_EVT_T *pe)
{
    sendFail(me);
}

/* ............................. Entry actions ............................. */
static void 
enSync(GsmMgr *const me)
{
	me->retryCount += 1;
	ModCmd_sync();
}

static void 
enInit(GsmMgr *const me)
{
	modemFound();
	ModCmd_initStr();
}

static void 
enPin(GsmMgr *const me)
{
	ModCmd_getPinStatus();
}

static void 
enSetPin(GsmMgr *const me)
{
	ModCmd_setPin(Config_getSIMPinNumber());
}

static void 
enEnableNetTime(GsmMgr *const me)
{
	ModCmd_enableNetTime();
}

static void 
enGetImei(GsmMgr *const me)
{
	ModCmd_getImei();
}

static void 
enSetManualGet(GsmMgr *const me)
{
	ModCmd_setManualGet();
}

static void 
enCipShutdown(GsmMgr *const me)
{
	ModCmd_cipShutdown();
}

static void 
enConFailure(GsmMgr *const me)
{
	powerOff();
	RKH_SET_STATIC_EVENT(&me->tmEvtObj0, evTout0);
	RKH_TMR_INIT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj0), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime0);
}

static void 
enUnregistered(GsmMgr *const me)
{
	ModCmd_getRegStatus();
	RKH_SET_STATIC_EVENT(&me->tmEvtObj1, evTout1);
	RKH_TMR_INIT(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj1), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime1);
}

static void 
enRegistered(GsmMgr *const me)
{
	registered();
	RKH_SET_STATIC_EVENT(&me->tmEvtObj2, evTout2);
	RKH_TMR_INIT(&me->tmEvtObj2.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj2), NULL);
	RKH_TMR_PERIODIC(&me->tmEvtObj2.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime2, WaitTime2);
}

static void 
enWaitRetryConfig(Socket *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj3, evTout3);
	RKH_TMR_INIT(&me->tmEvtObj3.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj3), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj3.tmr, RKH_UPCAST(RKH_SMA_T, me->itsGsmMgr), WaitTime3);
}

static void 
enWaitRetryConnect(Socket *const me)
{
    RKH_SET_STATIC_EVENT(&me->tmEvtObj4, evTout4);
    RKH_TMR_INIT(&me->tmEvtObj4.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj4), NULL);
    RKH_TMR_ONESHOT(&me->tmEvtObj4.tmr, RKH_UPCAST(RKH_SMA_T, me->itsGsmMgr), WaitTime4);
}

static void 
enSetAPN(Socket *const me)
{
    setupAPN(me);
}

static void 
enEnableNetwork(Socket *const me)
{
    ModCmd_startNetwork();
}

static void 
enCheckIP(Socket *const me)
{
    ModCmd_getIpStatus();
}

static void 
enGetOper(Socket *const me)
{
    ModCmd_getOper();
}

static void 
enWaitingServer(Socket *const me)
{
    RKH_SET_STATIC_EVENT(&me->tmEvtObj5, evTout5);
    RKH_TMR_INIT(&me->tmEvtObj5.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj5), NULL);
    RKH_TMR_ONESHOT(&me->tmEvtObj5.tmr, RKH_UPCAST(RKH_SMA_T, me->itsGsmMgr), WaitTime5);
}

static void 
enConnected(Socket *const me)
{
	socketConnected(me);
}

static void 
enIdle(Socket *const me)
{
    RKH_SET_STATIC_EVENT(&me->tmEvtObj6, evTout6);
    RKH_TMR_INIT(&me->tmEvtObj6.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj6), NULL);
    RKH_TMR_ONESHOT(&me->tmEvtObj6.tmr, RKH_UPCAST(RKH_SMA_T, me->itsGsmMgr), WaitTime6);
}

static void 
enWaitReopen(Socket *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj7, evTout7);
	RKH_TMR_INIT(&me->tmEvtObj7.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj7), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj7.tmr, RKH_UPCAST(RKH_SMA_T, me->itsGsmMgr), WaitTime7);
}

/* ............................. Exit actions .............................. */
static void 
exConFailure(GsmMgr *const me)
{
	powerOn();
	me->retryCount = 0;
	rkh_tmr_stop(&me->tmEvtObj0.tmr);
}

static void 
exUnregistered(GsmMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj1.tmr);
}

static void 
exRegistered(GsmMgr *const me)
{
	unregistered();
	rkh_tmr_stop(&me->tmEvtObj2.tmr);
}

static void 
exWaitRetryConfig(Socket *const me)
{
    rkh_tmr_stop(&me->tmEvtObj3.tmr);
}

static void 
exWaitRetryConnect(Socket *const me)
{
    ModCmd_init();
    rkh_tmr_stop(&me->tmEvtObj4.tmr);
}

static void 
exWaitingServer(Socket *const me)
{
	rkh_tmr_stop(&me->tmEvtObj5.tmr);
}

static void 
exConnected(Socket *const me)
{
	socketDisconnected(me);
}

static void 
exIdle(Socket *const me)
{
	rkh_tmr_stop(&me->tmEvtObj6.tmr);
}

static void 
exGetStatus(Socket *const me)
{
	reqRecall(me);
}

static void 
exWaitReopen(Socket *const me)
{
	rkh_tmr_stop(&me->tmEvtObj7.tmr);
}

/* ................................ Guards ................................. */
static rbool_t 
isCondGetStatusToIdle62(Socket *const me, RKH_EVT_T *pe)
{
	return ((me->retryCount < 5)) ? true : false;
}

static rbool_t 
isCondC0ToSync10(GsmMgr *const me, RKH_EVT_T *pe)
{
	return ((me->retryCount < 5)) ? true : false;
}

static rbool_t 
isCondC1ToWaitRetryConfig30(Socket *const me, RKH_EVT_T *pe)
{
	return ((me->retryCount < 5)) ? true : false;
}

static rbool_t 
isCondC2ToWaitRetryConnect32(Socket *const me, RKH_EVT_T *pe)
{
	return ((me->retryCount < Config_getMaxNumConnectRetries())) ? true : false;
}

/* ---------------------------- Global functions --------------------------- */
void
GsmMgr_ctor(void)
{
    GsmMgr *me;

    me = RKH_DOWNCAST(GsmMgr, gsmMgr);
    me->vtbl = rkhSmaVtbl;
    me->vtbl.task = dispatch;

    rkh_sma_ctor(RKH_UPCAST(RKH_SMA_T, me), &me->vtbl);

    me->itsSocket.itsGsmMgr = me;
    me->itsSMS.itsGsmMgr = me;

    gsmMgrSocket = (RKH_SM_T *)&(me->itsSocket);
    gsmMgrSMS = (RKH_SM_T *)&(me->itsSMS);

    RKH_SM_INIT(&me->itsSocket,
                socket, 0, HCAL,
                GsmMgr_Socket_WaitGsmReady, NULL, NULL);

    RKH_SM_INIT(&me->itsSMS,
                sms, 0, HCAL,
                GsmMgr_SMS_WaitGsmReady, NULL, NULL);
}

char *
GsmMgr_getImei(void)
{
    return GsmMgrInt.Imei;
}

/* ------------------------------ End of file ------------------------------ */
