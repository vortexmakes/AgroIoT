/**
 *  \file       CollectorAct.c
 *  \brief      Active object's action implementations.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkhsma.h"
#include "rkhsm.h"
#include "rkhfwk_cast.h"
#include "Collector.h"
#include "CollectorAct.h"
#include "CollectorActRequired.h"
#include "topic.h"
#include "signals.h"
#include "events.h"
#include "rkhtmr.h"
#include "rkhfwk_dynevt.h"
#include "device.h"
#include "rkhassert.h"
#include "Config.h"
#include "StatQue.h"
#include "GStatus.h"
#include "ffile.h"
#include "geoMgr.h"
#include "Trace.h"
#include "Backup.h"
#include "Flowmeter.h"

RKH_MODULE_NAME(CollectorAct)

/* ----------------------------- Local macros ------------------------------ */
#define GEO_INVALID_GEOSTAMP    \
    { \
        {GEO_INVALID_UTC}, {RMC_StatusInvalid}, \
        {GEO_INVALID_LATITUDE}, {GEO_INVALID_LATITUDE_IND}, \
        {GEO_INVALID_LONGITUDE}, {GEO_INVALID_LONGITUDE_IND}, \
        {GEO_INVALID_SPEED}, {GEO_INVALID_COURSE}, \
        {GEO_INVALID_DATE} \
    }

/* ------------------------------- Constants ------------------------------- */
static const Geo invalidPosition = GEO_INVALID_GEOSTAMP;

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
RKH_DCLR_SM_CONST_GLOBAL(mapping);
RKH_SM_T *collectorMapping;

/* ---------------------------- Local variables ---------------------------- */
static RKH_ROM_STATIC_EVENT(evMappingObj, evMapping);
static RKH_ROM_STATIC_EVENT(evNoMappingObj, evNoMapping);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
dispatch(RKH_SMA_T *me, void *arg)
{
    Mapping *region;

    region = &(RKH_DOWNCAST(Collector, me)->itsMapping);
    rkh_sm_dispatch((RKH_SM_T *)me, (RKH_EVT_T *)arg);
    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, region), (RKH_EVT_T *)arg);
}

static bool
testDevNullJobCond(Collector *const me)
{
    return (me->status.data.ioStatus.digIn != 0xff) ? true : false;
}

static RKH_TNT_T
getMapTimeOnStopped(void)
{
    return RKH_TIME_SEC(Config_getConnTime());
}

static RKH_TNT_T
getMapTimeOnRunning(void)
{
    return RKH_TIME_SEC(Config_getMapTimeOnRunning());
}

static void
propagateMappingEvent(Collector *const me, int result)
{
    const RKH_EVT_T *evt;

    evt = result ? &evMappingObj : &evNoMappingObj;
    RKH_SMA_POST_LIFO(RKH_UPCAST(RKH_SMA_T, me), evt, me);
}

static void
setMappingStatus(Mapping *const me, int status)
{
    if (me->itsCollector->base.sm.state == 
        RKH_CAST(RKH_ST_T, &DevStatus_DevNotConnected))
    {
        me->itsCollector->status.data.devData.a.x = status;
    }
}

static void
storeStatus(Mapping *const me)
{
    GStatus_setChecksum(&me->itsCollector->status);
    StatQue_put(&me->itsCollector->status);
    Backup_store(&me->itsCollector->status);
    ++me->nStoreLastSync;
}

/* ---------------------------- Global functions --------------------------- */
void
Collector_ctor(void)
{
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    me->vtbl = rkhSmaVtbl;
    me->vtbl.task = dispatch;
    me->status.data.position = invalidPosition;
    me->status.data.devData.a.x = MAPPING_STOP;
    me->status.data.ioStatus.digIn = 0xff;
    me->status.data.ioStatus.digOut = 0;
    rkh_sma_ctor(RKH_UPCAST(RKH_SMA_T, me), &me->vtbl);

    me->itsMapping.itsCollector = me;
    me->itsMapping.nStoreLastSync = 0;
    RKH_SM_INIT(&me->itsMapping,
                mapping, 0, HCAL,
                Mapping_Active, NULL, NULL);
    collectorMapping = (RKH_SM_T *)&(me->itsMapping);
}

