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
#include "Mock_Collector.h"
#include "Mock_rkhfwk_dynevt.h"
#include "Mock_signals.h"
#include "Mock_rkhfwk_cast.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
RKH_SMA_CREATE(Collector, collector, 0, HCAL, NULL, NULL, NULL);
RKH_SMA_DEF_PTR(collector);

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
    me->collector = collector;
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
    TEST_ASSERT_EQUAL(nSectionMaxExpect, sprayerSpy_getDoseMax());
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
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(sprayerSpy_getObj(),
                       SPRAYER,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);
    me->status.devData.hum = nSectionExpect;
    me->status.devData.h.pqty = doseExpect;

    dev = sprayer_ctor(0);

    rkh_fwk_ae_ExpectAndReturn((RKH_ES_T)sizeof(EvtSprayerData),
                               (RKH_SIG_T)evDevData,
                               0,
                               (RKH_EVT_T *)&evtObj);
    rkh_fwk_ae_IgnoreArg_sender();

    TEST_ASSERT_NOT_NULL(dev->vptr->makeEvt);

    evt = (*dev->vptr->makeEvt)(dev, &me->status.devData);

    TEST_ASSERT_NOT_NULL(evt);
    TEST_ASSERT_EQUAL(dev, ((EvtSprayerData *)evt)->base.dev);
    TEST_ASSERT_EQUAL(nSectionExpect, ((EvtSprayerData *)evt)->nSection);
    TEST_ASSERT_EQUAL(doseExpect, ((EvtSprayerData *)evt)->dose);
}

void
test_UpdateRawOperation(void)
{
    Device *dev;
    int nSectionExpect = 2;
    int doseExpect = 4;
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(sprayerSpy_getObj(),
                       SPRAYER,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);
    me->status.devData.hum = 0;
    me->status.devData.h.pqty = 0;

    dev = sprayer_ctor(0);

    me->dev = dev;
    TEST_ASSERT_NOT_NULL(dev->vptr->updateRaw);
    TEST_ASSERT_NOT_NULL(dev->collector);
    ((Sprayer *)dev)->nSection = nSectionExpect;
    ((Sprayer *)dev)->dose = doseExpect;

    (*dev->vptr->updateRaw)(dev);

    TEST_ASSERT_EQUAL(nSectionExpect, me->status.devData.hum);
    TEST_ASSERT_EQUAL(doseExpect, me->status.devData.h.pqty);
}

void
test_UpdateOperation(void)
{
    Device *dev;
    int nSectionExpect = 2;
    int doseExpect = 4;
    EvtSprayerData evtSprayerData;
    RKH_EVT_T *evt;
    Sprayer *sprayer;
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(sprayerSpy_getObj(),
                       SPRAYER,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);

    dev = sprayer_ctor(0);

    evtSprayerData.base.dev = dev;
    evtSprayerData.nSection = nSectionExpect;
    evtSprayerData.dose = doseExpect;
    evt = (RKH_EVT_T *)&evtSprayerData;

    (*dev->vptr->update)(dev, evt);

    sprayer = (Sprayer *)me->dev;
    TEST_ASSERT_EQUAL(dev, me->dev);
    TEST_ASSERT_EQUAL(nSectionExpect, sprayer->nSection);
    TEST_ASSERT_EQUAL(doseExpect, sprayer->dose);
}

void
test_TestOperation(void)
{
    Device *dev;
    int nSectionExpect = 2;
    int doseMax = 5;
    Sprayer *sprayer;
    int result = 0;
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(sprayerSpy_getObj(),
                       SPRAYER,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);

    dev = sprayer_ctor(doseMax);

    me->dev = dev;
    TEST_ASSERT_NOT_NULL(dev->vptr->test);
    TEST_ASSERT_NOT_NULL(dev->collector);
    sprayer = (Sprayer *)dev;
    sprayer->nSection = nSectionExpect;
    sprayer->dose = doseMax - 1;

    result = (*dev->vptr->test)(dev);
    TEST_ASSERT_EQUAL(0, result);

    sprayer->dose = doseMax + 1;

    result = (*dev->vptr->test)(dev);
    TEST_ASSERT_EQUAL(1, result);
}

/* ------------------------------ End of file ------------------------------ */
