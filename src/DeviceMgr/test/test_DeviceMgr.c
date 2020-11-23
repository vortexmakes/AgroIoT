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
initDeviceMgr(bool enableBackoff)
{
    rkh_trc_isoff__IgnoreAndReturn(false);
    rkh_trc_ao_Ignore();
    rkh_trc_obj_Ignore();
    rkh_trc_state_Ignore();

    rkh_tmr_init__Ignore();
    ps_init_Ignore();

    rkh_sm_init(RKH_UPCAST(RKH_SM_T, deviceMgr));
    if (enableBackoff == true)
    {
        deviceMgr->enableBackoff = true;    /* force to true to test it */
    }
}

static void
activeDeviceMgr(void)
{
    RKH_SET_STATIC_EVENT(&event, evOpen);
    Config_getDevPollCycleTime_ExpectAndReturn(DEV_POLL_CYCLE_TIME_DFT);
    ps_start_Expect();

    if (deviceMgr->enableBackoff == true)
    {
        ++deviceMgr->tries; /* set on ps_onStop() */
                            /* if enableBackoff is true */
    }
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
test_InitPollCycle(void)
{
    initDeviceMgr(DisableBackoff);
    activeDeviceMgr();

    TEST_ASSERT_EQUAL(0, deviceMgr->tries);
    TEST_ASSERT_EQUAL(0, deviceMgr->backoff);
    TEST_ASSERT_EQUAL(false, deviceMgr->enableBackoff);
    TEST_ASSERT_EQUAL(DEV_POLL_CYCLE_TIME_DFT, deviceMgr->pollCycle);
}

void
test_SetPollCycleWithDisabledBackoff(void)
{
    initDeviceMgr(DisableBackoff);
    activeDeviceMgr();

    RKH_SET_STATIC_EVENT(&event, evEndOfCycle);
    Config_getDevPollCycleTime_ExpectAndReturn(DEV_POLL_CYCLE_TIME_DFT);
    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_tmr_start_Expect(&deviceMgr->tmr.tmr, 
                         RKH_UPCAST(RKH_SMA_T, deviceMgr), 
                         RKH_TIME_SEC(DEV_POLL_CYCLE_TIME_DFT), 
                         0);

    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, deviceMgr), &event);

    TEST_ASSERT_EQUAL(0, deviceMgr->tries);
    TEST_ASSERT_EQUAL(0, deviceMgr->backoff);
    TEST_ASSERT_EQUAL(DEV_POLL_CYCLE_TIME_DFT, deviceMgr->pollCycle);
}

void
test_SetPollCycleWoutDevicesFirstTime(void)
{
    initDeviceMgr(EnableBackoff);
    activeDeviceMgr();

    if (deviceMgr->enableBackoff == true)
    {
        ++deviceMgr->tries; /* set on ps_onStop() */
                            /* if enableBackoff is true */
    }
    RKH_SET_STATIC_EVENT(&event, evEndOfCycle);
    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_tmr_start_Expect(&deviceMgr->tmr.tmr, 
                         RKH_UPCAST(RKH_SMA_T, deviceMgr), 
                         RKH_TIME_SEC(DEV_POLL_CYCLE_TIME_DFT), 
                         0);

    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, deviceMgr), &event);

    TEST_ASSERT_EQUAL(1, deviceMgr->tries);
    TEST_ASSERT_EQUAL(0, deviceMgr->backoff);
    TEST_ASSERT_EQUAL(DEV_POLL_CYCLE_TIME_DFT, deviceMgr->pollCycle);
}

void
test_SetPollCycleWithConnectedDevices(void)
{
    initDeviceMgr(EnableBackoff);
    activeDeviceMgr();

    deviceMgr->tries = 0; /* set by ps_onStationRecv() callback */
    RKH_SET_STATIC_EVENT(&event, evEndOfCycle);
    Config_getDevPollCycleTime_ExpectAndReturn(DEV_POLL_CYCLE_TIME_DFT);
    rkh_tmr_start_Expect(&deviceMgr->tmr.tmr, 
                         RKH_UPCAST(RKH_SMA_T, deviceMgr), 
                         RKH_TIME_SEC(DEV_POLL_CYCLE_TIME_DFT), 
                         0);

    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, deviceMgr), &event);

    TEST_ASSERT_EQUAL(0, deviceMgr->tries);
    TEST_ASSERT_EQUAL(0, deviceMgr->backoff);
    TEST_ASSERT_EQUAL(DEV_POLL_CYCLE_TIME_DFT, deviceMgr->pollCycle);
}

void
test_SetPollCycleWoutDevicesReachingMaxNumTriesOnce(void)
{
    initDeviceMgr(EnableBackoff);
    activeDeviceMgr();

    if (deviceMgr->enableBackoff == true)
    {
        deviceMgr->tries = DEV_MAX_NUM_TRIES; /* set by ps_onStop() callback */
                                              /* if enableBackoff is true */
    }
    RKH_SET_STATIC_EVENT(&event, evEndOfCycle);
    rkh_tmr_start_Expect(&deviceMgr->tmr.tmr, 
                         RKH_UPCAST(RKH_SMA_T, deviceMgr), 
                         RKH_TIME_SEC(DEV_POLL_CYCLE_TIME_DFT * 2), 
                         0);

    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, deviceMgr), &event);

    TEST_ASSERT_EQUAL(0, deviceMgr->tries);
    TEST_ASSERT_EQUAL(1, deviceMgr->backoff);
    TEST_ASSERT_EQUAL(DEV_POLL_CYCLE_TIME_DFT * 2, deviceMgr->pollCycle);
}

void
test_SetPollCycleWoutDevicesReachingMaxNumTriesMaxNumBackoffTimes(void)
{
    uint32_t pollCycleTime;

    initDeviceMgr(EnableBackoff);
    activeDeviceMgr();

    if (deviceMgr->enableBackoff == true)
    {
        deviceMgr->tries = DEV_MAX_NUM_TRIES; /* set by ps_onStop() callback */
                                              /* if enableBackoff is true */
    }
    deviceMgr->backoff = DEV_MAX_NUM_BACKOFF;
    pollCycleTime = (DEV_POLL_CYCLE_TIME_DFT << DEV_MAX_NUM_BACKOFF);
    deviceMgr->pollCycle = pollCycleTime;
    RKH_SET_STATIC_EVENT(&event, evEndOfCycle);
    rkh_tmr_start_Expect(&deviceMgr->tmr.tmr, 
                         RKH_UPCAST(RKH_SMA_T, deviceMgr), 
                         RKH_TIME_SEC(pollCycleTime), 
                         0);

    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, deviceMgr), &event);

    TEST_ASSERT_EQUAL(0, deviceMgr->tries);
    TEST_ASSERT_EQUAL(DEV_MAX_NUM_BACKOFF, deviceMgr->backoff);
    TEST_ASSERT_EQUAL(pollCycleTime, deviceMgr->pollCycle);
}

/* ------------------------------ End of file ------------------------------ */
