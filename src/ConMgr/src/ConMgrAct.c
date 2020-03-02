/**
 *  \file       ConMgr.c
 *  \brief      Active object's action implementations.
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
#include "rkhtmr.h"
#include "signals.h"
#include "ConMgr.h"
#include "ConMgrAct.h"
#include "bsp.h"
#include "ConMgrActAccess.h"

/* ----------------------------- Local macros ------------------------------ */
#define WaitTime0	RKH_TIME_SEC(5)
#define WaitTime1	RKH_TIME_SEC(60)
#define WaitTime2	RKH_TIME_SEC(2)
#define WaitTime3	RKH_TIME_SEC(2)
#define WaitTime4	RKH_TIME_SEC(2)
#define WaitTime5	RKH_TIME_SEC(2)
#define WaitTime6	RKH_TIME_SEC(5)
#define WaitTime7	RKH_TIME_SEC(2)

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Effect actions ............................. */
void 
ConMgr_ToConMgr_InactiveExt0(ConMgr *const me, RKH_EVT_T *pe)
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
	RKH_TR_FWK_STATE(me, &ConFailure);
	RKH_TR_FWK_STATE(me, &Unregistered);
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
	RKH_TR_FWK_STATE(me, &ConMgr_SMS);

	RKH_TR_FWK_STATE(me, &ConMgr_InitializeFinal);
	RKH_TR_FWK_STATE(me, &ConMgr_RegisteredFinal);
	RKH_TR_FWK_STATE(me, &ConMgr_RegisteredFinal);
	RKH_TR_FWK_STATE(me, &ConMgr_SendingFinal);
	RKH_TR_FWK_STATE(me, &ConMgr_ConfigureFinal);
	RKH_TR_FWK_STATE(me, &ConMgr_ActiveFinal);

	RKH_TR_FWK_STATE(me, &ConMgr_C0);
	RKH_TR_FWK_STATE(me, &ConMgr_C1);
	RKH_TR_FWK_STATE(me, &ConMgr_C2);

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
		RKH_TR_FWK_OBJ_NAME(ConMgr_ToConMgr_InactiveExt0, "ToConMgr_InactiveExt0");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_InactiveToConMgr_ActiveExt1, "ConMgr_InactiveToConMgr_ActiveExt1");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_ActiveToConMgr_InactiveExt2, "ConMgr_ActiveToConMgr_InactiveExt2");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ToConMgr_InitializeExt4, "ToConMgr_InitializeExt4");
        RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_GetImeiToConMgr_CipShutdownExt18, "ConMgr_GetImeiToConMgr_CipShutdownExt18");
        RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_RegisteredToConMgr_Registered_FinalExt25, "ConMgr_RegisteredToConMgr_Registered_FinalExt25");
		RKH_TR_FWK_OBJ_NAME(ConMgr_C1ToConMgr_Registered_FinalExt28, "C1ToConMgr_Registered_FinalExt28");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_LocalTimeToConMgr_ConfigureExt29, "ConMgr_LocalTimeToConMgr_ConfigureExt29");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_ConnectingToConMgr_DisconnectingExt33, "ConMgr_ConnectingToConMgr_DisconnectingExt33");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ToConMgr_WaitingServerExt34, "ToConMgr_WaitingServerExt34");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_WaitingServerToConMgr_WaitingServerExt35, "ConMgr_WaitingServerToConMgr_WaitingServerExt35");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_ConnectedToConMgr_RestartingExt37, "ConMgr_ConnectedToConMgr_RestartingExt37");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_IdleToConMgr_GetStatusExt40, "ConMgr_IdleToConMgr_GetStatusExt40");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_IdleToConMgr_SendingExt41, "ConMgr_IdleToConMgr_SendingExt41");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_IdleToConMgr_receivingExt42, "ConMgr_IdleToConMgr_receivingExt42");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_receivingToConMgr_IdleExt43, "ConMgr_receivingToConMgr_IdleExt43");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_receivingToConMgr_IdleExt44, "ConMgr_receivingToConMgr_IdleExt44");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_GetStatusToConMgr_IdleExt45, "ConMgr_GetStatusToConMgr_IdleExt45");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_GetStatusToConMgr_IdleExt46, "ConMgr_GetStatusToConMgr_IdleExt46");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_SendingToConMgr_IdleExt47, "ConMgr_SendingToConMgr_IdleExt47");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_waitOkToConMgr_Sending_FinalExt50, "ConMgr_waitOkToConMgr_Sending_FinalExt50");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_WaitPromptToConMgr_waitOkExt51, "ConMgr_WaitPromptToConMgr_waitOkExt51");
        RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_WaitRetryConfigToConMgr_ConfigureHistExt54, "ConMgr_WaitRetryConfigToConMgr_ConfigureHistExt54");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_WaitNetClockSyncToConMgr_LocalTimeExt55, "ConMgr_WaitNetClockSyncToConMgr_LocalTimeExt55");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_ConfigureToConMgr_ConnectingExt58, "ConMgr_ConfigureToConMgr_ConnectingExt58");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ToConMgr_GetOperExt59, "ToConMgr_GetOperExt59");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_CheckIPToConMgr_CheckIPExt64, "ConMgr_CheckIPToConMgr_CheckIPExt64");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_GetOperToConMgr_SetAPNExt65, "ConMgr_GetOperToConMgr_SetAPNExt65");
		RKH_TR_FWK_OBJ_NAME(ConMgr_C2ToConMgr_Registered_FinalExt67, "C2ToConMgr_Registered_FinalExt67");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_WaitRetryConnectToConMgr_ConnectingExt68, "ConMgr_WaitRetryConnectToConMgr_ConnectingExt68");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_InactiveToConMgr_InactiveLoc0, "ConMgr_InactiveToConMgr_InactiveLoc0");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_InactiveToConMgr_InactiveLoc1, "ConMgr_InactiveToConMgr_InactiveLoc1");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_ActiveToConMgr_ActiveLoc2, "ConMgr_ActiveToConMgr_ActiveLoc2");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_ActiveToConMgr_ActiveLoc3, "ConMgr_ActiveToConMgr_ActiveLoc3");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_ActiveToConMgr_ActiveLoc4, "ConMgr_ActiveToConMgr_ActiveLoc4");
		RKH_TR_FWK_OBJ_NAME(ConMgr_UnregisteredToUnregisteredLoc16, "UnregisteredToUnregisteredLoc16");
		RKH_TR_FWK_OBJ_NAME(ConMgr_UnregisteredToUnregisteredLoc17, "UnregisteredToUnregisteredLoc17");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_GetStatusToConMgr_GetStatusLoc23, "ConMgr_GetStatusToConMgr_GetStatusLoc23");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ConMgr_GetStatusToConMgr_GetStatusLoc24, "ConMgr_GetStatusToConMgr_GetStatusLoc24");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_Sync, "enConMgr_Sync");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_Init, "enConMgr_Init");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_Pin, "enConMgr_Pin");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_SetPin, "enConMgr_SetPin");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_EnableNetTime, "enConMgr_EnableNetTime");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_GetImei, "enConMgr_GetImei");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_SetManualGet, "enConMgr_SetManualGet");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_CipShutdown, "enConMgr_CipShutdown");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConFailure, "enConFailure");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enUnregistered, "enUnregistered");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_Registered, "enConMgr_Registered");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_Connected, "enConMgr_Connected");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_SetAPN, "enConMgr_SetAPN");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_EnableNetwork, "enConMgr_EnableNetwork");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_CheckIP, "enConMgr_CheckIP");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enConMgr_GetOper, "enConMgr_GetOper");
		RKH_TR_FWK_OBJ_NAME(ConMgr_exConFailure, "exConFailure");
		RKH_TR_FWK_OBJ_NAME(ConMgr_exConMgr_Registered, "exConMgr_Registered");
		RKH_TR_FWK_OBJ_NAME(ConMgr_exConMgr_Connected, "exConMgr_Connected");
		RKH_TR_FWK_OBJ_NAME(ConMgr_exConMgr_GetStatus, "exConMgr_GetStatus");
		RKH_TR_FWK_OBJ_NAME(ConMgr_exConMgr_WaitRetryConnect, "exConMgr_WaitRetryConnect");
		RKH_TR_FWK_OBJ_NAME(ConMgr_isCondConMgr_GetStatusToConMgr_Idle45, "isCondConMgr_GetStatusToConMgr_Idle45");
		RKH_TR_FWK_OBJ_NAME(ConMgr_isCondC0ToConMgr_Sync10, "isCondC0ToConMgr_Sync10");
		RKH_TR_FWK_OBJ_NAME(ConMgr_isCondC1ToConMgr_WaitRetryConnect27, "isCondC1ToConMgr_WaitRetryConnect27");
		RKH_TR_FWK_OBJ_NAME(ConMgr_isCondC2ToConMgr_WaitRetryConfig66, "isCondC2ToConMgr_WaitRetryConfig66");
	#endif
	
	init(me);
	me->retryCount = 0;
    Config_getConnectionDomain(me->domain);
    Config_getConnectionPort(me->port);
}

