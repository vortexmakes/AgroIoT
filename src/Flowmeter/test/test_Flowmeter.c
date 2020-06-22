/**
 *  \file       test_Flowmeter.c
 *  \brief      Unit test for flowmeter device.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.17.06  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "rkhfwk_cast.h"
#include "Flowmeter.h"
#include "events.h"
#include "Mock_signals.h"
#include "Mock_rkhassert.h"
#include "Mock_rkhfwk_dynevt.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static Flowmeter me;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
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
test_InitializeAttributes(void)
{
    Flowmeter_init(&me);

    TEST_ASSERT_EQUAL(0, me.flow1.nPulses);
    TEST_ASSERT_EQUAL(Forward, me.flow1.dir);
    TEST_ASSERT_EQUAL(0, me.flow2.nPulses);
    TEST_ASSERT_EQUAL(Forward, me.flow2.dir);
}

void
test_Update(void)
{
    FlowmeterEvt event;

    event.flow1.nPulses = 32;
    event.flow1.dir = Reverse;
    event.flow2.nPulses = 16;
    event.flow2.dir = Forward;

    Flowmeter_update(&me, RKH_UPCAST(RKH_EVT_T, &event));

    TEST_ASSERT_EQUAL(event.flow1.nPulses, me.flow1.nPulses);
    TEST_ASSERT_EQUAL(event.flow1.dir, me.flow1.dir);
    TEST_ASSERT_EQUAL(event.flow2.nPulses, me.flow2.nPulses);
    TEST_ASSERT_EQUAL(event.flow2.dir, me.flow2.dir);
}

void
test_UpdateRawStatus(void)
{
    CBOX_STR status;

    me.flow1.nPulses = 0xad;
    me.flow2.nPulses = 0xde;
    me.flow1.dir = Forward;
    me.flow2.dir = Reverse;

    Flowmeter_updateRaw(&me, &status);
    TEST_ASSERT_EQUAL_HEX16(0xdead, status.a.y);
    TEST_ASSERT_EQUAL_HEX16(0x02, status.a.z);
}

void
test_MakeAnFlowmeterEvent(void)
{
    FlowData flow1, flow2;
    FlowmeterEvt eventObj, *realEvt;
    RKH_EVT_T *event;

    flow1.nPulses = 0xad;
    flow2.nPulses = 0xde;
    flow1.dir = Forward;
    flow2.dir = Reverse;
    rkh_fwk_ae_ExpectAndReturn(sizeof(FlowmeterEvt), 
                               evFlowmeter, 
                               0, 
                               RKH_UPCAST(RKH_EVT_T, &eventObj));

    event = Flowmeter_makeEvt(&flow1, &flow2);

    realEvt = RKH_DOWNCAST(FlowmeterEvt, event);
    TEST_ASSERT_EQUAL_HEX16(flow1.nPulses, realEvt->flow1.nPulses);
    TEST_ASSERT_EQUAL_HEX16(flow2.nPulses, realEvt->flow2.nPulses);
    TEST_ASSERT_EQUAL(flow1.dir, realEvt->flow1.dir);
    TEST_ASSERT_EQUAL(flow2.dir, realEvt->flow2.dir);
}

/* ------------------------------ End of file ------------------------------ */
