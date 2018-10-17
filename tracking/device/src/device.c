/**
 *  \file       device.c
 *  \brief      Implementation of Device module.
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
#include "device.h"
#include "jobcond.h"
#include "rkhassert.h"
#include "rkhitl.h"     /* It's needed to include platform files */

RKH_MODULE_NAME(device)

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void 
device_ctor(Device *const me, int id, JobCond *jobCond, TestOper testOper)
{
    RKH_REQUIRE(me != (Device *)0);

    jobcond_ctor(jobCond, testOper);
    me->id = id;
    me->jobCond = jobCond;
}

/* ------------------------------ End of file ------------------------------ */