void 
ConMgr_ConMgr_InactiveToConMgr_ActiveExt1(ConMgr *const me, RKH_EVT_T *pe)
{
	ModMgr_open();
	powerOn();
}

void 
ConMgr_ConMgr_ActiveToConMgr_InactiveExt2(ConMgr *const me, RKH_EVT_T *pe)
{
	ModMgr_close();
	powerOff();
}

void 
ConMgr_ToConMgr_InitializeExt4(ConMgr *const me, RKH_EVT_T *pe)
{
    me->retryCount = 0;
}

void 
ConMgr_ConMgr_GetImeiToConMgr_CipShutdownExt18(ConMgr *const me, RKH_EVT_T *pe)
{
    storeImei(me, pe);
}

void 
ConMgr_ConMgr_RegisteredToConMgr_Registered_FinalExt25(ConMgr *const me, RKH_EVT_T *pe)
{
	stopSMS();
}

void 
ConMgr_C1ToConMgr_Registered_FinalExt28(ConMgr *const me, RKH_EVT_T *pe)
{
	stopSMS();
}

void 
ConMgr_ConMgr_LocalTimeToConMgr_ConfigureExt29(ConMgr *const me, RKH_EVT_T *pe)
{
	rtimeSync(pe);
}

void 
ConMgr_ConMgr_ConnectingToConMgr_DisconnectingExt33(ConMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_disconnect();
}

