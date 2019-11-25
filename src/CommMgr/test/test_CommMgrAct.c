/**
 *  \file       test_CommMgrAct.c
 *  \brief      Unit test for state machine module.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina db@vortexmakes.com
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include "unity.h"
#include "CommMgr.h"
#include "CommMgrAct.h"
#include "signals.h"
#include "events.h"
#include "rkhfwk_cast.h"
#include "Mock_rkhtmr.h"
#include "Mock_rkhtrc_record.h"
#include "Mock_rkhassert.h"
#include "Mock_rkhsma.h"
#include "Mock_GStatus.h"
#include "Mock_YFrame.h"
#include "Mock_topic.h"
#include "Mock_StatQue.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
RKHROM RKH_SBSC_T Idle, WaitSync, SendingStatus, ReceivingStatusAck, 
                     SendingEndOfHist, SendingHist, ReceivingMsgAck;
RKHROM RKH_SCMP_T Active, Current, History;
RKHROM RKH_SCHOICE_T C0, C1, C2, C3, C4;
RKHROM RKH_FINAL_T CurrentFinal, HistoryFinal;

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
RKH_SMA_CREATE(CommMgr, commMgr, 0, HCAL, NULL, CommMgr_ToIdleExt0, NULL);
RKH_SMA_DEF_PTR(commMgr);
static CommMgr *me;
static RKH_EVT_T evtObj, *evt;
const RKHSmaVtbl rkhSmaVtbl =  /* Instantiate it because rkhsma is mocked */
{
    rkh_sma_activate,
    rkh_sma_dispatch,
    rkh_sma_post_fifo,
    rkh_sma_post_lifo
};
static SendEvt evSendObj;
static ReceivedEvt evReceivedObj;
static GStatus gStatus =
{
    {
        "185124", "A", "37.8402883", "-", "057.6884350", "-", "0.078",
        "", "310119"
    },
    {1, 1, {0, 0, 1}, {0xdddd, 0xffff, 0xffff}, 0},
    {0xff, 0x3f},
    3
};
static const char singleFrame[] =
    "!0|19355826018345180,185124,-37.8402883,-057.6884350,0.078,,310119,3FFF,0000,00,00,DDDD,FFFF,FFFF,3";

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
    me = RKH_DOWNCAST(CommMgr, commMgr);
    evt = &evtObj;
    RKH_SET_STATIC_EVENT(&evSendObj, evSend);
    RKH_SET_STATIC_EVENT(&evReceivedObj, evReceived);
}

void
tearDown(void)
{
}

void
test_Initialize(void)
{
    rkh_trc_ao_Ignore();
    rkh_trc_obj_Ignore();
    rkh_trc_state_Ignore();
    rkh_trc_state_Ignore();
    rkh_trc_state_Ignore();
    rkh_trc_state_Ignore();
    rkh_trc_state_Ignore();
    rkh_trc_state_Ignore();
    rkh_trc_state_Ignore();
    rkh_trc_state_Ignore();
    rkh_trc_sig_Ignore();
    rkh_trc_sig_Ignore();
    rkh_trc_sig_Ignore();
    rkh_trc_sig_Ignore();
    rkh_trc_sig_Ignore();
    rkh_trc_sig_Ignore();
    rkh_trc_sig_Ignore();
    rkh_trc_obj_Ignore();
    topic_subscribe_Expect(Status, RKH_UPCAST(RKH_SMA_T, me));

    CommMgr_ToIdleExt0(me, evt);

    TEST_ASSERT_EQUAL(false, me->isPendingStatus);
    TEST_ASSERT_EQUAL(true, me->isHistEmpty);
    TEST_ASSERT_EQUAL(TypeOfRespUnknown, me->lastRecvResponse);
}

void
test_StartSync(void)
{
    rkh_tmr_init__Expect(&me->tmEvtObj0.tmr,
                         RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj0));
    rkh_tmr_start_Expect(&me->tmEvtObj0.tmr,
                         RKH_UPCAST(RKH_SMA_T, me),
                         RKH_TIME_SEC(60), 0);
    CommMgr_enWaitSync(me);
    TEST_ASSERT_EQUAL(evTout0, me->tmEvtObj0.evt.e);
}

void
test_StopSync(void)
{
    rkh_tmr_stop_ExpectAndReturn(&me->tmEvtObj0.tmr, 0);
    CommMgr_exWaitSync(me);
}

