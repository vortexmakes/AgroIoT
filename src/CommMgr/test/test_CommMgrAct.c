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
#include "Mock_rkhsma.h"
#include "Mock_GStatus.h"
#include "Mock_YFrame.h"
#include "Mock_topic.h"
#include "Mock_StatQue.h"
#include "Mock_rkhassert.h"
#include "Mock_Config.h"
#include "Mock_geoMgr.h"

/* ----------------------------- Local macros ------------------------------ */
#define GEO_INVALID_GEOSTAMP    \
    { \
        {GEO_INVALID_UTC}, {RMC_StatusInvalid}, \
        {GEO_INVALID_LATITUDE}, {GEO_INVALID_LATITUDE_IND}, \
        {GEO_INVALID_LONGITUDE}, {GEO_INVALID_LONGITUDE_IND}, \
        {GEO_INVALID_SPEED}, {GEO_INVALID_COURSE}, \
        {GEO_INVALID_DATE} \
    }

/* ------------------------------- Constants ------------------------------- */
RKHROM RKH_SBSC_T Idle, WaitSync, SendingStatus, ReceivingStatusAck, 
                     SendingEndOfHist, SendingHist, ReceivingMsgAck,
                     SendingStartOfHist;
RKHROM RKH_SCMP_T Active, Current, History;
RKHROM RKH_SCHOICE_T C0, C1, C2, C3, C4, C5, C6;
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
static ReceivedEvt evReceivedObj;
static GStatusType gStatus =
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
static const char frameData[] =
    "|1b,185124,-37.8402883,-057.6884350,0.078,,310119,3FFF,0000,00,00,DDDD,FFFF,FFFF,3";
static const char invalidFrameData[] =
    "|1b,185124,-37.8402883,-057.6884350,0.078,,310119,3FFF,0000,00,00,DDDD,FFFF,FFFF,3";
static const Geo invalidPosition = GEO_INVALID_GEOSTAMP;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
MockAssertCallback(const char* const file, int line, int cmock_num_calls)
{
    TEST_PASS();
}

static ruint
MockYFrameDataCallback(GStatusType *from, char *to, rInt type, 
                       int cmock_num_calls)
{
    TEST_ASSERT_EQUAL_MEMORY(&from->position, &invalidPosition, sizeof(Geo));
    return strlen(invalidFrameData);
}

static int
setupForSendingABlockOfFrames(ruint nFrames, int len, bool validity)
{
    GStatus elem;
    int n, i;

    me->framesToSend = nFrames;
    n = (me->framesToSend > MAX_NUM_FRAMES_PER_MSG) ? 
                                MAX_NUM_FRAMES_PER_MSG :
                                me->framesToSend;
    for (i = 0; i < n; ++i)
    {
        StatQue_read_ExpectAndReturn(&elem, 0);
        StatQue_read_IgnoreArg_elem();
        GStatus_checkValidity_ExpectAndReturn(0, validity);
        GStatus_checkValidity_IgnoreArg_me();
        YFrame_data_ExpectAndReturn(&elem.data, 
                                    me->evSendObj.buf + me->evSendObj.size, 
                                    YFRAME_MGP_TYPE, len);
        YFrame_data_IgnoreArg_from(); 
        if (validity == false)
        {
            YFrame_data_StubWithCallback(MockYFrameDataCallback);
        }
    }
    return n;
}

/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
    me = RKH_DOWNCAST(CommMgr, commMgr);
    evt = &evtObj;
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
    topic_subscribe_Expect(TCPConnection, RKH_UPCAST(RKH_SMA_T, me));

    CommMgr_ToIdleExt0(me, evt);

    TEST_ASSERT_EQUAL(false, me->isPendingStatus);
    TEST_ASSERT_EQUAL(TypeOfRespUnknown, me->lastRecvResponse);
    TEST_ASSERT_EQUAL(evSend, me->evSendObj.evt.e);
}

