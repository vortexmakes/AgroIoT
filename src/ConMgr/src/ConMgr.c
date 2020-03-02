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
#include "rkhsma.h"
#include "signals.h"
#include "ConMgr.h"
#include "ConMgrAct.h"

/* ----------------------------- Local macros ------------------------------ */
/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(ConMgr_Inactive, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_Sync, ConMgr_enConMgr_Sync, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_Init, ConMgr_enConMgr_Init, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_Pin, ConMgr_enConMgr_Pin, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_SetPin, ConMgr_enConMgr_SetPin, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_EnableNetTime, ConMgr_enConMgr_EnableNetTime, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_GetImei, ConMgr_enConMgr_GetImei, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_SetManualGet, ConMgr_enConMgr_SetManualGet, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_CipShutdown, ConMgr_enConMgr_CipShutdown, NULL, &ConMgr_Initialize, NULL);
RKH_CREATE_BASIC_STATE(ConFailure, ConMgr_enConFailure, ConMgr_exConFailure, &ConMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(Unregistered, ConMgr_enUnregistered, ConMgr_exUnregistered, &ConMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_LocalTime, NULL, NULL, &ConMgr_Registered, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_WaitingServer, ConMgr_enConMgr_WaitingServer, ConMgr_exConMgr_WaitingServer, &ConMgr_Connecting, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_Idle, ConMgr_enConMgr_Idle, ConMgr_exConMgr_Idle, &ConMgr_Connected, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_receiving, NULL, NULL, &ConMgr_Connected, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_GetStatus, NULL, ConMgr_exConMgr_GetStatus, &ConMgr_Connected, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_waitOk, NULL, NULL, &ConMgr_Sending, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_WaitPrompt, NULL, NULL, &ConMgr_Sending, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_Restarting, NULL, NULL, &ConMgr_Connecting, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_WaitReopen, ConMgr_enConMgr_WaitReopen, ConMgr_exConMgr_WaitReopen, &ConMgr_Connecting, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_WaitRetryConfig, ConMgr_enConMgr_WaitRetryConfig, ConMgr_exConMgr_WaitRetryConfig, &ConMgr_Registered, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_WaitNetClockSync, ConMgr_enConMgr_WaitNetClockSync, ConMgr_exConMgr_WaitNetClockSync, &ConMgr_Registered, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_SetAPN, ConMgr_enConMgr_SetAPN, NULL, &ConMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_EnableNetwork, ConMgr_enConMgr_EnableNetwork, NULL, &ConMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_CheckIP, ConMgr_enConMgr_CheckIP, NULL, &ConMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_GetOper, ConMgr_enConMgr_GetOper, NULL, &ConMgr_Configure, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_WaitRetryConnect, ConMgr_enConMgr_WaitRetryConnect, ConMgr_exConMgr_WaitRetryConnect, &ConMgr_Registered, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_Disconnecting, NULL, NULL, &ConMgr_Registered, NULL);
RKH_CREATE_BASIC_STATE(ConMgr_SMS, NULL, NULL, &ConMgr_Registered, NULL);

RKH_CREATE_COMP_REGION_STATE(ConMgr_Active, NULL, NULL, RKH_ROOT, &ConMgr_Initialize, ConMgr_ToConMgr_InitializeExt4, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(ConMgr_Initialize, NULL, NULL, &ConMgr_Active, &ConMgr_Sync, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(ConMgr_Registered, ConMgr_enConMgr_Registered, ConMgr_exConMgr_Registered, &ConMgr_Active, &ConMgr_WaitNetClockSync, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(ConMgr_Connecting, NULL, NULL, &ConMgr_Registered, &ConMgr_WaitingServer, ConMgr_ToConMgr_WaitingServerExt34, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(ConMgr_Connected, ConMgr_enConMgr_Connected, ConMgr_exConMgr_Connected, &ConMgr_Connecting, &ConMgr_Idle, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(ConMgr_Sending, NULL, NULL, &ConMgr_Connected, &ConMgr_WaitPrompt, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_HISTORY_STORAGE(ConMgr_Configure);
RKH_CREATE_COMP_REGION_STATE(ConMgr_Configure, NULL, NULL, &ConMgr_Registered, &ConMgr_GetOper, ConMgr_ToConMgr_GetOperExt59, RKH_SHISTORY, NULL, NULL, NULL, RKH_GET_HISTORY_STORAGE(ConMgr_Configure));

RKH_CREATE_TRANS_TABLE(ConMgr_Inactive)
	RKH_TRREG(evOpen, NULL, ConMgr_ConMgr_InactiveToConMgr_ActiveExt1, &ConMgr_Active),
	RKH_TRINT(evRecv, NULL, ConMgr_ConMgr_InactiveToConMgr_InactiveLoc0),
	RKH_TRINT(evSend, NULL, ConMgr_ConMgr_InactiveToConMgr_InactiveLoc1),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Active)
	RKH_TRREG(evClose, NULL, ConMgr_ConMgr_ActiveToConMgr_InactiveExt2, &ConMgr_Inactive),
	RKH_TRCOMPLETION(NULL, NULL, &ConMgr_Inactive),
	RKH_TRINT(evSend, NULL, ConMgr_ConMgr_ActiveToConMgr_ActiveLoc2),
	RKH_TRINT(evRecv, NULL, ConMgr_ConMgr_ActiveToConMgr_ActiveLoc3),
	RKH_TRINT(evSigLevel, NULL, ConMgr_ConMgr_ActiveToConMgr_ActiveLoc4),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Initialize)
	RKH_TRREG(evNoResponse, NULL, NULL, &ConFailure),
	RKH_TRREG(evError, NULL, NULL, &ConFailure),
	RKH_TRCOMPLETION(NULL, NULL, &ConFailure),
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
	RKH_TRREG(evImei, NULL, ConMgr_ConMgr_GetImeiToConMgr_CipShutdownExt18, &ConMgr_CipShutdown),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_SetManualGet)
	RKH_TRREG(evOk, NULL, NULL, &Unregistered),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_CipShutdown)
	RKH_TRREG(evOk, NULL, NULL, &ConMgr_SetManualGet),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConFailure)
	RKH_TRREG(evTout0, NULL, NULL, &ConMgr_Active),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(Unregistered)
	RKH_TRREG(evTout1, NULL, NULL, &ConFailure),
	RKH_TRREG(evReg, NULL, NULL, &ConMgr_Registered),
	RKH_TRINT(evRegStatusTout, NULL, ConMgr_UnregisteredToUnregisteredLoc16),
	RKH_TRINT(evNoReg, NULL, ConMgr_UnregisteredToUnregisteredLoc17),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Registered)
	RKH_TRREG(evNoReg, NULL, NULL, &Unregistered),
	RKH_TRREG(evError, NULL, ConMgr_ConMgr_RegisteredToConMgr_Registered_FinalExt25, &ConMgr_RegisteredFinal),
	RKH_TRCOMPLETION(NULL, NULL, &ConFailure),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_LocalTime)
	RKH_TRREG(evLocalTime, NULL, ConMgr_ConMgr_LocalTimeToConMgr_ConfigureExt29, &ConMgr_Configure),
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
	RKH_TRREG(evClose, NULL, ConMgr_ConMgr_ConnectingToConMgr_DisconnectingExt33, &ConMgr_Disconnecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_WaitingServer)
	RKH_TRREG(evTout2, NULL, ConMgr_ConMgr_WaitingServerToConMgr_WaitingServerExt35, &ConMgr_WaitingServer),
	RKH_TRREG(evConnected, NULL, NULL, &ConMgr_Connected),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Connected)
	RKH_TRREG(evRestart, NULL, ConMgr_ConMgr_ConnectedToConMgr_RestartingExt37, &ConMgr_Restarting),
	RKH_TRREG(evClosed, NULL, NULL, &ConMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Idle)
	RKH_TRREG(evTout3, NULL, ConMgr_ConMgr_IdleToConMgr_GetStatusExt40, &ConMgr_GetStatus),
	RKH_TRREG(evSend, NULL, ConMgr_ConMgr_IdleToConMgr_SendingExt41, &ConMgr_Sending),
	RKH_TRREG(evRecv, NULL, ConMgr_ConMgr_IdleToConMgr_receivingExt42, &ConMgr_receiving),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_receiving)
	RKH_TRREG(evNoResponse, NULL, ConMgr_ConMgr_receivingToConMgr_IdleExt43, &ConMgr_Idle),
	RKH_TRREG(evError, NULL, ConMgr_ConMgr_receivingToConMgr_IdleExt43, &ConMgr_Idle),
	RKH_TRREG(evOk, NULL, ConMgr_ConMgr_receivingToConMgr_IdleExt44, &ConMgr_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_GetStatus)
	RKH_TRREG(evNoResponse, ConMgr_isCondConMgr_GetStatusToConMgr_Idle45, ConMgr_ConMgr_GetStatusToConMgr_IdleExt45, &ConMgr_Idle),
	RKH_TRREG(evConnected, NULL, ConMgr_ConMgr_GetStatusToConMgr_IdleExt46, &ConMgr_Idle),
	RKH_TRINT(evSend, NULL, ConMgr_ConMgr_GetStatusToConMgr_GetStatusLoc23),
	RKH_TRINT(evRecv, NULL, ConMgr_ConMgr_GetStatusToConMgr_GetStatusLoc24),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Sending)
	RKH_TRREG(evNoResponse, NULL, ConMgr_ConMgr_SendingToConMgr_IdleExt47, &ConMgr_Idle),
	RKH_TRREG(evError, NULL, ConMgr_ConMgr_SendingToConMgr_IdleExt47, &ConMgr_Idle),
	RKH_TRCOMPLETION(NULL, NULL, &ConMgr_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_waitOk)
	RKH_TRREG(evOk, NULL, ConMgr_ConMgr_waitOkToConMgr_Sending_FinalExt50, &ConMgr_SendingFinal),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_WaitPrompt)
	RKH_TRREG(evOk, NULL, ConMgr_ConMgr_WaitPromptToConMgr_waitOkExt51, &ConMgr_waitOk),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Restarting)
	RKH_TRREG(evDisconnected, NULL, NULL, &ConMgr_WaitReopen),
	RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_WaitReopen),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_WaitReopen)
	RKH_TRREG(evTout4, NULL, NULL, &ConMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_WaitRetryConfig)
    RKH_TRREG(evTout5, NULL, ConMgr_ConMgr_WaitRetryConfigToConMgr_ConfigureHistExt54, &ConMgr_ConfigureHist),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_WaitNetClockSync)
	RKH_TRREG(evNetClockSync, NULL, ConMgr_ConMgr_WaitNetClockSyncToConMgr_LocalTimeExt55, &ConMgr_LocalTime),
	RKH_TRREG(evTout6, NULL, NULL, &ConMgr_Configure),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Configure)
	RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_C2),
	RKH_TRCOMPLETION(NULL, ConMgr_ConMgr_ConfigureToConMgr_ConnectingExt58, &ConMgr_Connecting),
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
	RKH_TRREG(evIPGprsAct, NULL, ConMgr_ConMgr_CheckIPToConMgr_CheckIPExt64, &ConMgr_CheckIP),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_GetOper)
	RKH_TRREG(evOper, NULL, ConMgr_ConMgr_GetOperToConMgr_SetAPNExt65, &ConMgr_SetAPN),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_WaitRetryConnect)
	RKH_TRREG(evTout7, NULL, ConMgr_ConMgr_WaitRetryConnectToConMgr_ConnectingExt68, &ConMgr_Connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_Disconnecting)
	RKH_TRREG(evDisconnected, NULL, NULL, &ConMgr_ActiveFinal),
	RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_ActiveFinal),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ConMgr_SMS)
	RKH_TRREG(evStopSMS, NULL, NULL, &ConMgr_RegisteredFinal),
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(ConMgr_C0);
RKH_CREATE_CHOICE_STATE(ConMgr_C1);
RKH_CREATE_CHOICE_STATE(ConMgr_C2);

RKH_CREATE_BRANCH_TABLE(ConMgr_C0)
	RKH_BRANCH(ConMgr_isCondC0ToConMgr_Sync10, NULL, &ConMgr_Sync),
	RKH_BRANCH(ELSE, NULL, &ConMgr_InitializeFinal),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(ConMgr_C1)
	RKH_BRANCH(ConMgr_isCondC1ToConMgr_WaitRetryConnect27, NULL, &ConMgr_WaitRetryConnect),
	RKH_BRANCH(ELSE, ConMgr_C1ToConMgr_Registered_FinalExt28, &ConMgr_RegisteredFinal),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(ConMgr_C2)
	RKH_BRANCH(ConMgr_isCondC2ToConMgr_WaitRetryConfig66, NULL, &ConMgr_WaitRetryConfig),
	RKH_BRANCH(ELSE, ConMgr_C2ToConMgr_Registered_FinalExt67, &ConMgr_RegisteredFinal),
RKH_END_BRANCH_TABLE


/* ............................. Active object ............................. */
RKH_SMA_CREATE(ConMgr, conMgr, 1, HCAL, &ConMgr_Inactive, ConMgr_ToConMgr_InactiveExt0, NULL);
RKH_SMA_DEF_PTR(conMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