void 
ConMgr_ToConMgr_WaitingServerExt34(ConMgr *const me, RKH_EVT_T *pe)
{
    ModCmd_connect(me->protocol, me->domain, me->port);
}

void 
ConMgr_ConMgr_WaitingServerToConMgr_WaitingServerExt35(ConMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_getConnStatus();
}

void 
ConMgr_ConMgr_ConnectedToConMgr_RestartingExt37(ConMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_disconnect();
}

void 
ConMgr_ConMgr_IdleToConMgr_GetStatusExt40(ConMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_getConnStatus();
}

void 
ConMgr_ConMgr_IdleToConMgr_SendingExt41(ConMgr *const me, RKH_EVT_T *pe)
{
	sendRequest(pe);
}

void 
ConMgr_ConMgr_IdleToConMgr_receivingExt42(ConMgr *const me, RKH_EVT_T *pe)
{
	readData();
}

void 
ConMgr_ConMgr_receivingToConMgr_IdleExt43(ConMgr *const me, RKH_EVT_T *pe)
{
	recvFail(me);
}

void 
ConMgr_ConMgr_receivingToConMgr_IdleExt44(ConMgr *const me, RKH_EVT_T *pe)
{
	recvOk(me);
}

void 
ConMgr_ConMgr_GetStatusToConMgr_IdleExt45(ConMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount += 1;
	ModCmd_init();
}

void 
ConMgr_ConMgr_GetStatusToConMgr_IdleExt46(ConMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount = 0;
}

void 
ConMgr_ConMgr_SendingToConMgr_IdleExt47(ConMgr *const me, RKH_EVT_T *pe)
{
	sendFail(me);
}

void 
ConMgr_ConMgr_waitOkToConMgr_Sending_FinalExt50(ConMgr *const me, RKH_EVT_T *pe)
{
    me->retryCount = 0;
    sendOk(me);
}

void 
ConMgr_ConMgr_WaitPromptToConMgr_waitOkExt51(ConMgr *const me, RKH_EVT_T *pe)
{
	flushData();
}

