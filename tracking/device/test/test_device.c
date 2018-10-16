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
#include "jobcond.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
enum
{
    DEVA, DEVB, NUM_DEVS
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

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static int 
DevA_test(JobCond *const me)
{
    return 1;
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
test_CtorInitAttr(void)
{
    DevAJobCond devAJobCond;

    device_ctor(DEVA, &(JobCond *)&devAJobCond);
    TEST_ASSERT_
}

/* ------------------------------ End of file ------------------------------ */
