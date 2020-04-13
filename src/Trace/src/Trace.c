/**
 *  \file       Trace.c
 *  \brief      Implementation of application tracing module.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "Trace.h"
#include "events.h"
#include "signals.h"
#include "rkhfwk_dynevt.h"
#include "Collector.h"
#include "rkhfwk_cast.h"
#include "rkhtrc_record.h"
#include "StatQue.h"
#include "geoMgr.h"
#include "ffile.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
static const int trace;

/* ---------------------------- Local variables ---------------------------- */
static GStatus traceGStatus = 
{
    {
        {
            "000000", {RMC_StatusInvalid},
            {GEO_INVALID_LATITUDE}, {GEO_INVALID_LATITUDE_IND},
            {GEO_INVALID_LONGITUDE}, {GEO_INVALID_LONGITUDE_IND},
            {GEO_INVALID_SPEED}, {GEO_INVALID_COURSE},
            "000000"
        },
        {0, 0, {0, 0, 0}, {0, 0, 0}, 0},
        {0, 0},
        0
    },
    0
};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
Trace_init(void)
{
    RKH_TR_FWK_OBJ_NAME(&trace, "trace");
}

void 
Trace_send(TraceId id, TraceArg arg0, TraceArg arg1)
{
    TraceEvt *evt;

    if (RKH_GET_SMA(RKH_GET_PRIO(collector)) != (RKH_SMA_T*)0)
    {
        evt = RKH_ALLOC_EVT(TraceEvt, evTrace, &trace);
        evt->id = id;
        evt->arg0 = arg0;
        evt->arg1 = arg1;
        RKH_SMA_POST_FIFO(collector, RKH_UPCAST(RKH_EVT_T, evt), &trace);
    }
}

void 
Trace_put(TraceId id, TraceArg arg0, TraceArg arg1)
{
    traceGStatus.data.devData.a.x = id;
    traceGStatus.data.devData.a.y = arg0;
    traceGStatus.data.devData.a.z = arg1;

    GStatus_setChecksum(&traceGStatus);
    StatQue_put(&traceGStatus);
    ffile_sync();
}

/* ------------------------------ End of file ------------------------------ */
