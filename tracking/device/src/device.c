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
device_ctor(Device *const me, int id, JobCond *jobCond, 
            TestOper testOper, MakeEvtOper makeEvt, UpdateOper update)
{
    RKH_REQUIRE((me != (Device *)0) && 
                (jobCond != (JobCond *)0) &&
                (testOper != (TestOper)0) &&
                (makeEvt!= (MakeEvtOper)0) &&
                (update!= (UpdateOper)0));

    me->jobCond = jobCond;
    me->jobCond->test = testOper;
    me->id = id;
    me->makeEvt = makeEvt;
    me->update = update;
}

RKH_EVT_T * 
device_makeEvt(Device *const me, CBOX_STR *rawData)
{
    RKH_REQUIRE((me != (Device *)0) && (rawData != (CBOX_STR *)0) &&
                (me->makeEvt != (MakeEvtOper)0));
    return (*me->makeEvt)(me, rawData);
}

void 
device_update(Device *const me, RKH_EVT_T *evt)
{
    RKH_REQUIRE((me != (Device *)0) && (evt != (RKH_EVT_T *)0) &&
                (me->update != (UpdateOper)0));
    (*me->update)(me, evt);
}

/* ------------------------------ End of file ------------------------------ */
