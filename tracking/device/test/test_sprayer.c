/**
 *  \file       test_sprayer.c
 *  \brief      Unit test for sprayer concrete device..
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.17.10  LeFr  v1.0.00  ---
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "sprayer.h"
#include "sprayerSpy.h"
#include "Mock_rkhassert.h"
#include "Mock_device.h"
#include "Mock_collector.h"
#include "Mock_rkhfwk_dynevt.h"
#include "Mock_signals.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static Collector collectorActObj;
Collector *const collector = &collectorActObj;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void 
MockAssertCallback(const char* const file, int line, int cmock_num_calls)
{
    TEST_PASS();
}

static void 
Mock_device_ctor_Callback(Device *const me, int id, RKH_SMA_T *collector, 
                          JobCond *jobCond, DevVtbl *vtbl, 
                          int cmock_num_calls)
{
    me->jobCond = sprayerSpy_getJobCondObj();
    me->vptr = sprayerSpy_getVtbl();
}

/* ---------------------------- Global functions --------------------------- */
void 
setUp(void)
{
    Mock_rkhassert_Init();
    Mock_device_Init();
}

void 
tearDown(void)
{
    Mock_rkhassert_Verify();
    Mock_device_Verify();
    Mock_rkhassert_Destroy();
    Mock_device_Destroy();
}

void
test_InitAttributes(void)
{
    Device *dev;
    int nSectionMaxExpect = 5;

    device_ctor_Expect(sprayerSpy_getObj(), 
                       SPRAYER, 
                       (RKH_SMA_T *)collector, 
                       (JobCond *)0, 
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);

    dev = sprayer_ctor(nSectionMaxExpect);

    TEST_ASSERT_EQUAL(0, sprayerSpy_getNSection());
    TEST_ASSERT_EQUAL(0, sprayerSpy_getDose());
    TEST_ASSERT_EQUAL(nSectionMaxExpect, sprayerSpy_getNSectionMax());
    TEST_ASSERT_NOT_NULL(dev->jobCond);
    TEST_ASSERT_NOT_NULL(dev->vptr);
}

void
test_MakeEventOperation(void)
{
    Device *dev;
    RKH_EVT_T *evt;
    int nSectionExpect = 2;
    int doseExpect = 4;
    EvtSprayerData evtObj;

    device_ctor_Expect(sprayerSpy_getObj(), 
                       SPRAYER, 
                       (RKH_SMA_T *)collector, 
                       (JobCond *)0, 
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);
    collector->rawData.hum = nSectionExpect;
    collector->rawData.h.pqty = doseExpect;

    dev = sprayer_ctor(0);

    rkh_fwk_ae_ExpectAndReturn((RKH_ES_T)sizeof(EvtSprayerData), 
                               (RKH_SIG_T)evDevData, 
                               0,
                               (RKH_EVT_T *)&evtObj);
    rkh_fwk_ae_IgnoreArg_sender();

    TEST_ASSERT_NOT_NULL(dev->vptr->makeEvt);
    evt = (*dev->vptr->makeEvt)(dev, &collector->rawData);
    TEST_ASSERT_NOT_NULL(evt);
    TEST_ASSERT_EQUAL(dev, ((EvtSprayerData *)evt)->base.dev);
    TEST_ASSERT_EQUAL(nSectionExpect, ((EvtSprayerData *)evt)->param.nSection);
    TEST_ASSERT_EQUAL(doseExpect, ((EvtSprayerData *)evt)->param.dose);
}

/* ------------------------------ End of file ------------------------------ */
