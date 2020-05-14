/**
 *  \file       Skeleton.c
 *  \brief      Implementation of Skeleton concrete module
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "device.h"
#include "Skeleton.h"
#include "Collector.h"
#include "rkhfwk_dynevt.h"
#include "signals.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct SkeletonJobCond SkeletonJobCond;
struct SkeletonJobCond
{
    JobCond base;   /* Base class */
    int x;          /* Attribute used to check job condition */
};

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static SkeletonJobCond skeletonJobCond;
static Skeleton skeleton;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static int
Skeleton_test(Device *const me)
{
    SkeletonJobCond *jc;
    Skeleton *realMe;
    int result;

    realMe = (Skeleton *)me;
    jc = (SkeletonJobCond *)(me->jobCond);
    result = (realMe->x > jc->xMax) ? 1 : 0;
    return result;
}

static RKH_EVT_T *
Skeleton_makeEvt(Device *const me, CBOX_STR *rawData)
{
    EvtSkeletonData *evt;

    evt = RKH_ALLOC_EVT(EvtSkeletonData, evDevData, 0);
    evt->base.dev = me;
    evt->x = rawData->hum;
    return (RKH_EVT_T *)evt;
}

static bool
Skeleton_update(Device *const me, RKH_EVT_T *evt)
{
    Skeleton *currDev;
    EvtSkeletonData *realEvt;
    Collector *collector;

    realEvt = (EvtSkeletonData *)evt;
    collector = (Collector *)(me->collector);

    collector->dev = realEvt->base.dev;
    collector->status.data.devData.a.x = collector->dev->id;
    currDev = (Skeleton *)(collector->dev);
    currDev->x = realEvt->x;
    return false;
}

static void
Skeleton_updateRaw(Device *const me)
{
    CBOX_STR *rawData = &((Collector *)(me->collector))->status.data.devData;

    rawData->hum = ((Skeleton *)me)->x;
}

static DevVtbl vtbl = {Skeleton_test,
                       Skeleton_makeEvt,
                       Skeleton_update,
                       Skeleton_updateRaw};

/* ---------------------------- Global functions --------------------------- */
Device *
Skeleton_ctor(int xMax)
{
    SkeletonJobCond *jc;
    Skeleton *me = &skeleton;

    device_ctor((Device *)me, SKELETON, (RKH_SMA_T *)collector,
                (JobCond *)&skeletonJobCond, &vtbl);
    me->x = 0; /* atttibute default initialization */
    jc = (SkeletonJobCond *)(me->base.jobCond); /* it's not quite safe */
    jc->xMax = xMax; /* initializes job condition parameter */
    return (Device *)me;
}

int
skeletonSpy_getX(void)
{
    return skeleton.x;
}

Device *
skeletonSpy_getObj(void)
{
    return (Device *)&skeleton;
}

JobCond *
skeletonSpy_getJobCondObj(void)
{
    return (JobCond *)&skeletonJobCond;
}

DevVtbl *
skeletonSpy_getVtbl(void)
{
    return (DevVtbl *)&vtbl;
}

/* ------------------------------ End of file ------------------------------ */
