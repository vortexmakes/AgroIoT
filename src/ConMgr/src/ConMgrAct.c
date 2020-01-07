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
#include "ConMgrActRequired.h"

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
ConMgr_ToinactiveExt0(ConMgr *const me, RKH_EVT_T *pe)
{
	me->FAILURE_TRY_DELAY = 6;
		
	RKH_TR_FWK_AO(me);
	RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
	RKH_TR_FWK_STATE(me, &inactive);
	RKH_TR_FWK_STATE(me, &Active);
	RKH_TR_FWK_STATE(me, &initialize);
	RKH_TR_FWK_STATE(me, &sync);
	RKH_TR_FWK_STATE(me, &init);
	RKH_TR_FWK_STATE(me, &pin);
	RKH_TR_FWK_STATE(me, &setPin);
	RKH_TR_FWK_STATE(me, &enableNetTime);
	RKH_TR_FWK_STATE(me, &getImei);
	RKH_TR_FWK_STATE(me, &cipShutdown);
	RKH_TR_FWK_STATE(me, &setManualGet);
	RKH_TR_FWK_STATE(me, &failure);
	RKH_TR_FWK_STATE(me, &unregistered);
	RKH_TR_FWK_STATE(me, &registered);
	RKH_TR_FWK_STATE(me, &localTime);
	RKH_TR_FWK_STATE(me, &connecting);
	RKH_TR_FWK_STATE(me, &waitingServer);
	RKH_TR_FWK_STATE(me, &connected);
	RKH_TR_FWK_STATE(me, &idle);
	RKH_TR_FWK_STATE(me, &receiving);
	RKH_TR_FWK_STATE(me, &getStatus);
	RKH_TR_FWK_STATE(me, &sending);
	RKH_TR_FWK_STATE(me, &waitOk);
	RKH_TR_FWK_STATE(me, &waitPrompt);
	RKH_TR_FWK_STATE(me, &restarting);
	RKH_TR_FWK_STATE(me, &wReopen);
	RKH_TR_FWK_STATE(me, &waitRetryConfig);
	RKH_TR_FWK_STATE(me, &waitNetClockSync);
	RKH_TR_FWK_STATE(me, &configure);
	RKH_TR_FWK_STATE(me, &setAPN);
	RKH_TR_FWK_STATE(me, &enableNetwork);
	RKH_TR_FWK_STATE(me, &checkIP);
	RKH_TR_FWK_STATE(me, &getOper);
	RKH_TR_FWK_STATE(me, &waitRetryConnect);
	RKH_TR_FWK_STATE(me, &disconnecting);
	RKH_TR_FWK_STATE(me, &SMS);
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
		RKH_TR_FWK_OBJ_NAME(ConMgr_ToinactiveExt0, "ToinactiveExt0");
		RKH_TR_FWK_OBJ_NAME(ConMgr_inactiveToActiveExt1, "inactiveToActiveExt1");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ActiveToinactiveExt2, "ActiveToinactiveExt2");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ToinitializeExt4, "ToinitializeExt4");
		RKH_TR_FWK_OBJ_NAME(ConMgr_registeredToregistered_FinalExt25, "registeredToregistered_FinalExt25");
		RKH_TR_FWK_OBJ_NAME(ConMgr_C1Toregistered_FinalExt28, "C1Toregistered_FinalExt28");
		RKH_TR_FWK_OBJ_NAME(ConMgr_localTimeToconfigureExt29, "localTimeToconfigureExt29");
		RKH_TR_FWK_OBJ_NAME(ConMgr_connectingTodisconnectingExt33, "connectingTodisconnectingExt33");
		RKH_TR_FWK_OBJ_NAME(ConMgr_TowaitingServerExt34, "TowaitingServerExt34");
		RKH_TR_FWK_OBJ_NAME(ConMgr_waitingServerTowaitingServerExt35, "waitingServerTowaitingServerExt35");
		RKH_TR_FWK_OBJ_NAME(ConMgr_connectedTorestartingExt37, "connectedTorestartingExt37");
		RKH_TR_FWK_OBJ_NAME(ConMgr_idleTogetStatusExt40, "idleTogetStatusExt40");
		RKH_TR_FWK_OBJ_NAME(ConMgr_idleTosendingExt41, "idleTosendingExt41");
		RKH_TR_FWK_OBJ_NAME(ConMgr_idleToreceivingExt42, "idleToreceivingExt42");
		RKH_TR_FWK_OBJ_NAME(ConMgr_receivingToidleExt43, "receivingToidleExt43");
		RKH_TR_FWK_OBJ_NAME(ConMgr_receivingToidleExt44, "receivingToidleExt44");
		RKH_TR_FWK_OBJ_NAME(ConMgr_getStatusToidleExt45, "getStatusToidleExt45");
		RKH_TR_FWK_OBJ_NAME(ConMgr_getStatusToidleExt46, "getStatusToidleExt46");
		RKH_TR_FWK_OBJ_NAME(ConMgr_sendingToidleExt47, "sendingToidleExt47");
		RKH_TR_FWK_OBJ_NAME(ConMgr_waitOkTosending_FinalExt50, "waitOkTosending_FinalExt50");
		RKH_TR_FWK_OBJ_NAME(ConMgr_waitPromptTowaitOkExt51, "waitPromptTowaitOkExt51");
		RKH_TR_FWK_OBJ_NAME(ConMgr_waitRetryConfigToHConfigureExt54, "waitRetryConfigToHConfigureExt54");
		RKH_TR_FWK_OBJ_NAME(ConMgr_waitNetClockSyncTolocalTimeExt55, "waitNetClockSyncTolocalTimeExt55");
		RKH_TR_FWK_OBJ_NAME(ConMgr_configureToconnectingExt58, "configureToconnectingExt58");
		RKH_TR_FWK_OBJ_NAME(ConMgr_TogetOperExt59, "TogetOperExt59");
		RKH_TR_FWK_OBJ_NAME(ConMgr_checkIPTocheckIPExt64, "checkIPTocheckIPExt64");
		RKH_TR_FWK_OBJ_NAME(ConMgr_getOperTosetAPNExt65, "getOperTosetAPNExt65");
		RKH_TR_FWK_OBJ_NAME(ConMgr_C2Toregistered_FinalExt67, "C2Toregistered_FinalExt67");
		RKH_TR_FWK_OBJ_NAME(ConMgr_waitRetryConnectToconnectingExt68, "waitRetryConnectToconnectingExt68");
		RKH_TR_FWK_OBJ_NAME(ConMgr_inactiveToinactiveLoc0, "inactiveToinactiveLoc0");
		RKH_TR_FWK_OBJ_NAME(ConMgr_inactiveToinactiveLoc1, "inactiveToinactiveLoc1");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ActiveToActiveLoc2, "ActiveToActiveLoc2");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ActiveToActiveLoc3, "ActiveToActiveLoc3");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ActiveToActiveLoc4, "ActiveToActiveLoc4");
		RKH_TR_FWK_OBJ_NAME(ConMgr_unregisteredTounregisteredLoc16, "unregisteredTounregisteredLoc16");
		RKH_TR_FWK_OBJ_NAME(ConMgr_unregisteredTounregisteredLoc17, "unregisteredTounregisteredLoc17");
		RKH_TR_FWK_OBJ_NAME(ConMgr_getStatusTogetStatusLoc22, "getStatusTogetStatusLoc22");
		RKH_TR_FWK_OBJ_NAME(ConMgr_getStatusTogetStatusLoc22, "getStatusTogetStatusLoc22");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ensync, "ensync");
		RKH_TR_FWK_OBJ_NAME(ConMgr_eninit, "eninit");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enpin, "enpin");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ensetPin, "ensetPin");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enenableNetTime, "enenableNetTime");
		RKH_TR_FWK_OBJ_NAME(ConMgr_engetImei, "engetImei");
		RKH_TR_FWK_OBJ_NAME(ConMgr_encipShutdown, "encipShutdown");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ensetManualGet, "ensetManualGet");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enfailure, "enfailure");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enunregistered, "enunregistered");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enregistered, "enregistered");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enconnected, "enconnected");
		RKH_TR_FWK_OBJ_NAME(ConMgr_ensetAPN, "ensetAPN");
		RKH_TR_FWK_OBJ_NAME(ConMgr_enenableNetwork, "enenableNetwork");
		RKH_TR_FWK_OBJ_NAME(ConMgr_encheckIP, "encheckIP");
		RKH_TR_FWK_OBJ_NAME(ConMgr_engetOper, "engetOper");
		RKH_TR_FWK_OBJ_NAME(ConMgr_exfailure, "exfailure");
		RKH_TR_FWK_OBJ_NAME(ConMgr_exregistered, "exregistered");
		RKH_TR_FWK_OBJ_NAME(ConMgr_exconnected, "exconnected");
		RKH_TR_FWK_OBJ_NAME(ConMgr_exwaitRetryConnect, "exwaitRetryConnect");
		RKH_TR_FWK_OBJ_NAME(ConMgr_isCondgetStatusToidle45, "isCondgetStatusToidle45");
		RKH_TR_FWK_OBJ_NAME(ConMgr_isCondC0Tosync10, "isCondC0Tosync10");
		RKH_TR_FWK_OBJ_NAME(ConMgr_isCondC1TowaitRetryConnect27, "isCondC1TowaitRetryConnect27");
		RKH_TR_FWK_OBJ_NAME(ConMgr_isCondC2TowaitRetryConfig66, "isCondC2TowaitRetryConfig66");
	#endif
	
	init();
}

