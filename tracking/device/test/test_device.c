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
#include "Mock_jobcond.h"
#include "Mock_rkhassert.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
enum
{
    DEVA, DEVB, NUM_DEVS
};

enum
{
    X1, X2, Y0
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

static int 
DevA_testJobCond(JobCond *const me)
{
    return 1;
}

static void
DevA_ctor(int xMax, int xMin, int yMin)
{
    DevA *me = &devA;
    DevAJobCond *jc;

    device_ctor((Device *)me, DEVA, (JobCond *)&devAJobCond, DevA_testJobCond);
    me->x = 0; /* default initialization */
    me->y = 0;
    jc = (DevAJobCond *)(me->base.jobCond); /* it's not quite safe */
    jc->xMax = xMax; /* initializes the job condition */
    jc->xMin = xMin;
    jc->yMin = yMin;
}

/* ---------------------------- Global functions --------------------------- */
void 
setUp(void)
{
    Mock_jobcond_Init();
}

void 
tearDown(void)
{
    Mock_jobcond_Verify();
    Mock_jobcond_Destroy();
}

void
test_InitAttr(void)
{
    jobcond_ctor_Expect((JobCond *)&devAJobCond, DevA_testJobCond);

    device_ctor((Device *)&devA, DEVA, (JobCond *)&devAJobCond, 
                DevA_testJobCond);

    TEST_ASSERT_EQUAL(DEVA, devA.base.id);
    TEST_ASSERT_EQUAL(&devAJobCond, devA.base.jobCond);
}

void
test_InitConcreteDevice(void)
{
    jobcond_ctor_Expect((JobCond *)&devAJobCond, DevA_testJobCond);

    DevA_ctor(X1, X2, Y0);

    TEST_ASSERT_EQUAL(DEVA, devA.base.id);
    TEST_ASSERT_EQUAL(&devAJobCond, devA.base.jobCond);
}

void
test_FailsWrongArgs(void)
{
    rkh_assert_Expect("device", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    device_ctor((Device *)0, DEVA, (JobCond *)&devAJobCond, DevA_testJobCond);
}

/* ------------------------------ End of file ------------------------------ */
