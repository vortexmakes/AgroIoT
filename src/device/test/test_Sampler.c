/**
 *  \file       test_sampler.c
 *  \brief      Unit test for Sampler concrete device..
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
#include "Sampler.h"
#include "SamplerSpy.h"
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
Mock_device_ctor_Callback(Device *const me, int id, RKH_SMA_T *collector,
                          JobCond *jobCond, DevVtbl *vtbl,
                          int cmock_num_calls)
{
    me->id = id;
    me->jobCond = SamplerSpy_getJobCondObj();
    me->vptr = SamplerSpy_getVtbl();
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

    device_ctor_Expect(SamplerSpy_getObj(),
                       SAMPLER,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);

    dev = Sampler_ctor();

    TEST_ASSERT_EQUAL(0, SamplerSpy_getHarness());
    TEST_ASSERT_EQUAL(0, SamplerSpy_getDeep());
    TEST_ASSERT_EQUAL(false, SamplerSpy_getIsValid());
    TEST_ASSERT_NOT_NULL(dev->jobCond);
    TEST_ASSERT_NOT_NULL(dev->vptr);
}

void
test_MakeEventOperation(void)
{
    Device *dev;
    RKH_EVT_T *evt;
    int harnessExpect = 2;
    int deepExpect = 4;
    bool isValidExpect = true;
    EvtSamplerData evtObj;
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(SamplerSpy_getObj(),
                       SAMPLER,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);
    me->status.data.devData.a.y = harnessExpect;
    me->status.data.devData.a.z = deepExpect;
    me->status.data.devData.a.m = isValidExpect;

    dev = Sampler_ctor();

    rkh_fwk_ae_ExpectAndReturn((RKH_ES_T)sizeof(EvtSamplerData),
                               (RKH_SIG_T)evDevData,
                               0,
                               (RKH_EVT_T *)&evtObj);
    rkh_fwk_ae_IgnoreArg_sender();

    TEST_ASSERT_NOT_NULL(dev->vptr->makeEvt);

    evt = (*dev->vptr->makeEvt)(dev, &me->status.data.devData);

    TEST_ASSERT_NOT_NULL(evt);
    TEST_ASSERT_EQUAL(dev, ((EvtSamplerData *)evt)->base.dev);
    TEST_ASSERT_EQUAL(harnessExpect, ((EvtSamplerData *)evt)->harness);
    TEST_ASSERT_EQUAL(deepExpect, ((EvtSamplerData *)evt)->deep);
    TEST_ASSERT_EQUAL(isValidExpect, ((EvtSamplerData *)evt)->isValid);
}

void
test_UpdateRawOperation(void)
{
    Device *dev;
    int harnessExpect = 2;
    int deepExpect = 4;
    bool isValidExpect = true;
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(SamplerSpy_getObj(),
                       SAMPLER,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);
    me->status.data.devData.a.y = 2;
    me->status.data.devData.a.z = 4;
    me->status.data.devData.a.m = true;

    dev = Sampler_ctor();

    me->dev = dev;
    TEST_ASSERT_NOT_NULL(dev->vptr->updateRaw);
    TEST_ASSERT_NOT_NULL(dev->collector);
    ((Sampler *)dev)->harness = harnessExpect;
    ((Sampler *)dev)->deep = deepExpect;
    ((Sampler *)dev)->isValid = isValidExpect;

    (*dev->vptr->updateRaw)(dev);

    TEST_ASSERT_EQUAL(harnessExpect, me->status.data.devData.a.y);
    TEST_ASSERT_EQUAL(deepExpect, me->status.data.devData.a.z);
    TEST_ASSERT_EQUAL(isValidExpect, me->status.data.devData.a.m);
    TEST_ASSERT_EQUAL(SAMPLER, me->status.data.devData.a.x);
}

void
test_UpdateOperation(void)
{
    Device *dev;
    int harnessExpect = 2;
    int deepExpect = 4;
    bool isValidExpect = true;
    EvtSamplerData evtSamplerData;
    RKH_EVT_T *evt;
    Sampler *sampler;
    Collector *me;
    bool result;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(SamplerSpy_getObj(),
                       SAMPLER,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);

    dev = Sampler_ctor();

    evtSamplerData.base.dev = dev;
    evtSamplerData.harness = harnessExpect;
    evtSamplerData.deep = deepExpect;
    evtSamplerData.isValid = isValidExpect;
    evt = (RKH_EVT_T *)&evtSamplerData;

    result = (*dev->vptr->update)(dev, evt);

    sampler = (Sampler *)me->dev;
    TEST_ASSERT_EQUAL(dev, me->dev);
    TEST_ASSERT_EQUAL(harnessExpect, sampler->harness);
    TEST_ASSERT_EQUAL(deepExpect, sampler->deep);
    TEST_ASSERT_EQUAL(isValidExpect, sampler->isValid);
    TEST_ASSERT_EQUAL(true, result);

    /* Invalid received data */
    isValidExpect = false;
    evtSamplerData.isValid = isValidExpect;

    result = (*dev->vptr->update)(dev, evt);

    TEST_ASSERT_EQUAL(dev, me->dev);
    TEST_ASSERT_EQUAL(false, result);
}

void
test_TestOperation(void)
{
    Device *dev;
    int result = 0;
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(SamplerSpy_getObj(),
                       SAMPLER,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);

    dev = Sampler_ctor();

    me->dev = dev;
    TEST_ASSERT_NOT_NULL(dev->vptr->test);
    TEST_ASSERT_NOT_NULL(dev->collector);

    result = (*dev->vptr->test)(dev);
    TEST_ASSERT_EQUAL(false, result);
}

/* ------------------------------ End of file ------------------------------ */
