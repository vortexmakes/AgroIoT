/**
 *  \file       sprayer.c
 *  \brief      Implementation of sprayer concrete module
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
#include "sprayer.h"
#include "Collector.h"
#include "rkhfwk_dynevt.h"
#include "signals.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct SprayerJobCond SprayerJobCond;
struct SprayerJobCond
{
    JobCond base;
    int doseMax;
};

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static SprayerJobCond sprayerJobCond;
static Sprayer sprayer;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static int
sprayer_test(Device *const me)
{
    SprayerJobCond *jc;
    Sprayer *realMe;
    int result;

    realMe = (Sprayer *)me;
    jc = (SprayerJobCond *)(me->jobCond);
    result = (realMe->dose > jc->doseMax) ? 1 : 0;
    return result;
}

static RKH_EVT_T *
sprayer_makeEvt(Device *const me, CBOX_STR *rawData)
{
    EvtSprayerData *evt;

    evt = RKH_ALLOC_EVT(EvtSprayerData, evDevData, 0);
    evt->base.dev = me;
    evt->nSection = rawData->hum;
    evt->dose = rawData->h.pqty;
    return (RKH_EVT_T *)evt;
}

static void
sprayer_update(Device *const me, RKH_EVT_T *evt)
{
    Sprayer *currDev;
    EvtSprayerData *realEvt;
    Collector *collector;

    realEvt = (EvtSprayerData *)evt;
    collector = (Collector *)(me->collector);

    collector->dev = realEvt->base.dev;
    collector->status.data.devData.a.x = collector->dev->id;
    currDev = (Sprayer *)(collector->dev);
    currDev->nSection = realEvt->nSection;
    currDev->dose = realEvt->dose;
}

static void
sprayer_updateRaw(Device *const me)
{
    CBOX_STR *rawData = &((Collector *)(me->collector))->status.data.devData;

    rawData->hum = ((Sprayer *)me)->nSection;
    rawData->h.pqty = ((Sprayer *)me)->dose;
}

static DevVtbl vtbl = {sprayer_test,
                       sprayer_makeEvt,
                       sprayer_update,
                       sprayer_updateRaw};

/* ---------------------------- Global functions --------------------------- */
Device *
sprayer_ctor(int doseMax)
{
    SprayerJobCond *jc;
    Sprayer *me = &sprayer;

    device_ctor((Device *)me, SPRAYER, (RKH_SMA_T *)collector,
                (JobCond *)&sprayerJobCond, &vtbl);
    me->nSection = 0; /* atttibute default initialization */
    me->dose = 0;
    jc = (SprayerJobCond *)(me->base.jobCond); /* it's not quite safe */
    jc->doseMax = doseMax; /* initializes job condition */
    return (Device *)me;
}

int
sprayerSpy_getNSection(void)
{
    return sprayer.nSection;
}

int
sprayerSpy_getDose(void)
{
    return sprayer.dose;
}

int
sprayerSpy_getDoseMax(void)
{
    return sprayerJobCond.doseMax;
}

Device *
sprayerSpy_getObj(void)
{
    return (Device *)&sprayer;
}

JobCond *
sprayerSpy_getJobCondObj(void)
{
    return (JobCond *)&sprayerJobCond;
}

DevVtbl *
sprayerSpy_getVtbl(void)
{
    return (DevVtbl *)&vtbl;
}

/* ------------------------------ End of file ------------------------------ */
