/**
 *  \file       Flowmeter.c
 *  \brief      Implements the flowmeter device.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.17.06  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkhitl.h"
#include "rkhassert.h"
#include "rkhfwk_cast.h"
#include "Flowmeter.h"
#include "events.h"
#include "signals.h"
#include "rkhfwk_dynevt.h"

/* ----------------------------- Local macros ------------------------------ */
RKH_MODULE_NAME(Flowmeter)

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
Flowmeter_init(Flowmeter *me)
{
    RKH_REQUIRE(me != (Flowmeter *)0);

    me->flow1.nPulses = me->flow2.nPulses = 0;
    me->flow1.dir = me->flow2.dir = Forward;
}

void
Flowmeter_update(Flowmeter *me, RKH_EVT_T *event)
{
    FlowmeterEvt *realEvt;

    RKH_REQUIRE((me != (Flowmeter *)0) && (event != (RKH_EVT_T *)0));

    realEvt = RKH_DOWNCAST(FlowmeterEvt, event);
    me->flow1.nPulses = realEvt->flow1.nPulses;
    me->flow1.dir = realEvt->flow1.dir;
    me->flow2.nPulses = realEvt->flow2.nPulses;
    me->flow2.dir = realEvt->flow2.dir;
}

void
Flowmeter_updateRaw(Flowmeter *me, CBOX_STR *status)
{
    RKH_REQUIRE((me != (Flowmeter *)0) && (status != (CBOX_STR *)0));

    status->a.y = 0;
    status->a.y = (me->flow2.nPulses << 8) | me->flow1.nPulses;
    status->a.z = 0;
    status->a.z = (me->flow2.dir << 1) | me->flow1.dir;
}

RKH_EVT_T *
Flowmeter_makeEvt(FlowData *flow1, FlowData *flow2)
{
    FlowmeterEvt *event;

    RKH_REQUIRE((flow1 != (FlowData *)0) && (flow2 != (FlowData *)0));

    event = RKH_ALLOC_EVT(FlowmeterEvt, evFlowmeter, 0);
    event->flow1.nPulses = flow1->nPulses;
    event->flow2.nPulses = flow2->nPulses;
    event->flow1.dir = flow1->dir;
    event->flow2.dir = flow2->dir;
    return (RKH_EVT_T *)event;
}

/* ------------------------------ End of file ------------------------------ */
