/**
 *  \file       jobcond.c
 *  \brief      Implementation of job condition module.
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
#include "jobcond.h"
#include "rkhassert.h"
#include "rkhitl.h"     /* It's needed to include platform files */

RKH_MODULE_NAME(jobcond)

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void 
jobcond_ctor(JobCond *const me, TestOper testOper)
{
    RKH_REQUIRE((me != (JobCond *)0) && (testOper != (TestOper)0));
    me->test = testOper;
}

/* ------------------------------ End of file ------------------------------ */
