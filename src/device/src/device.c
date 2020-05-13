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
#include "rkhsma.h"
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
device_ctor(Device *const me, int id, RKH_SMA_T *collector, JobCond *jobCond,
            DevVtbl *vtbl)
{
    RKH_REQUIRE((me != (Device *)0) && (jobCond != (JobCond *)0) &&
                (vtbl != (DevVtbl *)0));

    me->jobCond = jobCond;

    me->id = id;
    me->vptr = vtbl;
    me->collector = collector;
}

RKH_EVT_T *
device_makeEvt(Device *const me, CBOX_STR *rawData)
{
    RKH_REQUIRE((me != (Device *)0) && (me->vptr->makeEvt != (MakeEvtOper)0));
    return (*me->vptr->makeEvt)(me, rawData);
}

bool
device_update(Device *const me, RKH_EVT_T *evt)
{
    RKH_REQUIRE((me != (Device *)0) && (evt != (RKH_EVT_T *)0) &&
                (me->vptr->makeEvt != (MakeEvtOper)0));
    return (*me->vptr->update)(me, evt);
}

int
device_test(Device *const me)
{
    RKH_REQUIRE((me != (Device *)0) && (me->vptr->test != (TestOper)0));
    return (*me->vptr->test)(me);
}

void
device_updateRaw(Device *const me)
{
    RKH_REQUIRE((me != (Device *)0) &&
                (me->vptr->updateRaw != (UpdateRawOper)0));
    (*me->vptr->updateRaw)(me);
}

/* ------------------------------ End of file ------------------------------ */