void 
ConMgr_ConMgr_WaitRetryConfigToConMgr_ConfigureHistExt54(ConMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount += 1;
	ModCmd_init();
}

void 
ConMgr_ConMgr_WaitNetClockSyncToConMgr_LocalTimeExt55(ConMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_getLocalTime();
}

void 
ConMgr_ConMgr_ConfigureToConMgr_ConnectingExt58(ConMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount = 0;
}

void 
ConMgr_ToConMgr_GetOperExt59(ConMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount = 0;
}

void 
ConMgr_ConMgr_CheckIPToConMgr_CheckIPExt64(ConMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_requestIP();
}

void 
ConMgr_ConMgr_GetOperToConMgr_SetAPNExt65(ConMgr *const me, RKH_EVT_T *pe)
{
	storeOper(me, pe);
}

void 
ConMgr_C2ToConMgr_Registered_FinalExt67(ConMgr *const me, RKH_EVT_T *pe)
{
	stopSMS();
}

void 
ConMgr_ConMgr_WaitRetryConnectToConMgr_ConnectingExt68(ConMgr *const me, RKH_EVT_T *pe)
{
	me->retryCount += 1;
}

void 
ConMgr_ConMgr_InactiveToConMgr_InactiveLoc0(ConMgr *const me, RKH_EVT_T *pe)
{
	recvFail(me);
}

void 
ConMgr_ConMgr_InactiveToConMgr_InactiveLoc1(ConMgr *const me, RKH_EVT_T *pe)
{
	sendFail(me);
}

void 
ConMgr_ConMgr_ActiveToConMgr_ActiveLoc2(ConMgr *const me, RKH_EVT_T *pe)
{
	sendFail(me);
}

void 
ConMgr_ConMgr_ActiveToConMgr_ActiveLoc3(ConMgr *const me, RKH_EVT_T *pe)
{
	recvFail(me);
}

void 
ConMgr_ConMgr_ActiveToConMgr_ActiveLoc4(ConMgr *const me, RKH_EVT_T *pe)
{
	setSigLevel(me, pe);
}

void 
ConMgr_UnregisteredToUnregisteredLoc16(ConMgr *const me, RKH_EVT_T *pe)
{
	ModCmd_getRegStatus();
}

void 
ConMgr_UnregisteredToUnregisteredLoc17(ConMgr *const me, RKH_EVT_T *pe)
{
	startRegStatusTimer(me);
}

void 
ConMgr_ConMgr_GetStatusToConMgr_GetStatusLoc23(ConMgr *const me, RKH_EVT_T *pe)
{
	reqDefer(pe);
}

void 
ConMgr_ConMgr_GetStatusToConMgr_GetStatusLoc24(ConMgr *const me, RKH_EVT_T *pe)
{
	reqDefer(pe);
}

/* ............................. Entry actions ............................. */
void 
ConMgr_enConMgr_Sync(ConMgr *const me)
{
    me->retryCount += 1;
    ModCmd_sync();
}

void 
ConMgr_enConMgr_Init(ConMgr *const me)
{
    modemFound();
    ModCmd_initStr();
}

void 
ConMgr_enConMgr_Pin(ConMgr *const me)
{
	ModCmd_getPinStatus();
}

void 
ConMgr_enConMgr_SetPin(ConMgr *const me)
{
    ModCmd_setPin(Config_getSIMPinNumber());
}

void 
ConMgr_enConMgr_EnableNetTime(ConMgr *const me)
{
	ModCmd_enableNetTime();
}

void 
ConMgr_enConMgr_GetImei(ConMgr *const me)
{
	ModCmd_getImei();
}

void 
ConMgr_enConMgr_SetManualGet(ConMgr *const me)
{
	ModCmd_setManualGet();
}

void 
ConMgr_enConMgr_CipShutdown(ConMgr *const me)
{
	ModCmd_cipShutdown();
}

void 
ConMgr_enConFailure(ConMgr *const me)
{
	powerOff();
	RKH_SET_STATIC_EVENT(&me->tmEvtObj0, evTout0);
	RKH_TMR_INIT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj0), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime0);
}