void 
ConMgr_inactiveToActiveExt1(ConMgr *const me, RKH_EVT_T *pe)
{
	PowerOn();
}

void 
ConMgr_ActiveToinactiveExt2(ConMgr *const me, RKH_EVT_T *pe)
{
	close();
}

void 
ConMgr_ToinitializeExt4(ConMgr *const me, RKH_EVT_T *pe)
{
	initializeInit();
}

void 
ConMgr_registeredToregistered_FinalExt25(ConMgr *const me, RKH_EVT_T *pe)
{
	stopSMS();
}

void 
ConMgr_C1Toregistered_FinalExt28(ConMgr *const me, RKH_EVT_T *pe)
{
	stopSMS();
}

void 
ConMgr_localTimeToconfigureExt29(ConMgr *const me, RKH_EVT_T *pe)
{
	rtimeSync();
}

void 
ConMgr_connectingTodisconnectingExt33(ConMgr *const me, RKH_EVT_T *pe)
{
	socketClose();
}

void 
ConMgr_TowaitingServerExt34(ConMgr *const me, RKH_EVT_T *pe)
{
	socketOpen();
}

void 
ConMgr_waitingServerTowaitingServerExt35(ConMgr *const me, RKH_EVT_T *pe)
{
	getConnStatus();
}

