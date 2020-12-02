/**
 *  \file       test_DeviceMgr.c
 *  \brief      Unit test for state machine module.
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "DeviceMgr.h"
#include "Mock_rkhsma.h"
#include "rkhsm.h"
#include "rkhfwk_cast.h"
#include "signals.h"
#include "events.h"
#include "Mock_topic.h"
#include "Mock_rkhtrc_record.h"
#include "Mock_rkhtrc_filter.h"
#include "Mock_rkhtrc_out.h"
#include "Mock_rkhtmr.h"
#include "Mock_rkhfwk_dynevt.h"
#include "Mock_device.h"
#include "Mock_rkhassert.h"
#include "Mock_Config.h"
#include "Mock_StatQue.h"
#include "Mock_GStatus.h"
#include "Mock_geoMgr.h"
#include "Mock_Trace.h"
#include "Mock_Backup.h"
#include "Mock_Flowmeter.h"
#include "AgroIoTVersion.h"
#include "Mock_bsp.h"
#include "Mock_ps.h"
#include "Mock_rkhport.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define EnableBackoff       true
#define DisableBackoff      false

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
const RKHSmaVtbl rkhSmaVtbl =  /* Instantiate it because rkhsma is mocked */
{
    rkh_sma_activate, rkh_sma_dispatch, rkh_sma_post_fifo, rkh_sma_post_lifo
};
Device *sprayer, *sampler, *harvest;
static RKH_STATIC_EVENT(event, 0);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
init(void)
{
    rkh_trc_isoff__IgnoreAndReturn(false);
    rkh_trc_ao_Ignore();
    rkh_trc_obj_Ignore();
    rkh_trc_state_Ignore();

    Config_getDevPollCycleTime_ExpectAndReturn(DEV_POLL_CYCLE_TIME_DFT);
    ps_init_Ignore();

    rkh_sm_init(RKH_UPCAST(RKH_SM_T, deviceMgr));
    TEST_ASSERT_EQUAL(DEV_POLL_CYCLE_TIME_DFT, deviceMgr->pollPer);
}

static void
active(void)
{
    RKH_SET_STATIC_EVENT(&event, evOpen);
    ps_start_Expect();
    rkh_tmr_init__Ignore();
    rkh_tmr_start_Expect(&deviceMgr->tmr.tmr, 
                         RKH_UPCAST(RKH_SMA_T, deviceMgr), 
                         RKH_TIME_MS(deviceMgr->pollPer), 
                         0);

    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, deviceMgr), &event);
}

/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
}

void
tearDown(void)
{
}

void
test_Init(void)
{
    rkh_trc_isoff__IgnoreAndReturn(false);
    rkh_trc_ao_Ignore();
    rkh_trc_obj_Ignore();
    rkh_trc_state_Ignore();

    Config_getDevPollCycleTime_ExpectAndReturn(DEV_POLL_CYCLE_TIME_DFT);
    ps_init_Ignore();

    rkh_sm_init(RKH_UPCAST(RKH_SM_T, deviceMgr));
    TEST_ASSERT_EQUAL(DEV_POLL_CYCLE_TIME_DFT, deviceMgr->pollPer);
}

void
test_ActivateAndStartPollingCycle(void)
{
    init();

    RKH_SET_STATIC_EVENT(&event, evOpen);
    ps_start_Expect();
    rkh_tmr_init__Ignore();
    rkh_tmr_start_Expect(&deviceMgr->tmr.tmr, 
                         RKH_UPCAST(RKH_SMA_T, deviceMgr), 
                         RKH_TIME_MS(deviceMgr->pollPer), 
                         0);

    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, deviceMgr), &event);
}

void
test_PollingFinishedWaitingForEndOfCycle(void)
{
    init();
    active();

    RKH_SET_STATIC_EVENT(&event, evEndOfPolling);
    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, deviceMgr), &event);
}

void
test_RestartThePollingCycle(void)
{
    init();
    active();

    RKH_SET_STATIC_EVENT(&event, evEndOfPolling);
    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, deviceMgr), &event);

    /* Exit from Active state */
    RKH_SET_STATIC_EVENT(&event, evTimeout);
    rkh_tmr_stop_ExpectAndReturn(&deviceMgr->tmr.tmr, false);
    /* Exit from Starting state through its null transition */
    ps_start_station_Expect(ADDR_NORIA);
    ps_restart_Expect();
    /* Finally, reenter to Active state */
    rkh_tmr_init__Ignore();
    rkh_tmr_start_Expect(&deviceMgr->tmr.tmr, 
                         RKH_UPCAST(RKH_SMA_T, deviceMgr), 
                         RKH_TIME_MS(deviceMgr->pollPer), 
                         0);

    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, deviceMgr), &event);
}

void
test_DisablePollingCycle(void)
{
    init();

    RKH_SET_STATIC_EVENT(&event, evOpen);
    ps_start_Expect();
    rkh_tmr_init__Ignore();
    rkh_tmr_start_Expect(&deviceMgr->tmr.tmr, 
                         RKH_UPCAST(RKH_SMA_T, deviceMgr), 
                         RKH_TIME_MS(deviceMgr->pollPer), 
                         0);

    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, deviceMgr), &event);

    RKH_SET_STATIC_EVENT(&event, evClose);
    rkh_tmr_stop_ExpectAndReturn(&deviceMgr->tmr.tmr, false);
    ps_stop_Expect();
    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, deviceMgr), &event);
}

/* ------------------------------ End of file ------------------------------ */