/* ............................ Effect actions ............................. */
void
Collector_init(Collector *const me, RKH_EVT_T *pe)
{
    (void)pe;

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_AO(collectorMapping);
    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &DevStatus_Active);
    RKH_TR_FWK_STATE(me, &DevStatus_DevNotConnected);
    RKH_TR_FWK_STATE(me, &DevStatus_DevConnected);
    RKH_TR_FWK_STATE(me, &Mapping_Active);
    RKH_TR_FWK_STATE(me, &Mapping_Stopped);
    RKH_TR_FWK_STATE(me, &Mapping_Running);
    RKH_TR_FWK_PSTATE(me, &Mapping_C1);
    RKH_TR_FWK_PSTATE(me, &Mapping_C2);
    RKH_TR_FWK_PSTATE(me, &Mapping_C3);
    RKH_FILTER_OFF_SMA(collectorMapping);

    topic_subscribe(Status, RKH_UPCAST(RKH_SMA_T, me));
    rkh_sm_init(RKH_UPCAST(RKH_SM_T, &me->itsMapping));
    Flowmeter_init(&me->flowmeter);
}

void
Collector_updatePosition(Collector *const me, RKH_EVT_T *pe)
{
    me->status.data.position = RKH_DOWNCAST(GeoEvt, pe)->position;
}

void
Collector_updateDigOut(Collector *const me, RKH_EVT_T *pe)
{
    me->status.data.ioStatus.digOut = RKH_DOWNCAST(DigOutChangedEvt, pe)->status;
}

void
Collector_publishCurrStatus(Collector *const me, RKH_EVT_T *pe)
{
    GStatusEvt *evt;

    evt = RKH_ALLOC_EVT(GStatusEvt, evGStatus, me);
    evt->status = me->status.data;
    topic_publish(Status,
                  RKH_UPCAST(RKH_EVT_T, evt),
                  RKH_UPCAST(RKH_SMA_T, me));
}

void
Collector_updateDigInTestDevNull(Collector *const me, RKH_EVT_T *pe)
{
    int result;

    me->status.data.ioStatus.digIn = RKH_DOWNCAST(DigInChangedEvt, pe)->status;
    result = testDevNullJobCond(me);
    propagateMappingEvent(me, result);
}

void
Collector_updateDigIn(Collector *const me, RKH_EVT_T *pe)
{
    me->status.data.ioStatus.digIn = RKH_DOWNCAST(DigInChangedEvt, pe)->status;
}

void
Collector_updateAndTestDevData(Collector *const me, RKH_EVT_T *pe)
{
    EvtDevData *evtDevData;
    int result;

    evtDevData = RKH_DOWNCAST(EvtDevData, pe);
    RKH_REQUIRE(evtDevData->dev != (Device *)0);
    me->dev = evtDevData->dev; /* obtain device's instance */

    result = device_update(me->dev, RKH_UPCAST(RKH_EVT_T, evtDevData));
    device_updateRaw(me->dev);
    if (result == true)
    {
        storeStatus(&me->itsMapping);
    }
    result = device_test(me->dev);
    propagateMappingEvent(me, result);
}

void
Collector_storeTrace(Collector *const me, RKH_EVT_T *pe)
{
    TraceEvt *event;

    event = RKH_DOWNCAST(TraceEvt, pe);
    Trace_put(event->id, event->arg0, event->arg1);
}

void
Collector_initBackup(Collector *const me, RKH_EVT_T *pe)
{
    Backup_init(&me->backupInfo);
}

void
Collector_deinitBackup(Collector *const me, RKH_EVT_T *pe)
{
    Backup_deinit(&me->backupInfo);
}