void 
ConMgr_connectedTorestartingExt37(ConMgr *const me, RKH_EVT_T *pe)
{
	socketClose();
}

void 
ConMgr_idleTogetStatusExt40(ConMgr *const me, RKH_EVT_T *pe)
{
	getConnStatus();
}

void 
ConMgr_idleTosendingExt41(ConMgr *const me, RKH_EVT_T *pe)
{
	sendRequest();
}

void 
ConMgr_idleToreceivingExt42(ConMgr *const me, RKH_EVT_T *pe)
{
	readData();
}

void 
ConMgr_receivingToidleExt43(ConMgr *const me, RKH_EVT_T *pe)
{
	recvFail();
}

void 
ConMgr_receivingToidleExt44(ConMgr *const me, RKH_EVT_T *pe)
{
	recvOk();
}

void 
ConMgr_getStatusToidleExt45(ConMgr *const me, RKH_EVT_T *pe)
{
	incRetryCount();
	ModCmd_init();
}

void 
ConMgr_getStatusToidleExt46(ConMgr *const me, RKH_EVT_T *pe)
{
	resetRetryCount();
}

void 
ConMgr_sendingToidleExt47(ConMgr *const me, RKH_EVT_T *pe)
{
	sendFail();
}

void 
ConMgr_waitOkTosending_FinalExt50(ConMgr *const me, RKH_EVT_T *pe)
{
	sendOk();
}