void 
ConMgr_enUnregistered(ConMgr *const me)
{
	ModCmd_getRegStatus();
	RKH_SET_STATIC_EVENT(&me->tmEvtObj1, evTout1);
	RKH_TMR_INIT(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj1), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime1);
}

void 
ConMgr_enConMgr_Registered(ConMgr *const me)
{
	registered();
}

void 
ConMgr_enConMgr_WaitingServer(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj2, evTout2);
	RKH_TMR_INIT(&me->tmEvtObj2.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj2), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj2.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime2);
}

void 
ConMgr_enConMgr_Connected(ConMgr *const me)
{
	socketConnected(me);
}

void 
ConMgr_enConMgr_Idle(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj3, evTout3);
	RKH_TMR_INIT(&me->tmEvtObj3.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj3), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj3.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime3);
}

void 
ConMgr_enConMgr_WaitReopen(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj4, evTout4);
	RKH_TMR_INIT(&me->tmEvtObj4.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj4), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj4.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime4);
}

void 
ConMgr_enConMgr_WaitRetryConfig(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj5, evTout5);
	RKH_TMR_INIT(&me->tmEvtObj5.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj5), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj5.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime5);
}

void 
ConMgr_enConMgr_WaitNetClockSync(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj6, evTout6);
	RKH_TMR_INIT(&me->tmEvtObj6.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj6), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj6.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime6);
}

void 
ConMgr_enConMgr_SetAPN(ConMgr *const me)
{
    setupAPN(me);
}

void 
ConMgr_enConMgr_EnableNetwork(ConMgr *const me)
{
	ModCmd_startNetwork();
}

void 
ConMgr_enConMgr_CheckIP(ConMgr *const me)
{
	ModCmd_getIpStatus();
}

void 
ConMgr_enConMgr_GetOper(ConMgr *const me)
{
	ModCmd_getOper();
}

void 
ConMgr_enConMgr_WaitRetryConnect(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj7, evTout7);
	RKH_TMR_INIT(&me->tmEvtObj7.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj7), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj7.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime7);
}

/* ............................. Exit actions .............................. */
void 
ConMgr_exConFailure(ConMgr *const me)
{
	powerOn();
	rkh_tmr_stop(&me->tmEvtObj0.tmr);
}

void 
ConMgr_exUnregistered(ConMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj1.tmr);
}

void 
ConMgr_exConMgr_Registered(ConMgr *const me)
{
	unregistered();
}

void 
ConMgr_exConMgr_WaitingServer(ConMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj2.tmr);
}

void 
ConMgr_exConMgr_Connected(ConMgr *const me)
{
	socketDisconnected(me);
}

void 
ConMgr_exConMgr_Idle(ConMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj3.tmr);
}

void 
ConMgr_exConMgr_GetStatus(ConMgr *const me)
{
	reqRecall(me);
}

void 
ConMgr_exConMgr_WaitReopen(ConMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj4.tmr);
}

void 
ConMgr_exConMgr_WaitRetryConfig(ConMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj5.tmr);
}

void 
ConMgr_exConMgr_WaitNetClockSync(ConMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj6.tmr);
}

void 
ConMgr_exConMgr_WaitRetryConnect(ConMgr *const me)
{
	ModCmd_init();
	rkh_tmr_stop(&me->tmEvtObj7.tmr);
}

/* ................................ Guards ................................. */
rbool_t 
ConMgr_isCondConMgr_GetStatusToConMgr_Idle45(ConMgr *const me, RKH_EVT_T *pe)
{
	return ((me->retryCount < 5)) ? true : false;
}

rbool_t 
ConMgr_isCondC0ToConMgr_Sync10(ConMgr *const me, RKH_EVT_T *pe)
{
	return ((me->retryCount < 5)) ? true : false;
}

rbool_t 
ConMgr_isCondC1ToConMgr_WaitRetryConnect27(ConMgr *const me, RKH_EVT_T *pe)
{
	return ((me->retryCount < Config_getMaxNumConnectRetries())) ? true : false;
}

rbool_t 
ConMgr_isCondC2ToConMgr_WaitRetryConfig66(ConMgr *const me, RKH_EVT_T *pe)
{
	return ((me->retryCount < 5)) ? true : false;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