void
test_StartSync(void)
{
    rkh_tmr_init__Expect(&me->tmEvtObj0.tmr,
                         RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj0));
    Config_getConnTime_ExpectAndReturn(60);
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

    TEST_ASSERT_EQUAL_MEMORY(&event.status, &me->status, sizeof(GStatusType));
    TEST_ASSERT_TRUE(me->isPendingStatus == true);
}

void
test_SendCurrentStatus(void)
{
    ruint frameLen, headerLen;

    me->status = gStatus;
    frameLen = strlen(singleFrame);
    headerLen = strlen("!0|19355826018345180,");

    YFrame_header_ExpectAndReturn(&me->status, me->evSendObj.buf, 0, 
                                  YFRAME_SGP_TYPE, headerLen);
    YFrame_data_ExpectAndReturn(&me->status, &me->evSendObj.buf[headerLen], 
                                YFRAME_SGP_TYPE, frameLen);
    topic_publish_Expect(TCPConnection, 
                         RKH_UPCAST(RKH_EVT_T, &me->evSendObj), 
                         RKH_UPCAST(RKH_SMA_T, me));

    CommMgr_enSendingStatus(me);
    TEST_ASSERT_TRUE(me->isPendingStatus == false);
}

void
test_Receive(void)
{
    RKH_EVT_T evRecvObj;

    topic_publish_Expect(TCPConnection, &evRecvObj, RKH_UPCAST(RKH_SMA_T, me));
    topic_publish_IgnoreArg_evt();

    CommMgr_enReceivingStatusAck(me);

    topic_publish_Expect(TCPConnection, &evRecvObj, RKH_UPCAST(RKH_SMA_T, me));
    topic_publish_IgnoreArg_evt();

    CommMgr_enReceivingMsgAck(me);
}

void
test_CheckPendingStatus(void)
{
    rbool_t res;

    me->isPendingStatus = true;
    res = CommMgr_isCondC4ToCurrent26(me, RKH_UPCAST(RKH_EVT_T, 
                                                     &me->evSendObj));
    TEST_ASSERT_TRUE(res == true);

    me->isPendingStatus = false;
    res = CommMgr_isCondC4ToCurrent26(me, RKH_UPCAST(RKH_EVT_T, 
                                                     &me->evSendObj));
    TEST_ASSERT_TRUE(res == false);
}

