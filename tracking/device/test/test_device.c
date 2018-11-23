/**
 *  \file       test_device.c
 *  \brief      Unit test for device module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.16.10  LeFr  v1.0.00  ---
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "device.h"
#include "Mock_rkhassert.h"
#include "Mock_rkhevt.h"
#include "Mock_collector.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
enum
{
    DEVA, DEVB
};

/* ---------------------------- Local data types --------------------------- */
typedef struct DevAJobCond DevAJobCond;
struct DevAJobCond
{
    JobCond base;
    int xMax;
    int xMin;
    int yMin;
};

typedef struct DevA DevA;
struct DevA
{
    Device base;
    int x;
    int y;
};

typedef struct EvtDevData EvtDevData;
struct EvtDevData
{
    RKH_EVT_T base;
    Device *dev;
};

typedef struct EvtDevAData EvtDevAData;
struct EvtDevAData
{
    EvtDevData base;
    DevA param;
};

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static DevAJobCond devAJobCond;
static DevA devA;               /* It must be statically instantiated in a...*/
                                /* ...concrete class (c source file) */
static EvtDevAData evtDevAData;
static Collector collector;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void 
MockAssertCallback(const char* const file, int line, int cmock_num_calls)
{
    TEST_PASS();
}

static void
MockJobCondCtorCallback(JobCond *const me, TestOper testOper, 
                        int cmock_num_calls)
{
    me->test = testOper;
}

static int 
DevA_testJobCond(JobCond *const me)
{
    DevAJobCond *jc;
    DevA *realMe;
    int result = 1;

    realMe = (DevA *)(me->dev);
    jc = (DevAJobCond *)me;
    if ((realMe->x > jc->xMax) || (realMe->x < jc->xMin) || 
        (realMe->y < jc->yMin))
    {
        result = 0;
    }
    return result;
}

static RKH_EVT_T *
DevA_makeEvt(Device *const me, CBOX_STR *rawData)
{
    /* EvtDevAData *evtDevAData;... */
    /* ...evtDevAData = RKH_ALLOC_EVT(EvtDevAData, evDevAData, &devA);... */
    /* ...allocates an EvtDevAData object */
    evtDevAData.base.dev = me;
    evtDevAData.param.x = rawData->a.y; /* sets its attributes from rawData */
    evtDevAData.param.y = rawData->a.z;
}

static void
DevA_update(Device *const me, RKH_EVT_T *evt)
{
    DevA *dev;
    EvtDevAData *devEvt;

    devEvt = (EvtDevAData *)evt;
    dev = (DevA *)(((EvtDevData *)devEvt)->dev);
    dev->x = devEvt->param.x;
    dev->y = devEvt->param.y;
}

static void 
DevA_updateRaw(Device *const me)
{
    ((Collector *)(me->jobCond->collector))->rawData.a.y = ((DevA *)me)->x;
    ((Collector *)(me->jobCond->collector))->rawData.a.z = ((DevA *)me)->y;
}

static Device *
DevA_ctor(int xMin, int xMax, int yMin) /* Parameters of job condition */
{
    DevAJobCond *jc;
    static DevVtbl vtbl = {DevA_makeEvt, DevA_update, DevA_updateRaw};

    DevA *me = &devA;
    device_ctor((Device *)me, DEVA, (RKH_SMA_T *)&collector, 
                (JobCond *)&devAJobCond, 
                DevA_testJobCond, &vtbl);
    me->x = 0; /* atttibute default initialization */
    me->y = 0;
    jc = (DevAJobCond *)(me->base.jobCond); /* it's not quite safe */
    jc->xMax = xMax; /* initializes job condition */
    jc->xMin = xMin;
    jc->yMin = yMin;
    return (Device *)me;
}

static Device *
getDevice(int devId)
{
    Device *dev;

    if (devId == DEVA)
    {
        dev = (Device *)&devA;
    }
    else
    {
        dev = (Device *)0;
    }
    return dev;
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
test_InitAttr(void)
{
    Device *me = (Device *)&devA;
    DevVtbl *vtbl;

    device_ctor(me, DEVA, (RKH_SMA_T *)&collector, (JobCond *)&devAJobCond, 
                DevA_testJobCond, vtbl);

    TEST_ASSERT_EQUAL(DEVA, me->id);
    TEST_ASSERT_EQUAL(&devAJobCond, me->jobCond);
    TEST_ASSERT_EQUAL(vtbl, me->vptr);
}

void
test_InitConcreteDevice(void)
{
    Device *me = (Device *)0;

    me = DevA_ctor(1, 8, 3);
    TEST_ASSERT_NOT_NULL(me);
}

void
test_FailsWrongArgs(void)
{
    rkh_assert_Expect("device", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    device_ctor((Device *)0, DEVA, (RKH_SMA_T *)&collector,
                (JobCond *)&devAJobCond, DevA_testJobCond, 
                (DevVtbl *)0);
}

void
test_MakesAnEventFromReceivedRawData(void)
{
    Device *devAObj, *dev;
    RKH_EVT_T *evt;
    CBOX_STR rawData;

    devAObj = DevA_ctor(2, 8, 3);   /* from main() */
	rawData.a.x = DEVA;             /* from prosens */
	rawData.a.y = 4;
	rawData.a.z = 5;
    dev = getDevice(rawData.a.x);   /* from prosens */
    TEST_ASSERT_NOT_NULL(dev);

    evt = device_makeEvt(dev, &rawData);
    TEST_ASSERT_NOT_NULL(evt);
}

void
test_UpdateDeviceAttributes(void)
{
    Device *devAObj, *dev;          /* collector attribute */
    RKH_EVT_T *evt;                 /* transition event */

    devAObj = DevA_ctor(2, 8, 3);   /* from main() */
    evtDevAData.base.dev = devAObj; /* from prosens */
    evtDevAData.param.x = 4;
    evtDevAData.param.y = 5;
    evt = (RKH_EVT_T *)&evtDevAData;
    dev = collector.dev = ((EvtDevData *)evt)->dev; /* from updateDevData() */
    TEST_ASSERT_NOT_NULL(dev);

    device_update(dev, evt);

    TEST_ASSERT_EQUAL(DEVA, dev->id);
    TEST_ASSERT_EQUAL(4, ((DevA *)dev)->x);
    TEST_ASSERT_EQUAL(5, ((DevA *)dev)->y);
}

void
test_TestJobCondition(void)
{
    Device *dev;
    DevA *devAObj;
    int result;

    dev = DevA_ctor(2, 8, 3);
    devAObj = (DevA *)dev;
    devAObj->x = 4;
    devAObj->y = 8;

    result = device_test(dev);
    TEST_ASSERT_EQUAL(1, result);

    devAObj->x = 1;
    result = device_test(dev);
    TEST_ASSERT_EQUAL(0, result);

    devAObj->x = 9;
    result = device_test(dev);
    TEST_ASSERT_EQUAL(0, result);

    devAObj->y = 1;
    result = device_test(dev);
    TEST_ASSERT_EQUAL(0, result);
}

void
test_UpdateRawData(void)
{
    Device *dev;
    DevA *devAObj;

    dev = DevA_ctor(2, 8, 3);
    devAObj = (DevA *)dev;
    devAObj->x = 4;
    devAObj->y = 8;
    collector.rawData.a.y = collector.rawData.a.z = 0;

    device_updateRaw(dev);
    TEST_ASSERT_EQUAL(4, collector.rawData.a.y);
    TEST_ASSERT_EQUAL(8, collector.rawData.a.z);
}

/* ------------------------------ End of file ------------------------------ */
