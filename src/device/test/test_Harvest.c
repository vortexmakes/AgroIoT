/**
 *  \file       test_Harvest.c
 *  \brief      Unit test for Harvest concrete device..
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "Harvest.h"
#include "HarvestSpy.h"
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
    me->jobCond = HarvestSpy_getJobCondObj();
    me->vptr = HarvestSpy_getVtbl();
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

    device_ctor_Expect(HarvestSpy_getObj(),
                       HARVEST,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);

    dev = Harvest_ctor(0);

    TEST_ASSERT_EQUAL(0, HarvestSpy_getHoard());
    TEST_ASSERT_EQUAL(0, HarvestSpy_getNPail());
    TEST_ASSERT_EQUAL(0, HarvestSpy_getFlow());
    TEST_ASSERT_NOT_NULL(dev->jobCond);
    TEST_ASSERT_NOT_NULL(dev->vptr);
}

void
test_MakeEventOperation(void)
{
    Device *dev;
    RKH_EVT_T *evt;
    uint16_t hoardExpect = 2;
    uint16_t nPailExpect = 4;
    uint16_t flowExpect = 6;
    EvtHarvestData evtObj;
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(HarvestSpy_getObj(),
                       HARVEST,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);
    me->status.data.devData.h.hoard = hoardExpect;
    me->status.data.devData.h.pqty = nPailExpect;
    me->status.data.devData.h.flow = flowExpect;

    dev = Harvest_ctor(0);

    rkh_fwk_ae_ExpectAndReturn((RKH_ES_T)sizeof(EvtHarvestData),
                               (RKH_SIG_T)evDevData,
                               0,
                               (RKH_EVT_T *)&evtObj);
    rkh_fwk_ae_IgnoreArg_sender();

    TEST_ASSERT_NOT_NULL(dev->vptr->makeEvt);

    evt = (*dev->vptr->makeEvt)(dev, &me->status.data.devData);

    TEST_ASSERT_NOT_NULL(evt);
    TEST_ASSERT_EQUAL(dev, ((EvtHarvestData *)evt)->base.dev);
    TEST_ASSERT_EQUAL(hoardExpect, ((EvtHarvestData *)evt)->hoard);
    TEST_ASSERT_EQUAL(nPailExpect, ((EvtHarvestData *)evt)->nPail);
    TEST_ASSERT_EQUAL(flowExpect, ((EvtHarvestData *)evt)->flow);
}

void
test_UpdateRawOperation(void)
{
    Device *dev;
    uint16_t hoardExpect = 2;
    uint16_t nPailExpect = 4;
    uint16_t flowExpect = 6;
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(HarvestSpy_getObj(),
                       HARVEST,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);
    me->status.data.devData.h.hoard = 2;
    me->status.data.devData.h.pqty = 4;
    me->status.data.devData.h.flow = 6;

    dev = Harvest_ctor(0);

    me->dev = dev;
    TEST_ASSERT_NOT_NULL(dev->vptr->updateRaw);
    TEST_ASSERT_NOT_NULL(dev->collector);
    ((Harvest *)dev)->hoard = hoardExpect;
    ((Harvest *)dev)->nPail = nPailExpect;
    ((Harvest *)dev)->flow = flowExpect;

    (*dev->vptr->updateRaw)(dev);

    TEST_ASSERT_EQUAL(hoardExpect, me->status.data.devData.h.hoard);
    TEST_ASSERT_EQUAL(nPailExpect, me->status.data.devData.h.pqty);
    TEST_ASSERT_EQUAL(flowExpect, me->status.data.devData.h.flow);
}

void
test_UpdateOperation(void)
{
    Device *dev;
    uint16_t hoardExpect = 2;
    uint16_t nPailExpect = 4;
    uint16_t flowExpect = 6;
    EvtHarvestData evtHarvestData;
    RKH_EVT_T *evt;
    Harvest *harvest;
    Collector *me;
    bool result;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(HarvestSpy_getObj(),
                       HARVEST,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);

    dev = Harvest_ctor(0);

    evtHarvestData.base.dev = dev;
    evtHarvestData.hoard = hoardExpect;
    evtHarvestData.nPail = nPailExpect;
    evtHarvestData.flow = flowExpect;
    evt = (RKH_EVT_T *)&evtHarvestData;

    result = (*dev->vptr->update)(dev, evt);

    harvest = (Harvest *)me->dev;
    TEST_ASSERT_EQUAL(dev, me->dev);
    TEST_ASSERT_EQUAL(hoardExpect, harvest->hoard);
    TEST_ASSERT_EQUAL(nPailExpect, harvest->nPail);
    TEST_ASSERT_EQUAL(flowExpect, harvest->flow);
    TEST_ASSERT_EQUAL(false, result);
}

void
test_TestOperation(void)
{
    Device *dev;
    int result = 0;
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    device_ctor_Expect(HarvestSpy_getObj(),
                       HARVEST,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);

    dev = Harvest_ctor(0);

    me->dev = dev;
    ((Harvest *)dev)->nPail = 0;
    TEST_ASSERT_NOT_NULL(dev->vptr->test);
    TEST_ASSERT_NOT_NULL(dev->collector);

    result = (*dev->vptr->test)(dev);
    TEST_ASSERT_EQUAL(false, result);

    ((Harvest *)dev)->nPail = 1;

    result = (*dev->vptr->test)(dev);
    TEST_ASSERT_EQUAL(true, result);
}

void
test_ClearAttributes(void)
{
    Device *dev;                    /* collector attribute */
    Collector *me;
    Harvest *harvest;

    device_ctor_Expect(HarvestSpy_getObj(),
                       HARVEST,
                       (RKH_SMA_T *)collector,
                       (JobCond *)0,
                       (DevVtbl *)0);
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_vtbl();
    device_ctor_StubWithCallback(Mock_device_ctor_Callback);

    me = RKH_DOWNCAST(Collector, collector);
    dev = Harvest_ctor(0);

    ((Harvest *)dev)->hoard = 4;
    ((Harvest *)dev)->nPail = 5;
    ((Harvest *)dev)->flow = 6;

    (*dev->vptr->clear)(dev);

    harvest = (Harvest *)me->dev;
    TEST_ASSERT_EQUAL(dev, me->dev);
    TEST_ASSERT_EQUAL(0, harvest->hoard);
    TEST_ASSERT_EQUAL(0, harvest->nPail);
    TEST_ASSERT_EQUAL(0, harvest->flow);
}

/* ------------------------------ End of file ------------------------------ */