void
test_ReceiveAck(void)
{
    rbool_t res;

    strcpy(evReceivedObj.buf, "!2|");   /* in ConnMgr */
    evReceivedObj.size = strlen(evReceivedObj.buf);

    YFrame_parse_ExpectAndReturn(evReceivedObj.buf, TypeOfRespAck);
    CommMgr_ReceivingMsgAckToC3Ext19(me, 
                                     RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_EQUAL(TypeOfRespAck, me->lastRecvResponse);

    res = CommMgr_isCondC3ToC425(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_TRUE(res == true);

    YFrame_parse_ExpectAndReturn(evReceivedObj.buf, TypeOfRespAck);
    CommMgr_ReceivingStatusAckToC0Ext9(me, 
                                       RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_EQUAL(TypeOfRespAck, me->lastRecvResponse);

    res = CommMgr_isCondC0ToHistory11(me, 
                                      RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_TRUE(res == true);
}

void
test_ReceiveEmptyResponse(void)
{
    rbool_t res;
    
    strcpy(evReceivedObj.buf, "");   /* in ConnMgr */
    evReceivedObj.size = strlen(evReceivedObj.buf);

    YFrame_parse_ExpectAndReturn(evReceivedObj.buf, TypeOfRespEmpty);
    CommMgr_ReceivingStatusAckToC0Ext9(me, 
                                       RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_EQUAL(TypeOfRespEmpty, me->lastRecvResponse);

    res = CommMgr_isCondC0ToC628(me, 
                                 RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_TRUE(res == true);

    YFrame_parse_ExpectAndReturn(evReceivedObj.buf, TypeOfRespEmpty);
    CommMgr_ReceivingMsgAckToC3Ext19(me, 
                                     RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_EQUAL(TypeOfRespEmpty, me->lastRecvResponse);

    res = CommMgr_isCondC3ToC529(me, 
                                 RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_TRUE(res == true);
}

void
test_DeleteAckedSentMessage(void)
{
    rui16_t nFrames;

    nFrames = (rui16_t)me->nFramesToSend;
    StatQue_delete_ExpectAndReturn(&nFrames, 0);
    StatQue_delete_IgnoreArg_nElem();

    CommMgr_C3ToC4Ext25(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
}

void
test_FailToDeleteAckedSentFrames(void)
{
    rui16_t nFrames;

    nFrames = (rui16_t)me->nFramesToSend;
    StatQue_delete_ExpectAndReturn(&nFrames, 1);
    StatQue_delete_IgnoreArg_nElem();
    rkh_assert_Expect("CommMgrAct", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);


    CommMgr_C3ToC4Ext25(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
}

void
test_ReceiveUnknownResponse(void)
{
    rbool_t res;

    strcpy(evReceivedObj.buf, "!2|");   /* in ConnMgr */
    evReceivedObj.size = strlen(evReceivedObj.buf);

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
    evReceivedObj.size = strlen(evReceivedObj.buf);

    StatQue_init_ExpectAndReturn(8);
    Config_getMaxNumFramesToSend_ExpectAndReturn(100);
    CommMgr_ToC1Ext16(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    TEST_ASSERT_EQUAL(8, me->nFramesToSend);
}

void
test_CheckHistoryEmpty(void)
{
    rbool_t res;
    int nStoredFrames;

    strcpy(evReceivedObj.buf, "!2|");   /* in ConnMgr */
    evReceivedObj.size = strlen(evReceivedObj.buf);

    nStoredFrames = 0;
    StatQue_init_ExpectAndReturn(nStoredFrames);
    Config_getMaxNumFramesToSend_ExpectAndReturn(100);
    CommMgr_ToC1Ext16(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    res = CommMgr_isCondC1ToSendingStartOfHist20(me, 
                                          RKH_UPCAST(RKH_EVT_T, 
                                                     &evReceivedObj));
    TEST_ASSERT_TRUE(res == false);
    TEST_ASSERT_EQUAL(nStoredFrames, me->nFramesToSend);

    nStoredFrames = 8;
    StatQue_init_ExpectAndReturn(nStoredFrames);
    Config_getMaxNumFramesToSend_ExpectAndReturn(100);
    CommMgr_ToC1Ext16(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    res = CommMgr_isCondC1ToSendingStartOfHist20(me, 
                                          RKH_UPCAST(RKH_EVT_T, 
                                                     &evReceivedObj));
    TEST_ASSERT_TRUE(res == true);
    TEST_ASSERT_EQUAL(nStoredFrames, me->nFramesToSend);
}

void
test_CheckHistoryNoEmpty(void)
{
    rbool_t res;
    int nStoredFrames;

    strcpy(evReceivedObj.buf, "!2|");   /* in ConnMgr */
    evReceivedObj.size = strlen(evReceivedObj.buf);

    nStoredFrames = 2;
    StatQue_init_ExpectAndReturn(nStoredFrames);
    Config_getMaxNumFramesToSend_ExpectAndReturn(100);
    CommMgr_ToC1Ext16(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    res = CommMgr_isCondC1ToSendingStartOfHist20(me, 
                                          RKH_UPCAST(RKH_EVT_T, 
                                                     &evReceivedObj));
    TEST_ASSERT_TRUE(res == true);
    TEST_ASSERT_EQUAL(nStoredFrames, me->nFramesToSend);
}

void
test_CheckHistoryWithMinNumOfStoredFrames(void)
{
    rbool_t res;
    int nStoredFrames;

    strcpy(evReceivedObj.buf, "!2|");   /* in ConnMgr */
    evReceivedObj.size = strlen(evReceivedObj.buf);

    nStoredFrames = MIN_NUM_STORED_FRAMES_TO_SEND - 1;
    StatQue_init_ExpectAndReturn(nStoredFrames);
    Config_getMaxNumFramesToSend_ExpectAndReturn(100);
    CommMgr_ToC1Ext16(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    res = CommMgr_isCondC1ToSendingStartOfHist20(me, 
                                          RKH_UPCAST(RKH_EVT_T, 
                                                     &evReceivedObj));
    TEST_ASSERT_TRUE(res == false);
    TEST_ASSERT_EQUAL(nStoredFrames, me->nFramesToSend);

    nStoredFrames = MIN_NUM_STORED_FRAMES_TO_SEND;
    StatQue_init_ExpectAndReturn(nStoredFrames);
    Config_getMaxNumFramesToSend_ExpectAndReturn(100);
    CommMgr_ToC1Ext16(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    res = CommMgr_isCondC1ToSendingStartOfHist20(me, 
                                          RKH_UPCAST(RKH_EVT_T, 
                                                     &evReceivedObj));
    TEST_ASSERT_TRUE(res == false);
    TEST_ASSERT_EQUAL(nStoredFrames, me->nFramesToSend);

    nStoredFrames = MIN_NUM_STORED_FRAMES_TO_SEND + 1;
    StatQue_init_ExpectAndReturn(nStoredFrames);
    Config_getMaxNumFramesToSend_ExpectAndReturn(100);
    CommMgr_ToC1Ext16(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    res = CommMgr_isCondC1ToSendingStartOfHist20(me, 
                                          RKH_UPCAST(RKH_EVT_T, 
                                                     &evReceivedObj));
    TEST_ASSERT_TRUE(res == true);
    TEST_ASSERT_EQUAL(nStoredFrames, me->nFramesToSend);
}

void
test_CheckHistoryMaxFramesToSend(void)
{
    rbool_t res;
    int nStoredFrames;

    strcpy(evReceivedObj.buf, "!2|");   /* in ConnMgr */
    evReceivedObj.size = strlen(evReceivedObj.buf);

    nStoredFrames = 100 + 1;
    StatQue_init_ExpectAndReturn(nStoredFrames);
    Config_getMaxNumFramesToSend_ExpectAndReturn(100);
    CommMgr_ToC1Ext16(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    res = CommMgr_isCondC1ToSendingStartOfHist20(me, 
                                          RKH_UPCAST(RKH_EVT_T, 
                                                     &evReceivedObj));
    TEST_ASSERT_TRUE(res == true);
    TEST_ASSERT_EQUAL(MAX_NFRAMES_TOSEND, me->nFramesToSend);
}

void
test_StartHistoryMessage(void)
{
    ruint headerLen;

    me->status = gStatus;
    headerLen = strlen("!1|0002|355826018345180");

    StatQue_init_ExpectAndReturn(2);
    Config_getMaxNumFramesToSend_ExpectAndReturn(100);
    CommMgr_ToC1Ext16(me, RKH_UPCAST(RKH_EVT_T, &evReceivedObj));
    YFrame_header_ExpectAndReturn(&me->status, me->evSendObj.buf,
                                  me->nFramesToSend, 
                                  YFRAME_MGP_TYPE, headerLen);
    topic_publish_Expect(TCPConnection, 
                         RKH_UPCAST(RKH_EVT_T, &me->evSendObj), 
                         RKH_UPCAST(RKH_SMA_T, me));

    CommMgr_enSendingStartOfHist(me);

    TEST_ASSERT_EQUAL(me->nFramesToSend, me->framesToSend);
    TEST_ASSERT_EQUAL(headerLen, me->evSendObj.size);
}

void
test_DecrementNumFrameToSend(void)
{
    ruint nFrames;

    me->framesToSend = nFrames = 64;
    me->nFramesPerMsg = MAX_NUM_FRAMES_PER_MSG;
    CommMgr_SendingHistToC2Ext18(me, evt);
    TEST_ASSERT_EQUAL(nFrames - me->nFramesPerMsg, me->framesToSend);
}

void
test_SendABlockOfFrames(void)
{
    rbool_t res;
    ruint nFrames;
    int n, len;
    bool validity;

    len = strlen(frameData);
    validity = true;

    nFrames = MAX_NUM_FRAMES_PER_MSG;
    n = setupForSendingABlockOfFrames(nFrames, len, validity);
    CommMgr_SendingStartOfHistToSendingHistExt30(me, evt);
    CommMgr_SendingHistToC2Ext18(me, evt);
    res = CommMgr_isCondC2ToSendingEndOfHist23(me, evt);
    TEST_ASSERT_EQUAL(n, me->nFramesPerMsg);
    TEST_ASSERT_EQUAL(len * n, me->evSendObj.size);
    TEST_ASSERT_EQUAL(nFrames - me->nFramesPerMsg, me->framesToSend);
    TEST_ASSERT_TRUE(res == true);

    nFrames = MAX_NUM_FRAMES_PER_MSG - 1;
    n = setupForSendingABlockOfFrames(nFrames, len, validity);
    CommMgr_SendingStartOfHistToSendingHistExt30(me, evt);
    CommMgr_SendingHistToC2Ext18(me, evt);
    res = CommMgr_isCondC2ToSendingEndOfHist23(me, evt);
    TEST_ASSERT_EQUAL(n, me->nFramesPerMsg);
    TEST_ASSERT_EQUAL(len * n, me->evSendObj.size);
    TEST_ASSERT_EQUAL(nFrames - me->nFramesPerMsg, me->framesToSend);
    TEST_ASSERT_TRUE(res == true);

    nFrames = MAX_NUM_FRAMES_PER_MSG + 1;
    n = setupForSendingABlockOfFrames(nFrames, len, validity);
    CommMgr_SendingStartOfHistToSendingHistExt30(me, evt);
    CommMgr_SendingHistToC2Ext18(me, evt);
    res = CommMgr_isCondC2ToSendingEndOfHist23(me, evt);
    TEST_ASSERT_EQUAL(n, me->nFramesPerMsg);
    TEST_ASSERT_EQUAL(len * n, me->evSendObj.size);
    TEST_ASSERT_EQUAL(nFrames - me->nFramesPerMsg, me->framesToSend);
    TEST_ASSERT_TRUE(res == false);

    n = setupForSendingABlockOfFrames(nFrames - n, len, validity);
    CommMgr_C2ToSendingHistExt31(me, evt);
    CommMgr_SendingHistToC2Ext18(me, evt);
    res = CommMgr_isCondC2ToSendingEndOfHist23(me, evt);
    TEST_ASSERT_EQUAL(n, me->nFramesPerMsg);
    TEST_ASSERT_EQUAL(len * n, me->evSendObj.size);
    TEST_ASSERT_EQUAL(n - me->nFramesPerMsg, me->framesToSend);
    TEST_ASSERT_TRUE(res == true);

    validity = false;
    len = strlen(invalidFrameData);
    nFrames = MAX_NUM_FRAMES_PER_MSG;
    n = setupForSendingABlockOfFrames(nFrames, len, validity);
    CommMgr_SendingStartOfHistToSendingHistExt30(me, evt);
    CommMgr_SendingHistToC2Ext18(me, evt);
    res = CommMgr_isCondC2ToSendingEndOfHist23(me, evt);
    TEST_ASSERT_EQUAL(n, me->nFramesPerMsg);
    TEST_ASSERT_EQUAL(len * n, me->evSendObj.size);
    TEST_ASSERT_EQUAL(nFrames - me->nFramesPerMsg, me->framesToSend);
    TEST_ASSERT_TRUE(res == true);
}

void
test_SendABlockOfStatus(void)
{
    topic_publish_Expect(TCPConnection, 
                         RKH_UPCAST(RKH_EVT_T, &me->evSendObj), 
                         RKH_UPCAST(RKH_SMA_T, me));

    CommMgr_enSendingHist(me);
}

void
test_CheckEndOfBlock(void)
{
    rbool_t res;

    me->framesToSend = 0;
    res = CommMgr_isCondC2ToSendingEndOfHist23(me, evt);
    TEST_ASSERT_TRUE(res == true);

    me->framesToSend = 1;
    res = CommMgr_isCondC2ToSendingEndOfHist23(me, evt);
    TEST_ASSERT_TRUE(res == false);
}

void
test_EndHistoryMessage(void)
{
    ruint len = 16;

    YFrame_multipleTail_ExpectAndReturn(me->evSendObj.buf, len);
    topic_publish_Expect(TCPConnection, 
                         RKH_UPCAST(RKH_EVT_T, &me->evSendObj), 
                         RKH_UPCAST(RKH_SMA_T, me));

    CommMgr_enSendingEndOfHist(me);
    TEST_ASSERT_EQUAL(len, me->evSendObj.size);
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

void
test_PrepareForReceiving(void)
{
    me->nRecvTries = 0;
    CommMgr_SendingStatusToReceivingiStatusAckExt37(me, evt);
    TEST_ASSERT_EQUAL(16, me->nRecvTries);

    me->nRecvTries = 0;
    CommMgr_SendingEndOfHistToReceivingMsgAckExt34(me, evt);
    TEST_ASSERT_EQUAL(16, me->nRecvTries);
}

void
test_DecrementReceivingTries(void)
{
    int nTries = 2;

    me->nRecvTries = nTries;
    CommMgr_C0ToC6Ext35(me, evt);
    TEST_ASSERT_EQUAL(nTries - 1, me->nRecvTries);

    me->nRecvTries = nTries;
    CommMgr_C3ToC5Ext32(me, evt);
    TEST_ASSERT_EQUAL(nTries - 1, me->nRecvTries);
}

void
test_CheckReceivingTries(void)
{
    rbool_t res;
    int nTries = 2;

    me->nRecvTries = nTries;
    res = CommMgr_isCondC5ToHistoryFinal33(me, evt);
    TEST_ASSERT_TRUE(res == false);

    me->nRecvTries = 0;
    res = CommMgr_isCondC5ToHistoryFinal33(me, evt);
    TEST_ASSERT_TRUE(res == true);

    me->nRecvTries = nTries;
    res = CommMgr_isCondC6ToCurrentFinal36(me, evt);
    TEST_ASSERT_TRUE(res == false);

    me->nRecvTries = 0;
    res = CommMgr_isCondC6ToCurrentFinal36(me, evt);
    TEST_ASSERT_TRUE(res == true);
}

void
test_ReOpenConnection(void)
{
    RKH_EVT_T evRestartObj;

    topic_publish_Expect(TCPConnection, 
                         &evRestartObj, 
                         RKH_UPCAST(RKH_SMA_T, me));
    topic_publish_IgnoreArg_evt();

    CommMgr_C5ToHistoryFinalExt38(me, evt);

    topic_publish_Expect(TCPConnection, 
                         &evRestartObj, 
                         RKH_UPCAST(RKH_SMA_T, me));
    topic_publish_IgnoreArg_evt();

    CommMgr_C6ToCurrentFinalExt39(me, evt);
}

void
test_SendMsgFailInHistory(void)
{
    topic_publish_Expect(TCPConnection, 
                         RKH_UPCAST(RKH_EVT_T, evt), 
                         RKH_UPCAST(RKH_SMA_T, me));
    topic_publish_IgnoreArg_evt();

    CommMgr_HistoryToWaitSyncExt14(me, evt);
}

/* ------------------------------ End of file ------------------------------ */
