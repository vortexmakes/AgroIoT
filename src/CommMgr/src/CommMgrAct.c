/**
 *  \file       CommMgr.c
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
#include "CommMgr.h"
#include "CommMgrAct.h"
#include "bsp.h"
#include "CommMgrActRequired.h"

/* ----------------------------- Local macros ------------------------------ */
#define WaitTime0	RKH_TIME_SEC(3)

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Effect actions ............................. */
void 
CommMgr_ToidleExt0(CommMgr *const me, RKH_EVT_T *pe)
{
		
	RKH_TR_FWK_AO(me);
	RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
	RKH_TR_FWK_STATE(me, &idle);
	RKH_TR_FWK_STATE(me, &active);
	RKH_TR_FWK_STATE(me, &WaitSync);
	RKH_TR_FWK_STATE(me, &SendingStatus);
	RKH_TR_FWK_STATE(me, &ReceivingAck);
	RKH_TR_FWK_STATE(me, &SendingEndOfHist);
	RKH_TR_FWK_STATE(me, &SendingHist);
	RKH_TR_FWK_STATE(me, &EndCycle);
	RKH_TR_FWK_SIG(evNetConnected);
	RKH_TR_FWK_SIG(evNetDisconnected);
	RKH_TR_FWK_SIG(evSent);
	RKH_TR_FWK_SIG(evReceived);
	RKH_TR_FWK_SIG(evRecvFail);
	RKH_TR_FWK_SIG(evSendFail);
	RKH_TR_FWK_SIG(evGStatus);
	RKH_TR_FWK_TIMER(&me->tmEvtObj0.tmr);
	#if 0
		RKH_TR_FWK_OBJ_NAME(CommMgr_ToidleExt0, "ToidleExt0");
		RKH_TR_FWK_OBJ_NAME(CommMgr_idleToactiveExt1, "idleToactiveExt1");
		RKH_TR_FWK_OBJ_NAME(CommMgr_ReceivingAckToC1Ext6, "ReceivingAckToC1Ext6");
		RKH_TR_FWK_OBJ_NAME(CommMgr_ReceivingAckToEndCycleExt7, "ReceivingAckToEndCycleExt7");
		RKH_TR_FWK_OBJ_NAME(CommMgr_SendingEndOfHistToEndCycleExt9, "SendingEndOfHistToEndCycleExt9");
		RKH_TR_FWK_OBJ_NAME(CommMgr_SendingHistToC0Ext12, "SendingHistToC0Ext12");
		RKH_TR_FWK_OBJ_NAME(CommMgr_SendingHistToEndCycleExt13, "SendingHistToEndCycleExt13");
		RKH_TR_FWK_OBJ_NAME(CommMgr_C1ToEndCycleExt14, "C1ToEndCycleExt14");
		RKH_TR_FWK_OBJ_NAME(CommMgr_C3ToSendingHistExt18, "C3ToSendingHistExt18");
		RKH_TR_FWK_OBJ_NAME(CommMgr_activeToactiveLoc0, "activeToactiveLoc0");
		RKH_TR_FWK_OBJ_NAME(CommMgr_enWaitSync, "enWaitSync");
		RKH_TR_FWK_OBJ_NAME(CommMgr_enSendingStatus, "enSendingStatus");
		RKH_TR_FWK_OBJ_NAME(CommMgr_enReceivingAck, "enReceivingAck");
		RKH_TR_FWK_OBJ_NAME(CommMgr_enSendingEndOfHist, "enSendingEndOfHist");
		RKH_TR_FWK_OBJ_NAME(CommMgr_enSendingHist, "enSendingHist");
		RKH_TR_FWK_OBJ_NAME(CommMgr_exWaitSync, "exWaitSync");
		RKH_TR_FWK_OBJ_NAME(CommMgr_isCondC0ToSendingHist11, "isCondC0ToSendingHist11");
		RKH_TR_FWK_OBJ_NAME(CommMgr_isCondC1ToC215, "isCondC1ToC215");
		RKH_TR_FWK_OBJ_NAME(CommMgr_isCondC2ToSendingStatus16, "isCondC2ToSendingStatus16");
		RKH_TR_FWK_OBJ_NAME(CommMgr_isCondC3ToSendingHist18, "isCondC3ToSendingHist18");
	#endif
	
	init();
}

void 
CommMgr_idleToactiveExt1(CommMgr *const me, RKH_EVT_T *pe)
{
	activateSync();
}

void 
CommMgr_ReceivingAckToC1Ext6(CommMgr *const me, RKH_EVT_T *pe)
{
	parseRecv();
}

void 
CommMgr_ReceivingAckToEndCycleExt7(CommMgr *const me, RKH_EVT_T *pe)
{
	recvFail();
}

void 
CommMgr_SendingEndOfHistToEndCycleExt9(CommMgr *const me, RKH_EVT_T *pe)
{
	sendMsgFail();
}

void 
CommMgr_SendingHistToC0Ext12(CommMgr *const me, RKH_EVT_T *pe)
{
	nextSend();
}

void 
CommMgr_SendingHistToEndCycleExt13(CommMgr *const me, RKH_EVT_T *pe)
{
	sendMsgFail();
}

void 
CommMgr_C1ToEndCycleExt14(CommMgr *const me, RKH_EVT_T *pe)
{
	parseError();
}

void 
CommMgr_C3ToSendingHistExt18(CommMgr *const me, RKH_EVT_T *pe)
{
	initSendBlock();
}

void 
CommMgr_activeToactiveLoc0(CommMgr *const me, RKH_EVT_T *pe)
{
	updateStatus();
}

/* ............................. Entry actions ............................. */
void 
CommMgr_enWaitSync(CommMgr *const me)
{
	enWaitSync();
	RKH_SET_STATIC_EVENT(&me->tmEvtObj0, evTout0);
	RKH_TMR_INIT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj0), NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime0);
}

void 
CommMgr_enSendingStatus(CommMgr *const me)
{
	sendStatus();
}

void 
CommMgr_enReceivingAck(CommMgr *const me)
{
	receive();
}

void 
CommMgr_enSendingEndOfHist(CommMgr *const me)
{
	sendEndOfHist();
}

void 
CommMgr_enSendingHist(CommMgr *const me)
{
	sendOneMsg();
}

/* ............................. Exit actions .............................. */
void 
CommMgr_exWaitSync(CommMgr *const me)
{
	exWaitSync();
	rkh_tmr_stop(&me->tmEvtObj0.tmr);
}

/* ................................ Guards ................................. */
rbool_t 
CommMgr_isCondC0ToSendingHist11(CommMgr *const me, RKH_EVT_T *pe)
{
	return (isEndOfBlock()) ? true : false;
}

rbool_t 
CommMgr_isCondC1ToC215(CommMgr *const me, RKH_EVT_T *pe)
{
	return (isAck()) ? true : false;
}

rbool_t 
CommMgr_isCondC2ToSendingStatus16(CommMgr *const me, RKH_EVT_T *pe)
{
	return (isPending()) ? true : false;
}

rbool_t 
CommMgr_isCondC3ToSendingHist18(CommMgr *const me, RKH_EVT_T *pe)
{
	return (isThereMsg()) ? true : false;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
