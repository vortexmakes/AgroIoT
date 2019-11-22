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

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
    me = RKH_DOWNCAST(CommMgr, commMgr);
    evt = &evtObj;
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

    CommMgr_ToIdleExt0(me, evt);
    TEST_ASSERT_EQUAL(0, me->isPendingStatus);
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
test_OnSyncSendStatus(void)
{
    TEST_IGNORE_MESSAGE(__FUNCTION__);
}

void
test_CheckPendingStatus(void)
{
    TEST_IGNORE_MESSAGE(__FUNCTION__);
}

void
test_ReceiveAck(void)
{
    TEST_IGNORE_MESSAGE(__FUNCTION__);
}

void
test_ReceiveUnknownResponse(void)
{
    TEST_IGNORE_MESSAGE(__FUNCTION__);
}

void
test_CheckHistory(void)
{
    TEST_IGNORE_MESSAGE(__FUNCTION__);
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
