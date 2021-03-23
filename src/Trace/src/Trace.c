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
/* ---------------------------- Local variables ---------------------------- */
static const int trace;
static bool firstTime = true;
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
}

void 
Trace_send(TraceId id, TraceArg arg0, TraceArg arg1)
{
    TraceEvt *evt;

    if (RKH_GET_SMA(RKH_GET_PRIO(collector)) != (RKH_SMA_T*)0)
    {
        if (firstTime == true)
        {
            RKH_TR_FWK_OBJ_NAME(&trace, "trace");
            firstTime = false;
        }
        evt = RKH_ALLOC_EVT(TraceEvt, evTrace, &trace);
        evt->id = id;
        evt->arg0 = arg0;
        evt->arg1 = arg1;
        RKH_SMA_POST_LIFO(collector, RKH_UPCAST(RKH_EVT_T, evt), &trace);
    }
}

void 
Trace_put(TraceId id, TraceArg arg0, TraceArg arg1)
{
    ACCEL_T *trace;

    trace = &traceGStatus.data.devData.a;
    trace->x = id;
    trace->y = arg0;
    trace->z = arg1;

    GStatus_setChecksum(&traceGStatus);
    StatQue_put(&traceGStatus);
    ffile_sync();
}

void 
Trace_generate(GStatusType *status, TraceId id, TraceArg arg0, 
                    TraceArg arg1)
{
    ACCEL_T *trace;

    trace = &status->devData.a;
    if (status != (GStatusType *)0)
    {
        trace->x = id;
        trace->y = arg0;
        trace->z = arg1;
    }
}

void 
Trace_set(GStatusType *status, TraceId id, TraceArg arg0, 
          TraceArg arg1)
{
    ACCEL_T *trace;

    *status = traceGStatus.data;
    trace = &status->devData.a;
    if (status != (GStatusType *)0)
    {
        trace->x = id;
        trace->y = arg0;
        trace->z = arg1;
    }
}

/* ------------------------------ End of file ------------------------------ */