void 
ConMgr_waitPromptTowaitOkExt51(ConMgr *const me, RKH_EVT_T *pe)
{
	flushData();
}

void 
ConMgr_waitRetryConfigToHConfigureExt54(ConMgr *const me, RKH_EVT_T *pe)
{
	configTry();
}

void 
ConMgr_waitNetClockSyncTolocalTimeExt55(ConMgr *const me, RKH_EVT_T *pe)
{
	localTimeGet();
}

void 
ConMgr_configureToconnectingExt58(ConMgr *const me, RKH_EVT_T *pe)
{
	ConnectInit();
}

void 
ConMgr_TogetOperExt59(ConMgr *const me, RKH_EVT_T *pe)
{
	configureInit();
}

void 
ConMgr_checkIPTocheckIPExt64(ConMgr *const me, RKH_EVT_T *pe)
{
	requestIP();
}

void 
ConMgr_getOperTosetAPNExt65(ConMgr *const me, RKH_EVT_T *pe)
{
	storeOper();
}

void 
ConMgr_C2Toregistered_FinalExt67(ConMgr *const me, RKH_EVT_T *pe)
{
	stopSMS();
}

void 
ConMgr_waitRetryConnectToconnectingExt68(ConMgr *const me, RKH_EVT_T *pe)
{
	connectTry();
}

void 
ConMgr_inactiveToinactiveLoc0(ConMgr *const me, RKH_EVT_T *pe)
{
	recvFail();
}

void 
ConMgr_inactiveToinactiveLoc1(ConMgr *const me, RKH_EVT_T *pe)
{
	sendFail();
}

void 
ConMgr_ActiveToActiveLoc2(ConMgr *const me, RKH_EVT_T *pe)
{
	sendFail();
}

void 
ConMgr_ActiveToActiveLoc3(ConMgr *const me, RKH_EVT_T *pe)
{
	recvFail();
}

void 
ConMgr_ActiveToActiveLoc4(ConMgr *const me, RKH_EVT_T *pe)
{
	setSigLevel();
}

void 
ConMgr_unregisteredTounregisteredLoc16(ConMgr *const me, RKH_EVT_T *pe)
{
	checkRegStatus();
}

void 
ConMgr_unregisteredTounregisteredLoc17(ConMgr *const me, RKH_EVT_T *pe)
{
	startRegStatusTimer();
}

void 
ConMgr_getStatusTogetStatusLoc22(ConMgr *const me, RKH_EVT_T *pe)
{
	defer();
}

void 
ConMgr_getStatusTogetStatusLoc22(ConMgr *const me, RKH_EVT_T *pe)
{
	defer();
}

/* ............................. Entry actions ............................. */
void 
ConMgr_ensync(ConMgr *const me)
{
	sendSync();
}

void 
ConMgr_eninit(ConMgr *const me)
{
	sendInit();
}

void 
ConMgr_enpin(ConMgr *const me)
{
	checkPin();
}

void 
ConMgr_ensetPin(ConMgr *const me)
{
	setPin();
}

void 
ConMgr_enenableNetTime(ConMgr *const me)
{
	netTimeEnable();
}

void 
ConMgr_engetImei(ConMgr *const me)
{
	getImei();
}

void 
ConMgr_encipShutdown(ConMgr *const me)
{
	cipShutdown();
}

