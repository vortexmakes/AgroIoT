/**
 *  \file       test_Skeleton.c
 *  \brief      Unit test for Skeleton concrete device..
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "Skeleton.h"
#include "SkeletonSpy.h"
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
    me->jobCond = SkeletonSpy_getJobCondObj();
    me->vptr = SkeletonSpy_getVtbl();
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

    device_ctor_Expect(SkeletonSpy_getObj(),
                       SKELETON,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);

    dev = Skeleton_ctor();

    TEST_ASSERT_EQUAL(0, SkeletonSpy_getX());
    TEST_ASSERT_NOT_NULL(dev->jobCond);
    TEST_ASSERT_NOT_NULL(dev->vptr);
}

void
test_MakeEventOperation(void)
{
    Device *dev;
    RKH_EVT_T *evt;
    int xExpect = 2;
    EvtSkeletonData evtObj;
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(SkeletonSpy_getObj(),
                       SAMPLER,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);
    me->status.data.devData.a.y = xExpect;

    dev = Skeleton_ctor();

    rkh_fwk_ae_ExpectAndReturn((RKH_ES_T)sizeof(EvtSkeletonData),
                               (RKH_SIG_T)evDevData,
                               0,
                               (RKH_EVT_T *)&evtObj);
    rkh_fwk_ae_IgnoreArg_sender();

    TEST_ASSERT_NOT_NULL(dev->vptr->makeEvt);

    evt = (*dev->vptr->makeEvt)(dev, &me->status.data.devData);

    TEST_ASSERT_NOT_NULL(evt);
    TEST_ASSERT_EQUAL(dev, ((EvtSkeletonData *)evt)->base.dev);
    TEST_ASSERT_EQUAL(xExpect, ((EvtSkeletonData *)evt)->x);
}

void
test_UpdateRawOperation(void)
{
    Device *dev;
    int xExpect = 2;
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(SkeletonSpy_getObj(),
                       SAMPLER,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);
    me->status.data.devData.a.y = 2;

    dev = Skeleton_ctor();

    me->dev = dev;
    TEST_ASSERT_NOT_NULL(dev->vptr->updateRaw);
    TEST_ASSERT_NOT_NULL(dev->collector);
    ((Skeleton *)dev)->x = xExpect;

    (*dev->vptr->updateRaw)(dev);

    TEST_ASSERT_EQUAL(xExpect, me->status.data.devData.a.y);
    TEST_ASSERT_EQUAL(SKELETON, me->status.data.devData.a.x);
}

void
test_UpdateOperation(void)
{
    Device *dev;
    int xExpect = 2;
    EvtSkeletonData evtSkeletonData;
    RKH_EVT_T *evt;
    Skeleton *Skeleton;
    Collector *me;
    bool result;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(SkeletonSpy_getObj(),
                       SAMPLER,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);

    dev = Skeleton_ctor();

    evtSkeletonData.base.dev = dev;
    evtSkeletonData.x = xExpect;
    evt = (RKH_EVT_T *)&evtSkeletonData;

    result = (*dev->vptr->update)(dev, evt);

    Skeleton = (Skeleton *)me->dev;
    TEST_ASSERT_EQUAL(dev, me->dev);
    TEST_ASSERT_EQUAL(xExpect, Skeleton->x);
    TEST_ASSERT_EQUAL(false, result);
}

void
test_TestOperation(void)
{
    Device *dev;
    int result = 0;
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(SkeletonSpy_getObj(),
                       SAMPLER,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);

    dev = Skeleton_ctor();

    me->dev = dev;
    TEST_ASSERT_NOT_NULL(dev->vptr->test);
    TEST_ASSERT_NOT_NULL(dev->collector);

    result = (*dev->vptr->test)(dev);
    TEST_ASSERT_EQUAL(false, result);
}

/* ------------------------------ End of file ------------------------------ */
