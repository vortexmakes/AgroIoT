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
#include "YFrame.h"
#include "topic.h"
#include "StatQue.h"
#include "settings.h"
#include "rkhassert.h"
#include "Config.h"

RKH_MODULE_NAME(CommMgrAct);

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
static RKH_ROM_STATIC_EVENT(evRecvObj, evRecv);

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
checkHist(CommMgr *const me)
{
    rui16_t maxNumFrames;

    me->nFramesToSend = StatQue_init();
    maxNumFrames = Config_getMaxNumFramesToSend();
    if (me->nFramesToSend > maxNumFrames)
    {
        me->nFramesToSend = maxNumFrames;
    }
}

void
parseReceived(CommMgr *const me, RKH_EVT_T *pe)
{
    ReceivedEvt *realEvt;

    realEvt = RKH_DOWNCAST(ReceivedEvt, pe);
	me->lastRecvResponse = YFrame_parse(realEvt->buf);
}

void
publishReceive(CommMgr *const me)
{
    topic_publish(TCPConnection, 
                  RKH_CAST(RKH_EVT_T, &evRecvObj), 
                  RKH_UPCAST(RKH_SMA_T, me));
}

/* ............................ Effect actions ............................. */
void 
CommMgr_ToIdleExt0(CommMgr *const me, RKH_EVT_T *pe)
{
	/*init();*/
	RKH_TR_FWK_AO(me);
	RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
	RKH_TR_FWK_STATE(me, &Idle);
	RKH_TR_FWK_STATE(me, &Active);
	RKH_TR_FWK_STATE(me, &WaitSync);
	RKH_TR_FWK_STATE(me, &Current);
	RKH_TR_FWK_STATE(me, &SendingStatus);
	RKH_TR_FWK_STATE(me, &ReceivingStatusAck);
	RKH_TR_FWK_STATE(me, &History);
	RKH_TR_FWK_STATE(me, &SendingEndOfHist);
	RKH_TR_FWK_STATE(me, &SendingHist);
	RKH_TR_FWK_STATE(me, &ReceivingMsgAck);
	RKH_TR_FWK_STATE(me, &SendingStartOfHist);
	RKH_TR_FWK_SIG(evNetConnected);
	RKH_TR_FWK_SIG(evNetDisconnected);
	RKH_TR_FWK_SIG(evSent);
	RKH_TR_FWK_SIG(evReceived);
	RKH_TR_FWK_SIG(evRecvFail);
	RKH_TR_FWK_SIG(evSendFail);
	RKH_TR_FWK_SIG(evGStatus);
	RKH_TR_FWK_TIMER(&me->tmEvtObj0.tmr);
	
    topic_subscribe(Status, RKH_UPCAST(RKH_SMA_T, me));
    topic_subscribe(TCPConnection, RKH_UPCAST(RKH_SMA_T, me));
    me->isPendingStatus = false;
    me->lastRecvResponse = TypeOfRespUnknown;
    RKH_SET_STATIC_EVENT(&me->evSendObj, evSend);
}

void 
CommMgr_IdleToActiveExt1(CommMgr *const me, RKH_EVT_T *pe)
{
	/*activateSync();*/
}

void 
CommMgr_CurrentToWaitSyncExt5(CommMgr *const me, RKH_EVT_T *pe)
{
	/*sendMsgFail();*/
}

void 
CommMgr_CurrentToWaitSyncExt6(CommMgr *const me, RKH_EVT_T *pe)
{
	/*recvFail();*/
}

void 
CommMgr_ReceivingStatusAckToC0Ext9(CommMgr *const me, RKH_EVT_T *pe)
{
	/*parseRecv();*/
	parseReceived(me, pe);
}

void 
CommMgr_C0ToCurrentFinalExt12(CommMgr *const me, RKH_EVT_T *pe)
{
	/*parseError();*/
}

void 
CommMgr_HistoryToWaitSyncExt13(CommMgr *const me, RKH_EVT_T *pe)
{
	/*recvFail();*/
}

void 
CommMgr_HistoryToWaitSyncExt14(CommMgr *const me, RKH_EVT_T *pe)
{
	/*sendMsgFail();*/
}

void 
CommMgr_ToC1Ext16(CommMgr *const me, RKH_EVT_T *pe)
{
	/*checkHist();*/
	checkHist(me);
}

void 
CommMgr_SendingHistToC2Ext18(CommMgr *const me, RKH_EVT_T *pe)
{
	/*nextSend();*/
    --me->framesToSend;
}

void 
CommMgr_ReceivingMsgAckToC3Ext19(CommMgr *const me, RKH_EVT_T *pe)
{
	/*parseRecv();*/
	parseReceived(me, pe);
}

void 
CommMgr_C3ToHistoryFinalExt24(CommMgr *const me, RKH_EVT_T *pe)
{
	/*parseError();*/
}

void 
CommMgr_C3ToC4Ext25(CommMgr *const me, RKH_EVT_T *pe)
{
    /*deleteAcked();*/
    rui16_t nFrames;
    rInt res;

    nFrames = (rui16_t)me->nFramesToSend;
    res = StatQue_delete(&nFrames);
    RKH_ENSURE((nFrames == me->nFramesToSend) && (res == 0));
}

void 
CommMgr_C4ToC1Ext27(CommMgr *const me, RKH_EVT_T *pe)
{
	/*checkHist();*/
	checkHist(me);
}

void 
CommMgr_ActiveToActiveLoc0(CommMgr *const me, RKH_EVT_T *pe)
{
    GStatusEvt *realEvt; 

	/*updateStatus();*/
    realEvt = RKH_DOWNCAST(GStatusEvt, pe);
    me->status = realEvt->status;
    me->isPendingStatus = true;
}