void 
ConMgr_ensetManualGet(ConMgr *const me)
{
	setManualGet();
}

void 
ConMgr_enfailure(ConMgr *const me)
{
	PowerOff();
	RKH_SET_STATIC_EVENT(&me->tmEvtObj0, evTout0);
	RKH_TMR_INIT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj0), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime0);
}

void 
ConMgr_enunregistered(ConMgr *const me)
{
	checkRegStatus();
	RKH_SET_STATIC_EVENT(&me->tmEvtObj1, evTout1);
	RKH_TMR_INIT(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj1), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime1);
}

void 
ConMgr_enregistered(ConMgr *const me)
{
	registered();
}

void 
ConMgr_enwaitingServer(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj2, evTout2);
	RKH_TMR_INIT(&me->tmEvtObj2.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj2), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj2.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime2);
}

void 
ConMgr_enconnected(ConMgr *const me)
{
	socketConnected();
}

void 
ConMgr_enidle(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj3, evTout3);
	RKH_TMR_INIT(&me->tmEvtObj3.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj3), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj3.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime3);
}

void 
ConMgr_enwReopen(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj4, evTout4);
	RKH_TMR_INIT(&me->tmEvtObj4.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj4), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj4.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime4);
}

void 
ConMgr_enwaitRetryConfig(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj5, evTout5);
	RKH_TMR_INIT(&me->tmEvtObj5.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj5), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj5.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime5);
}

void 
ConMgr_enwaitNetClockSync(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj6, evTout6);
	RKH_TMR_INIT(&me->tmEvtObj6.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj6), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj6.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime6);
}

void 
ConMgr_ensetAPN(ConMgr *const me)
{
	setupAPN();
}

void 
ConMgr_enenableNetwork(ConMgr *const me)
{
	startNetwork();
}

void 
ConMgr_encheckIP(ConMgr *const me)
{
	getIpStatus();
}

void 
ConMgr_engetOper(ConMgr *const me)
{
	getOper();
}

void 
ConMgr_enwaitRetryConnect(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj7, evTout7);
	RKH_TMR_INIT(&me->tmEvtObj7.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj7), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj7.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime7);
}

/* ............................. Exit actions .............................. */
void 
ConMgr_exfailure(ConMgr *const me)
{
	PowerOn();
	rkh_tmr_stop(&me->tmEvtObj0.tmr);
}

void 
ConMgr_exunregistered(ConMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj1.tmr);
}

void 
ConMgr_exregistered(ConMgr *const me)
{
	unregistered();
}

void 
ConMgr_exwaitingServer(ConMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj2.tmr);
}

void 
ConMgr_exconnected(ConMgr *const me)
{
	socketDisconnected();
}

void 
ConMgr_exidle(ConMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj3.tmr);
}

void 
ConMgr_exwReopen(ConMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj4.tmr);
}

void 
ConMgr_exwaitRetryConfig(ConMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj5.tmr);
}

void 
ConMgr_exwaitNetClockSync(ConMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj6.tmr);
}

void 
ConMgr_exwaitRetryConnect(ConMgr *const me)
{
	ModCmd_init();
	rkh_tmr_stop(&me->tmEvtObj7.tmr);
}

/* ................................ Guards ................................. */
rbool_t 
ConMgr_isCondgetStatusToidle45(ConMgr *const me, RKH_EVT_T *pe)
{
	return (inTryGetStatus()) ? true : false;
}

rbool_t 
ConMgr_isCondC0Tosync10(ConMgr *const me, RKH_EVT_T *pe)
{
	return (keepTrying()) ? true : false;
}

rbool_t 
ConMgr_isCondC1TowaitRetryConnect27(ConMgr *const me, RKH_EVT_T *pe)
{
	return (checkConnectTry()) ? true : false;
}

rbool_t 
ConMgr_isCondC2TowaitRetryConfig66(ConMgr *const me, RKH_EVT_T *pe)
{
	return (checkConfigTry()) ? true : false;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
