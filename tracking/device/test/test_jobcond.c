/**
 *  \file       test_jobcond.c
 *  \brief      Unit test for job condition module.
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
#include "jobcond.h"
#include "Mock_rkhassert.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct DevAJobCond DevAJobCond;
struct DevAJobCond
{
    JobCond base;
    int xMax;
    int xMin;
    int yMin;
};

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static DevAJobCond devAJobCond;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void 
MockAssertCallback(const char* const file, int line, int cmock_num_calls)
{
    TEST_PASS();
}

static int 
DevA_test(JobCond *const me)
{
    return 1;
}

/* ---------------------------- Global functions --------------------------- */
void 
setUp(void)
{
    Mock_rkhassert_Init();
}

void 
tearDown(void)
{
    Mock_rkhassert_Verify();
    Mock_rkhassert_Destroy();
}

void
test_InitAttr(void)
{
    JobCond *me = (JobCond *)&devAJobCond;

    jobcond_ctor(me, DevA_test);
    TEST_ASSERT_EQUAL(DevA_test, me->test);
}

void
test_FailsWrongArgs(void)
{
    JobCond *me = (JobCond *)&devAJobCond;

    rkh_assert_Expect("jobcond", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    jobcond_ctor((JobCond *)0, DevA_test);

    rkh_assert_Expect("jobcond", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    jobcond_ctor(me, (TestOper)0);
}

/* ------------------------------ End of file ------------------------------ */