void 
CommMgr_SendingStartOfHistToSendingHistExt30(CommMgr *const me, RKH_EVT_T *pe)
{
	/*sendFirst();*/
    rInt res;
    GPS_STR from;
    GStatus to;

    res = StatQue_read(&from);
    RKH_ENSURE(res == 0);
    res = GStatus_fromGpsStr(&from, &to);
    RKH_ENSURE(res == 0);
    me->evSendObj.size = YFrame_data(&to, me->evSendObj.buf, YFRAME_MGP_TYPE);
}

void 
CommMgr_C1ToSendingHistExt31(CommMgr *const me, RKH_EVT_T *pe)
{
    /*prepareNext();*/
    rInt res;
    GPS_STR from;
    GStatus to;

    res = StatQue_read(&from);
    RKH_ENSURE(res == 0);
    res = GStatus_fromGpsStr(&from, &to);
    RKH_ENSURE(res == 0);
    me->evSendObj.size = YFrame_data(&to, me->evSendObj.buf, YFRAME_MGP_TYPE);
}

/* ............................. Entry actions ............................. */
void 
CommMgr_enWaitSync(CommMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj0, evTout0);
	RKH_TMR_INIT(&me->tmEvtObj0.tmr, 
                 RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj0), 
                 NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj0.tmr, 
                    RKH_UPCAST(RKH_SMA_T, me), 
                    RKH_TIME_SEC(Config_getConnTime()));
}

void 
CommMgr_enSendingStatus(CommMgr *const me)
{
	/*sendStatus();*/
    me->evSendObj.size = YFrame_header(&me->status, me->evSendObj.buf, 0, 
                                       YFRAME_SGP_TYPE);
    me->evSendObj.size += YFrame_data(&me->status,
                                      &me->evSendObj.buf[me->evSendObj.size], 
                                      YFRAME_SGP_TYPE);
    topic_publish(TCPConnection, 
                  RKH_UPCAST(RKH_EVT_T, &me->evSendObj), 
                  RKH_UPCAST(RKH_SMA_T, me));
    me->isPendingStatus = false;
}

void 
CommMgr_enReceivingStatusAck(CommMgr *const me)
{
	/*receive();*/
    publishReceive(me);
}

void 
CommMgr_enSendingEndOfHist(CommMgr *const me)
{
	/*sendEndOfHist();*/
    me->evSendObj.size = YFrame_multipleTail(me->evSendObj.buf);
    topic_publish(TCPConnection, 
                  RKH_UPCAST(RKH_EVT_T, &me->evSendObj), 
                  RKH_UPCAST(RKH_SMA_T, me));
}

void 
CommMgr_enSendingHist(CommMgr *const me)
{
	/*sendOneMsg();*/
    topic_publish(TCPConnection, 
                  RKH_UPCAST(RKH_EVT_T, &me->evSendObj), 
                  RKH_UPCAST(RKH_SMA_T, me));
}

void 
CommMgr_enReceivingMsgAck(CommMgr *const me)
{
	/*receive();*/
    publishReceive(me);
}

void 
CommMgr_enSendingStartOfHist(CommMgr *const me)
{
	/*sendStartOfHist();*/
    me->framesToSend = me->nFramesToSend;
    me->evSendObj.size = YFrame_header(&me->status, me->evSendObj.buf, 
                                       me->nFramesToSend, YFRAME_MGP_TYPE);
    topic_publish(TCPConnection, 
                  RKH_UPCAST(RKH_EVT_T, &me->evSendObj), 
                  RKH_UPCAST(RKH_SMA_T, me));
}

/* ............................. Exit actions .............................. */
void 
CommMgr_exWaitSync(CommMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj0.tmr);
}

/* ................................ Guards ................................. */
rbool_t 
CommMgr_isCondC0ToHistory11(CommMgr *const me, RKH_EVT_T *pe)
{
	/*return (isAck()) ? true : false;*/
	return (me->lastRecvResponse == TypeOfRespAck) ? true : false;
}

rbool_t 
CommMgr_isCondC0ToReceivingStatusAck28(CommMgr *const me, RKH_EVT_T *pe)
{
	/*return (isEmpty()) ? true : false;*/
	return (me->lastRecvResponse == TypeOfRespEmpty) ? true : false;
}

rbool_t 
CommMgr_isCondC1ToSendingHist20(CommMgr *const me, RKH_EVT_T *pe)
{
	/*return (isThereMsg()) ? true : false;*/
	return (me->nFramesToSend != 0) ? true : false;
}

rbool_t 
CommMgr_isCondC2ToSendingEndOfHist23(CommMgr *const me, RKH_EVT_T *pe)
{
	/*return (isEndOfBlock()) ? true : false;*/
	return (me->framesToSend == 0) ? true : false;
}

rbool_t 
CommMgr_isCondC3ToC425(CommMgr *const me, RKH_EVT_T *pe)
{
	/*return (isAck()) ? true : false;*/
	return (me->lastRecvResponse == TypeOfRespAck) ? true : false;
}

rbool_t 
CommMgr_isCondC3ToReceivingMsgAck29(CommMgr *const me, RKH_EVT_T *pe)
{
	/*return (isEmpty()) ? true : false;*/
	return (me->lastRecvResponse == TypeOfRespEmpty) ? true : false;
}

rbool_t 
CommMgr_isCondC4ToCurrent26(CommMgr *const me, RKH_EVT_T *pe)
{
	/*return (isPending()) ? true : false;*/
	return (me->isPendingStatus == true) ? true : false;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
