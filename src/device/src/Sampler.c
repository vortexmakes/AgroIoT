/**
 *  \file       Sampler.c
 *  \brief      Implementation of Sampler concrete module
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "device.h"
#include "Sampler.h"
#include "Collector.h"
#include "rkhfwk_dynevt.h"
#include "signals.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct SamplerJobCond SamplerJobCond;
struct SamplerJobCond
{
    JobCond base;   /* Base class */
};

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static SamplerJobCond samplerJobCond;
static Sampler sampler;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static int
Sampler_test(Device *const me)
{
    return false;
}

static RKH_EVT_T *
Sampler_makeEvt(Device *const me, CBOX_STR *rawData)
{
    EvtSamplerData *evt;

    evt = RKH_ALLOC_EVT(EvtSamplerData, evDevData, 0);
    evt->base.dev = me;
    evt->harness = rawData->a.y;
    evt->deep = rawData->a.z;
    evt->isValid = rawData->a.m;
    return (RKH_EVT_T *)evt;
}

static bool
Sampler_update(Device *const me, RKH_EVT_T *evt)
{
    Sampler *currDev;
    EvtSamplerData *realEvt;
    Collector *collector;
    bool result = true;

    realEvt = (EvtSamplerData *)evt;
    if (realEvt->isValid == true)
    {
        collector = (Collector *)(me->collector);
        collector->dev = realEvt->base.dev;
        currDev = (Sampler *)(collector->dev);

        currDev->harness = realEvt->harness;
        currDev->deep = realEvt->deep;
        currDev->isValid = realEvt->isValid;
    }
    else
    {
        result = false;
    }
    return result;
}

static void
Sampler_updateRaw(Device *const me)
{
    CBOX_STR *rawData = &((Collector *)(me->collector))->status.data.devData;

    rawData->a.x = me->id;
    rawData->a.y = ((Sampler *)me)->harness;
    rawData->a.z = ((Sampler *)me)->deep;
    rawData->a.m = ((Sampler *)me)->isValid;
}

static DevVtbl vtbl = {Sampler_test,
                       Sampler_makeEvt,
                       Sampler_update,
                       Sampler_updateRaw,
                       (ClearOper)0};

/* ---------------------------- Global functions --------------------------- */
Device *
Sampler_ctor(void)
{
    Sampler *me = &sampler;

    device_ctor((Device *)me, SAMPLER, (RKH_SMA_T *)collector,
                (JobCond *)&samplerJobCond, &vtbl);
    me->harness = 0;    /* atttibute default initialization */
    me->deep = 0;       /* atttibute default initialization */
    me->isValid = false;/* atttibute default initialization */
    return (Device *)me;
}

int
SamplerSpy_getHarness(void)
{
    return sampler.harness;
}

int
SamplerSpy_getDeep(void)
{
    return sampler.deep;
}

bool
SamplerSpy_getIsValid(void)
{
    return sampler.isValid;
}

Device *
SamplerSpy_getObj(void)
{
    return (Device *)&sampler;
}

JobCond *
SamplerSpy_getJobCondObj(void)
{
    return (JobCond *)&samplerJobCond;
}

DevVtbl *
SamplerSpy_getVtbl(void)
{
    return (DevVtbl *)&vtbl;
}

/* ------------------------------ End of file ------------------------------ */
