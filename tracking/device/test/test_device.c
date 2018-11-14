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

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static DevAJobCond devAJobCond;
static DevA devA;               /* It must be statically instantiated in a */
                                /* concrete class (c source file) */

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
DevA_testJobCond(Device *const me)
{
    DevAJobCond *jc;
    DevA *realMe;
    int result = 1;

    realMe = (DevA *)me;
    jc = (DevAJobCond *)(me->jobCond);
    if ((realMe->x > jc->xMax) || (realMe->x < jc->xMin) || 
        (realMe->y < jc->yMin))
    {
        result = 0;
    }
    return result;
}

static void
DevA_makeEvt(Device *const me, CBOX_STR *rawData)
{
    DevA *realMe;

    /* allocates an Evt? object */
    /* sets its attributes from rawData */
    /* return the object address */
    realMe = (DevA *)me;
    realMe->x = rawData->a.y;
    realMe->y = rawData->a.z;
}

static Device *
DevA_getInstance(void)
{
    return (Device *)&devA;
}

static Device *
DevA_ctor(int xMin, int xMax, int yMin)
{
    DevAJobCond *jc;

    DevA *me = &devA;
    device_ctor((Device *)me, DEVA, (JobCond *)&devAJobCond, 
                DevA_testJobCond, DevA_makeEvt);
    me->x = 0; /* default initialization */
    me->y = 0;
    jc = (DevAJobCond *)(me->base.jobCond); /* it's not quite safe */
    jc->xMax = xMax; /* initializes the job condition */
    jc->xMin = xMin;
    jc->yMin = yMin;
    return (Device *)me;
}

static void
DevA_set(int x, int y)
{
    DevA *me = &devA;

    me->x = x;
    me->y = y;
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

    device_ctor(me, DEVA, (JobCond *)&devAJobCond, DevA_testJobCond, 
                DevA_makeEvt);

    TEST_ASSERT_EQUAL(DEVA, me->id);
    TEST_ASSERT_EQUAL(&devAJobCond, me->jobCond);
}

void
test_InitConcreteDevice(void)
{
    Device *me = (Device *)0;
    me = DevA_ctor(1, 8, 3);
    TEST_ASSERT_NOT_NULL(me);
}

void
test_CallsTestOper(void)
{
    Device *me;
    int result;

    me = DevA_ctor(2, 8, 3);
    DevA_set(5, 4);

    result = (*me->jobCond->test)(me);
    TEST_ASSERT_EQUAL(1, result);
}

void
test_FailsWrongArgs(void)
{
    rkh_assert_Expect("device", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    device_ctor((Device *)0, DEVA, (JobCond *)&devAJobCond, DevA_testJobCond, 
                DevA_makeEvt);
}

void
test_TransformsReceivedRawDataToDeviceClass(void)
{
    Device *me;
    CBOX_STR rawData;

    me = DevA_ctor(2, 8, 3);
	rawData.a.x = DEVA;
	rawData.a.y = 4;
	rawData.a.z = 5;

    (*me->makeEvt)(me, &rawData);
}

/* ------------------------------ End of file ------------------------------ */
