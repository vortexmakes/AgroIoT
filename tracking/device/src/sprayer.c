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
#include "unity.h"
#include "device.h"
#include "sprayer.h"
#include "collector.h"
#include "rkhfwk_dynevt.h"
#include "signals.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct SprayerJobCond SprayerJobCond;
struct SprayerJobCond
{
    JobCond base;
    int nSectionMax;
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
    return 1;
}

static RKH_EVT_T *
sprayer_makeEvt(Device *const me, CBOX_STR *rawData)
{
    EvtSprayerData *evt;

    evt = RKH_ALLOC_EVT(EvtSprayerData, evDevData, 0);
    evt->base.dev = me;
    evt->param.nSection = rawData->hum;
    evt->param.dose = rawData->h.pqty;
    return (RKH_EVT_T *)evt;
}

static void
sprayer_update(Device *const me, RKH_EVT_T *evt)
{
}

static void 
sprayer_updateRaw(Device *const me)
{
}

static DevVtbl vtbl = {sprayer_test, 
                       sprayer_makeEvt, 
                       sprayer_update, 
                       sprayer_updateRaw};

/* ---------------------------- Global functions --------------------------- */
Device * 
sprayer_ctor(int nSectionMax)
{
    SprayerJobCond *jc;
    Sprayer *me = &sprayer;

    device_ctor((Device *)me, SPRAYER, (RKH_SMA_T *)collector, 
                (JobCond *)&sprayerJobCond, &vtbl);
    me->nSection = 0; /* atttibute default initialization */
    me->dose = 0;
    jc = (SprayerJobCond *)(me->base.jobCond); /* it's not quite safe */
    jc->nSectionMax = nSectionMax; /* initializes job condition */
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
sprayerSpy_getNSectionMax(void)
{
    return sprayerJobCond.nSectionMax;
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
