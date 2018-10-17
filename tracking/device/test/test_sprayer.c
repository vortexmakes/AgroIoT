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
#include "Mock_rkhassert.h"
#include "Mock_device.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
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
test_InitAttr(void)
{
    int sectionThd_Expect = 5;

    device_ctor_Expect(0, SPRAYER, 0, 0);
    device_ctor_IgnoreArg_me();
    device_ctor_IgnoreArg_jobCond();
    device_ctor_IgnoreArg_testOper();

    sprayer_ctor(sectionThd_Expect);
}

void
test_FailsWrongArgs(void)
{
#if 0
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
#else
    TEST_IGNORE();
#endif
}

/* ------------------------------ End of file ------------------------------ */
