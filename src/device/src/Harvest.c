/**
 *  \file       Harvest.c
 *  \brief      Implementation of Harvest concrete module
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "device.h"
#include "Harvest.h"
#include "Collector.h"
#include "rkhfwk_dynevt.h"
#include "signals.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct HarvestJobCond HarvestJobCond;
struct HarvestJobCond
{
    JobCond base;   /* Base class */
    uint16_t nPailMin; /* Attribute used to check job condition */
};

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static HarvestJobCond harvestJobCond;
static Harvest harvest;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static int
Harvest_test(Device *const me)
{
    HarvestJobCond *jc;
    Harvest *realMe;
    int result;

    realMe = (Harvest *)me;
    jc = (HarvestJobCond *)(me->jobCond);
    result = (realMe->nPail > jc->nPailMin) ? 1 : 0;
    return result;
}

static RKH_EVT_T *
Harvest_makeEvt(Device *const me, CBOX_STR *rawData)
{
    EvtHarvestData *evt;

    evt = RKH_ALLOC_EVT(EvtHarvestData, evDevData, 0);
    evt->base.dev = me;
    evt->hoard = rawData->h.hoard;
    evt->nPail = rawData->h.pqty;
    evt->flow = rawData->h.flow;
    return (RKH_EVT_T *)evt;
}

static bool
Harvest_update(Device *const me, RKH_EVT_T *evt)
{
    Harvest *currDev;
    EvtHarvestData *realEvt;
    Collector *collector;

    realEvt = (EvtHarvestData *)evt;
    collector = (Collector *)(me->collector);

    collector->dev = realEvt->base.dev;
    collector->status.data.devData.a.x = collector->dev->id;
    currDev = (Harvest *)(collector->dev);
    currDev->hoard += realEvt->hoard;
    currDev->nPail += realEvt->nPail;
    currDev->flow += realEvt->flow;
    return false;
}

static void
Harvest_updateRaw(Device *const me)
{
    CBOX_STR *rawData = &((Collector *)(me->collector))->status.data.devData;

    rawData->h.hoard = ((Harvest *)me)->hoard;
    rawData->h.pqty = ((Harvest *)me)->nPail;
    rawData->h.flow = ((Harvest *)me)->flow;
}

static void
Harvest_clear(Device *const me)
{
    Harvest *realMe;
    Collector *collector;

    collector = (Collector *)(me->collector);
    realMe = (Harvest *)(collector->dev);
    realMe->hoard = 0;
    realMe->nPail = 0;
    realMe->flow = 0;
}

static DevVtbl vtbl = {Harvest_test,
                       Harvest_makeEvt,
                       Harvest_update,
                       Harvest_updateRaw,
                       Harvest_clear};

/* ---------------------------- Global functions --------------------------- */
Device *
Harvest_ctor(uint16_t nPailMin)
{
    HarvestJobCond *jc;
    Harvest *me = &harvest;

    device_ctor((Device *)me, HARVEST, (RKH_SMA_T *)collector,
                (JobCond *)&harvestJobCond, &vtbl);
    me->hoard = 0; /* atttibute default initialization */
    me->nPail = 0; /* atttibute default initialization */
    me->flow = 0; /* atttibute default initialization */
    jc = (HarvestJobCond *)(me->base.jobCond); /* it's not quite safe */
    jc->nPailMin = nPailMin; /* initializes job condition parameter */
    return (Device *)me;
}

uint16_t
HarvestSpy_getHoard(void)
{
    return harvest.hoard;
}

uint16_t
HarvestSpy_getNPail(void)
{
    return harvest.nPail;
}

uint16_t
HarvestSpy_getFlow(void)
{
    return harvest.flow;
}

Device *
HarvestSpy_getObj(void)
{
    return (Device *)&harvest;
}

JobCond *
HarvestSpy_getJobCondObj(void)
{
    return (JobCond *)&harvestJobCond;
}

DevVtbl *
HarvestSpy_getVtbl(void)
{
    return (DevVtbl *)&vtbl;
}

/* ------------------------------ End of file ------------------------------ */
