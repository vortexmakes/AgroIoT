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
#include "events.h"
#include "CommMgr.h"
#include "CommMgrAct.h"
#include "CommMgrActRequired.h"

/* ----------------------------- Local macros ------------------------------ */
#define WaitTime0	RKH_TIME_SEC(60)

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Effect actions ............................. */
void 
CommMgr_ToIdleExt0(CommMgr *const me, RKH_EVT_T *pe)
{
	/*init();*/
	RKH_TR_FWK_AO(me);
	RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
	RKH_TR_FWK_STATE(me, &CommMgr_Idle);
	RKH_TR_FWK_STATE(me, &CommMgr_Active);
	RKH_TR_FWK_STATE(me, &CommMgr_WaitSync);
	RKH_TR_FWK_STATE(me, &CommMgr_SendingStatus);
	RKH_TR_FWK_STATE(me, &CommMgr_ReceivingAck);
	RKH_TR_FWK_STATE(me, &CommMgr_SendingEndOfHist);
	RKH_TR_FWK_STATE(me, &CommMgr_SendingHist);
	RKH_TR_FWK_STATE(me, &CommMgr_EndCycle);
	RKH_TR_FWK_SIG(evNetConnected);
	RKH_TR_FWK_SIG(evNetDisconnected);
	RKH_TR_FWK_SIG(evSent);
	RKH_TR_FWK_SIG(evReceived);
	RKH_TR_FWK_SIG(evRecvFail);
	RKH_TR_FWK_SIG(evSendFail);
	RKH_TR_FWK_SIG(evGStatus);
	RKH_TR_FWK_TIMER(&me->tmEvtObj0.tmr);
	
    me->isPendingStatus = 0;
}

void 
CommMgr_idleToactiveExt1(CommMgr *const me, RKH_EVT_T *pe)
{
	/*activateSync();*/
}

void 
CommMgr_ReceivingAckToC1Ext6(CommMgr *const me, RKH_EVT_T *pe)
{
	/*parseRecv();*/
}

void 
CommMgr_ReceivingAckToEndCycleExt7(CommMgr *const me, RKH_EVT_T *pe)
{
	/*recvFail();*/
}

void 
CommMgr_SendingEndOfHistToEndCycleExt9(CommMgr *const me, RKH_EVT_T *pe)
{
	/*sendMsgFail();*/
}

void 
CommMgr_SendingHistToC0Ext12(CommMgr *const me, RKH_EVT_T *pe)
{
	/*nextSend();*/
}

void 
CommMgr_SendingHistToEndCycleExt13(CommMgr *const me, RKH_EVT_T *pe)
{
	/*sendMsgFail();*/
}

void 
CommMgr_C1ToEndCycleExt14(CommMgr *const me, RKH_EVT_T *pe)
{
	/*parseError();*/
}

void 
CommMgr_C3ToSendingHistExt18(CommMgr *const me, RKH_EVT_T *pe)
{
	/*initSendBlock();*/
}

void 
CommMgr_ActiveToActiveLoc0(CommMgr *const me, RKH_EVT_T *pe)
{
    GStatusEvt *realEvt;

	/*updateStatus();*/
    realEvt = RKH_DOWNCAST(GStatusEvt, pe);
    me->status = realEvt->status;
    me->isPendingStatus = 1;
}

/* ............................. Entry actions ............................. */
void 
CommMgr_enWaitSync(CommMgr *const me)
{
	/*enWaitSync();*/
	RKH_SET_STATIC_EVENT(&me->tmEvtObj0, evTout0);
	RKH_TMR_INIT(&me->tmEvtObj0.tmr, 
                 RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj0), 
                 NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime0);
}

void 
CommMgr_enSendingStatus(CommMgr *const me)
{
	/*sendStatus();*/
}

void 
CommMgr_enReceivingAck(CommMgr *const me)
{
	/*receive();*/
}

void 
CommMgr_enSendingEndOfHist(CommMgr *const me)
{
	/*sendEndOfHist();*/
}

void 
CommMgr_enSendingHist(CommMgr *const me)
{
	/*sendOneMsg();*/
}

/* ............................. Exit actions .............................. */
void 
CommMgr_exWaitSync(CommMgr *const me)
{
	/*exWaitSync();*/
	rkh_tmr_stop(&me->tmEvtObj0.tmr);
}

/* ................................ Guards ................................. */
rbool_t 
CommMgr_isCondC0ToSendingHist11(CommMgr *const me, RKH_EVT_T *pe)
{
	/*return (isEndOfBlock()) ? true : false;*/
}

rbool_t 
CommMgr_isCondC1ToC215(CommMgr *const me, RKH_EVT_T *pe)
{
	/*return (isAck()) ? true : false;*/
}

rbool_t 
CommMgr_isCondC2ToSendingStatus16(CommMgr *const me, RKH_EVT_T *pe)
{
	/*return (isPending()) ? true : false;*/
}

rbool_t 
CommMgr_isCondC3ToSendingHist18(CommMgr *const me, RKH_EVT_T *pe)
{
	/*return (isThereMsg()) ? true : false;*/
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