void
Collector_updateFlowmeter(Collector *const me, RKH_EVT_T *pe)
{
    Flowmeter_update(&me->flowmeter, pe);
    Flowmeter_updateRaw(&me->flowmeter, &me->status.data.devData);
}

void
Mapping_storeStatus(Mapping *const me, RKH_EVT_T *pe)
{
    storeStatus(me);
}

void
Mapping_syncDir(Mapping *const me, RKH_EVT_T *pe)
{
    ffile_sync();
    me->nStoreLastSync = 0;
}

/* ............................. Entry actions ............................. */
void
Collector_enActive(Collector *const me)
{
    RKH_SET_STATIC_EVENT(&me->updateStatusTmr, evTimeout);
    RKH_TMR_INIT(&me->updateStatusTmr.tmr,
                 RKH_UPCAST(RKH_EVT_T, &me->updateStatusTmr),
                 NULL);
    RKH_TMR_PERIODIC(&me->updateStatusTmr.tmr,
                     RKH_UPCAST(RKH_SMA_T, me),
					 UPDATING_STATUS_TIME, UPDATING_STATUS_TIME);
}

void
Collector_initAndTestDevNull(Collector *const me)
{
    int result;

    me->dev = (Device *)0;
    result = testDevNullJobCond(me);
    propagateMappingEvent(me, result);
}

void
Mapping_enStopped(Mapping *const me)
{
    RKH_TNT_T nTicks;

    nTicks = getMapTimeOnStopped();
    RKH_SET_STATIC_EVENT(&me->syncStoppedTmr, evToutSyncStopped);
    RKH_TMR_INIT(&me->syncStoppedTmr.tmr,
                 RKH_UPCAST(RKH_EVT_T, &me->syncStoppedTmr), NULL);
    RKH_TMR_PERIODIC(&me->syncStoppedTmr.tmr,
                     RKH_UPCAST(RKH_SMA_T, me->itsCollector),
                     nTicks, nTicks);
    setMappingStatus(me, MAPPING_STOP);
}

void
Mapping_enRunning(Mapping *const me)
{
    RKH_TNT_T nTicks;

    nTicks = getMapTimeOnRunning();
    RKH_SET_STATIC_EVENT(&me->syncRunningTmr.evt, evToutSyncRunning);
    RKH_TMR_INIT(&me->syncRunningTmr.tmr,
                 RKH_UPCAST(RKH_EVT_T, &me->syncRunningTmr), NULL);
    RKH_TMR_PERIODIC(&me->syncRunningTmr.tmr,
                     RKH_UPCAST(RKH_SMA_T, me->itsCollector),
                     nTicks, nTicks);
    setMappingStatus(me, MAPPING_RUNNING);
}

/* ............................. Exit actions .............................. */
void
Collector_exActive(Collector *const me)
{
    rkh_tmr_stop(&me->updateStatusTmr.tmr);
}

void
Mapping_exStopped(Mapping *const me)
{
    rkh_tmr_stop(&me->syncStoppedTmr.tmr);
}

void
Mapping_exRunning(Mapping *const me)
{
    rkh_tmr_stop(&me->syncRunningTmr.tmr);
}

/* ................................ Guards ................................. */
rbool_t
Mapping_isSyncDirOnStopped(const RKH_SM_T *me, RKH_EVT_T *pe)
{
    (void)pe;

    return (RKH_UPCAST(Mapping, me)->nStoreLastSync >= 
            Config_getMaxNumStoreOnStopped()) ? RKH_TRUE : RKH_FALSE;
}

rbool_t
Mapping_isSyncDirOnRunning(const RKH_SM_T *me, RKH_EVT_T *pe)
{
    (void)pe;

    return (RKH_UPCAST(Mapping, me)->nStoreLastSync >= 
            Config_getMaxNumStoreOnRunning()) ? RKH_TRUE : RKH_FALSE;
}

/* ------------------------------ End of file ------------------------------ */
