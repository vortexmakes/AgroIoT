/**
 *  \file       test_CommMgrAct.c
 *  \brief      Unit test for state machine module.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "CommMgrAct.h"
#include "CommMgr.h"
#include "signals.h"
#include "rkhfwk_cast.h"
#include "Mock_rkhtmr.h"
#include "Mock_rkhtrc_record.h"
#include "Mock_rkhassert.h"
#include "Mock_rkhsma.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
RKH_SMA_CREATE(CommMgr, commMgr, 0, HCAL, NULL, NULL, NULL);
RKH_SMA_DEF_PTR(commMgr);
static CommMgr *me;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
    me = RKH_DOWNCAST(CommMgr, commMgr);
//    Mock_DigIn_Init();
}

void
tearDown(void)
{
//    Mock_DigIn_Verify();
//    Mock_DigIn_Destroy();
}

void
test_Initialize(void)
{
    RKH_EVT_T *evt;

    rkh_trc_ao_Ignore();
    rkh_trc_obj_Ignore();
    rkh_trc_state_Ignore();
    rkh_trc_sig_Ignore();

    ConnectionTopic_subscribe_Expect(me);
    DeviceStatus_subscribe_Expect(me);
    TEST_ASSERT_EQUAL(0, me->status.pending);

    TEST_ASSERT_EQUAL(0, me->digIn.clk);
    TEST_ASSERT_EQUAL(0, me->digIn.clkX3);
    TEST_ASSERT_EQUAL(0, me->digIn.clkX6);
    TEST_ASSERT_EQUAL(0, me->digIn.failure);
}

#if 0
void
test_StartSynchro(void)
{
    rkh_tmr_init__Expect(&me->evSyncObj.tmr, (RKH_EVT_T *)&me->evSyncObj);
    rkh_tmr_start_Expect(&me->evSyncObj.tmr, RKH_UPCAST(RKH_SMA_T, me), 
                         SIGMON_SYNC_TIME, SIGMON_SYNC_TIME);
    SigMon_enSMActive(me);
}

void
test_StopSynchro(void)
{
    rkh_tmr_stop_ExpectAndReturn(&me->evSyncObj.tmr, true);
    SigMon_exSMActive(me);
}

void
test_Synchro(void)
{
    DigIn status;
    rInt inSeqExpect;

    inSeqExpect = 0;
    me->digIn.clk = status.clk = 1;
    me->digIn.clkX3 = status.clkX3 = 0;
    me->digIn.clkX6 = status.clkX6 = 1;
    me->digIn.failure = status.failure = 0;
    inSeqExpect = ((status.clk << 2) | (status.clkX3 << 1) | status.clkX6);
    DigIn_get_ExpectAndReturn(status);
    rkh_sma_post_lifo_Expect(RKH_UPCAST(RKH_SMA_T, me), &me->evInObj, me);

    SigMon_SMActiveToSMActiveLoc2(me, RKH_UPCAST(RKH_EVT_T, &me->evSyncObj));

    TEST_ASSERT_EQUAL(status.clk, me->digIn.clk);
    TEST_ASSERT_EQUAL(status.clkX3, me->digIn.clkX3);
    TEST_ASSERT_EQUAL(status.clkX6, me->digIn.clkX6);
    TEST_ASSERT_EQUAL(status.failure, me->digIn.failure);
    TEST_ASSERT_EQUAL(mapDigIn[inSeqExpect], me->evInObj.e);
}

void
test_StoreDigInput(void)
{
    storeDigInPrepareAndExecute(1, 0);
    storeDigInPrepareAndExecute(0, 0);
    storeDigInPrepareAndExecute(6, 1);
    storeDigInPrepareAndExecute(2, 0);
    storeDigInPrepareAndExecute(4, 1);
    storeDigInPrepareAndExecute(0, 0);
    storeDigInPrepareAndExecute(4, 1);
    storeDigInPrepareAndExecute(0, 0);
    storeDigInPrepareAndExecute(4, 1);
    storeDigInPrepareAndExecute(0, 0);
    storeDigInPrepareAndExecute(5, 1);
    storeDigInPrepareAndExecute(1, 0);
    storeDigInPrepareAndExecute(0, 0);
    storeDigInPrepareAndExecute(6, 1);
}

void
test_StartAdqCycle(void)
{
    Relay_getCurrent_ExpectAndReturn(0xdead);
    Relay_getVoltage_ExpectAndReturn(0xbeaf);

    SigMon_enSeq1(me);
    TEST_ASSERT_EQUAL(0xdead, me->currVal);
    TEST_ASSERT_EQUAL(0xbeaf, me->voltVal);
}

void
test_AdquireSeq1(void)
{
    Relay_getCurrent_ExpectAndReturn(1);
    Relay_getVoltage_ExpectAndReturn(1);
    Relay_getCurrent_ExpectAndReturn(2);
    Relay_getVoltage_ExpectAndReturn(2);
    Relay_getCurrent_ExpectAndReturn(3);
    Relay_getVoltage_ExpectAndReturn(3);

    SigMon_enSeq1(me);
    SigMon_Seq1ToSeq1Loc6(me, RKH_UPCAST(RKH_EVT_T, &me->evSyncObj));
    SigMon_Seq1ToSeq2Ext7(me, RKH_UPCAST(RKH_EVT_T, &me->evSyncObj));

    TEST_ASSERT_EQUAL(3, me->currVal);
    TEST_ASSERT_EQUAL(3, me->voltVal);
}

void
test_AdquireSeq2(void)
{
    Relay_getCurrent_ExpectAndReturn(1);
    Relay_getVoltage_ExpectAndReturn(1);
    Relay_getCurrent_ExpectAndReturn(2);
    Relay_getVoltage_ExpectAndReturn(2);

    SigMon_Seq2ToSeq2Loc5(me, RKH_UPCAST(RKH_EVT_T, &me->evSyncObj));
    SigMon_Seq2ToSeq2Loc5(me, RKH_UPCAST(RKH_EVT_T, &me->evSyncObj));

    TEST_ASSERT_EQUAL(2, me->currVal);
    TEST_ASSERT_EQUAL(2, me->voltVal);
}

void
test_StopAdqCycle(void)
{
    StoreTest_saveRelayStatus_Expect(me->currVal, me->voltVal);
    SigMon_Seq2ToSeq3Ext8(me, RKH_UPCAST(RKH_EVT_T, &me->evSyncObj));
}

void
test_Failure(void)
{
    DigIn status;

    StoreTest_saveDigInStatus_Expect(status);
    StoreTest_saveDigInStatus_IgnoreArg_digIn();

    SigMon_SMActiveToSigMon_FinalExt3(me, 
                                      RKH_UPCAST(RKH_EVT_T, &me->evSyncObj));
}

#endif
/* ------------------------------ End of file ------------------------------ */
