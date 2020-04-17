/**
 *  \file       CommMgrAct.c
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
#include "geoMgr.h"
#include "Trace.h"

RKH_MODULE_NAME(CommMgrAct);

/* ----------------------------- Local macros ------------------------------ */
#define MAX_NUM_RECV_TRIES      16
#define GEO_INVALID_GEOSTAMP    \
    { \
        {GEO_INVALID_UTC}, {RMC_StatusInvalid}, \
        {GEO_INVALID_LATITUDE}, {GEO_INVALID_LATITUDE_IND}, \
        {GEO_INVALID_LONGITUDE}, {GEO_INVALID_LONGITUDE_IND}, \
        {GEO_INVALID_SPEED}, {GEO_INVALID_COURSE}, \
        {GEO_INVALID_DATE} \
    }

/* ------------------------------- Constants ------------------------------- */
static RKH_ROM_STATIC_EVENT(evRecvObj, evRecv);
static RKH_ROM_STATIC_EVENT(evRestartObj, evRestart);

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

static void
sendFrames(CommMgr *const me)
{
    rInt res;
    GStatus to;
    int i;

    me->nFramesPerMsg = (me->framesToSend > MAX_NUM_FRAMES_PER_MSG) ? 
                                                    MAX_NUM_FRAMES_PER_MSG :
                                                    me->framesToSend;
    me->evSendObj.size = 0;
    for (i = 0; i < me->nFramesPerMsg; ++i)
    {
        res = StatQue_read(&to);
        RKH_ENSURE(res == 0);
        res = (bool)GStatus_checkValidity(&to);
        if (res == false)
        {
            Trace_generate(&to.data, TraceId_CorruptStatus, 0, 0);
        }
        me->evSendObj.size += YFrame_data(&to.data, 
                                    me->evSendObj.buf + me->evSendObj.size, 
                                    YFRAME_MGP_TYPE);
    }
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
    RKH_TR_FWK_STATE(me, &CurrentFinal);
    RKH_TR_FWK_STATE(me, &HistoryFinal);
    RKH_TR_FWK_PSTATE(me, &C0);
    RKH_TR_FWK_PSTATE(me, &C1);
    RKH_TR_FWK_PSTATE(me, &C2);
    RKH_TR_FWK_PSTATE(me, &C3);
    RKH_TR_FWK_PSTATE(me, &C4);
    RKH_TR_FWK_PSTATE(me, &C5);
    RKH_TR_FWK_PSTATE(me, &C6);
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
    topic_publish(TCPConnection, 
                  RKH_UPCAST(RKH_EVT_T, &evRestartObj), 
                  RKH_UPCAST(RKH_SMA_T, me));
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
    me->framesToSend -= me->nFramesPerMsg;
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
CommMgr_C3ToC5Ext32(CommMgr *const me, RKH_EVT_T *pe)
{
    /*checkRecvTries();*/
    --me->nRecvTries;
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
    sendFrames(me);
}

void
CommMgr_C2ToSendingHistExt31(CommMgr *const me, RKH_EVT_T *pe)
{
    /*prepareNext();*/
    sendFrames(me);
}

void
CommMgr_SendingEndOfHistToReceivingMsgAckExt34(CommMgr *const me, RKH_EVT_T *pe)
{
    /*setReceive();*/
    me->nRecvTries = MAX_NUM_RECV_TRIES;
}

void
CommMgr_C0ToC6Ext35(CommMgr *const me, RKH_EVT_T *pe)
{
    /*checkRecvTries();*/
    --me->nRecvTries;
}

void
CommMgr_SendingStatusToReceivingiStatusAckExt37(CommMgr *const me,
                                                RKH_EVT_T *pe)
{
    /*setReceive();*/
    me->nRecvTries = MAX_NUM_RECV_TRIES;
}

void
CommMgr_C5ToHistoryFinalExt38(CommMgr *const me, RKH_EVT_T *pe)
{
    /*reOpenConnection();*/
    topic_publish(TCPConnection, 
                  RKH_UPCAST(RKH_EVT_T, &evRestartObj), 
                  RKH_UPCAST(RKH_SMA_T, me));
}

void
CommMgr_C6ToCurrentFinalExt39(CommMgr *const me, RKH_EVT_T *pe)
{
    /*reOpenConnection();*/
    topic_publish(TCPConnection, 
                  RKH_UPCAST(RKH_EVT_T, &evRestartObj), 
                  RKH_UPCAST(RKH_SMA_T, me));
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
CommMgr_isCondC0ToC628(CommMgr *const me, RKH_EVT_T *pe)
{
    /*return (isEmpty()) ? true : false;*/
    return (me->lastRecvResponse == TypeOfRespEmpty) ? true : false;
}

rbool_t
CommMgr_isCondC1ToSendingStartOfHist20(CommMgr *const me, RKH_EVT_T *pe)
{
    /*return (isThereMsg()) ? true : false;*/
    return (me->nFramesToSend > MIN_NUM_STORED_FRAMES_TO_SEND) ? true : false;
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
CommMgr_isCondC3ToC529(CommMgr *const me, RKH_EVT_T *pe)
{
    /*return (isEmpty()) ? true : false;*/
    return (me->lastRecvResponse == TypeOfRespEmpty) ? true : false;
}

rbool_t
CommMgr_isCondC5ToHistoryFinal33(CommMgr *const me, RKH_EVT_T *pe)
{
    /*return (isMaxRecvTries()) ? true : false;*/
    return (me->nRecvTries == 0) ? true : false;
}

rbool_t
CommMgr_isCondC4ToCurrent26(CommMgr *const me, RKH_EVT_T *pe)
{
    /*return (isPending()) ? true : false;*/
    return (me->isPendingStatus == true) ? true : false;
}

rbool_t
CommMgr_isCondC6ToCurrentFinal36(CommMgr *const me, RKH_EVT_T *pe)
{
    /*return (isMaxRecvTries()) ? true : false;*/
    return (me->nRecvTries == 0) ? true : false;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