void
test_UpdateStatus(void)
{
    GStatusEvt event;

    event.status.ioStatus.digIn = 0xde;
    event.status.ioStatus.digOut = 0xad;

    CommMgr_ActiveToActiveLoc0(me, RKH_UPCAST(RKH_EVT_T, &event));

    TEST_ASSERT_EQUAL_MEMORY(&event.status, &me->status, sizeof(GStatus));
    TEST_ASSERT_EQUAL(1, me->isPendingStatus);
}

void
test_SendCurrentStatus(void)
{
    ruint frameLen, len, headerLen;

    me->status = gStatus;
    frameLen = strlen(singleFrame);
    headerLen = strlen("!0|19355826018345180,");

    YFrame_header_ExpectAndReturn(&me->status, evSendObj.buf, 0, 
                                  YFRAME_SGP_TYPE, headerLen);
    YFrame_header_IgnoreArg_to();
    YFrame_data_ExpectAndReturn(&me->status, &evSendObj.buf[len], 
                                YFRAME_SGP_TYPE, frameLen);
    YFrame_data_IgnoreArg_to();
    topic_publish_Expect(TCPConnection, 
                         RKH_UPCAST(RKH_EVT_T, &evSendObj), 
                         RKH_UPCAST(RKH_SMA_T, me));
    topic_publish_IgnoreArg_evt();

    CommMgr_enSendingStatus(me);
}

void
test_CheckPendingStatus(void)
{
    rbool_t res;

    me->isPendingStatus = true;
    res = CommMgr_isCondC4ToCurrent26(me, RKH_UPCAST(RKH_EVT_T, &evSendObj));
    TEST_ASSERT_TRUE(res == true);

    me->isPendingStatus = false;
    res = CommMgr_isCondC4ToCurrent26(me, RKH_UPCAST(RKH_EVT_T, &evSendObj));
    TEST_ASSERT_TRUE(res == false);
}

void
test_ReceiveAck(void)
{
    rbool_t res;

    strcpy(evReceivedObj.buf, "!2|");   /* in ConnMgr */
    evReceivedObj.size = strlen("!2|");

    YFrame_parse_ExpectAndReturn(evReceivedObj.buf, TypeOfRespAck);
    CommMgr_ReceivingStatusAckToC0Ext9(me, 
                                       RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_EQUAL(TypeOfRespAck, me->lastRecvResponse);

    res = CommMgr_isCondC3ToC425(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_TRUE(res == true);

    res = CommMgr_isCondC0ToHistory11(me, 
                                      RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_TRUE(res == true);
}

void
test_ReceiveUnknownResponse(void)
{
    rbool_t res;

    strcpy(evReceivedObj.buf, "!2|");   /* in ConnMgr */
    evReceivedObj.size = strlen("!2|");

    YFrame_parse_ExpectAndReturn(evReceivedObj.buf, TypeOfRespUnknown);
    CommMgr_ReceivingStatusAckToC0Ext9(me, 
                                       RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_EQUAL(TypeOfRespUnknown, me->lastRecvResponse);

    res = CommMgr_isCondC3ToC425(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_TRUE(res == false);
}

void
test_CheckHistory(void)
{
    strcpy(evReceivedObj.buf, "!2|");   /* in ConnMgr */
    evReceivedObj.size = strlen("!2|");

    StatQue_isEmpty_ExpectAndReturn(true);
    CommMgr_ToC1Ext16(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));

    StatQue_isEmpty_ExpectAndReturn(true);
    CommMgr_C4ToC1Ext27(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
}

void
test_StartHistoryMessage(void)
{
    TEST_IGNORE_MESSAGE(__FUNCTION__);
}

void
test_PrepareNextStatusBlock(void)
{
    TEST_IGNORE_MESSAGE(__FUNCTION__);
}

void
test_SendABlockOfStatus(void)
{
    TEST_IGNORE_MESSAGE(__FUNCTION__);
}

void
test_CheckEndOfBlock(void)
{
    TEST_IGNORE_MESSAGE(__FUNCTION__);
}

void
test_EndHistoryMessage(void)
{
    TEST_IGNORE_MESSAGE(__FUNCTION__);
}

void
test_FailToSend(void)
{
    TEST_IGNORE_MESSAGE(__FUNCTION__);
}

void
test_FailToReceive(void)
{
    TEST_IGNORE_MESSAGE(__FUNCTION__);
}

/* ------------------------------ End of file ------------------------------ */
