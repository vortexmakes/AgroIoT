/**
 *  \file       DeviceMgr.c
 *  \brief      TpSensor Externals Devices Server
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.06.05  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina db@vortexmakes.com
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdio.h>
#include <string.h>
#include "rkh.h"
#include "rkhfwk_pubsub.h"
#include "rkhtmr.h"
#include "bsp.h"
#include "priorities.h"
#include "signals.h"
#include "events.h"
#include "topic.h"
#include "DeviceMgr.h"
#include "device.h"
#include "ps.h"
#include "ps_callback.h"
#include "tplink.h"
#include "tplhal.h"
#include "Config.h"
#include "Flowmeter.h"

/* ----------------------------- Local macros ------------------------------ */
/* ......................... Declares active object ........................ */
/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE DeviceMgr_Inactive, DeviceMgr_InCycle, DeviceMgr_Idle;
RKH_DCLR_COMP_STATE DeviceMgr_Active;

/* ........................ Declares initial action ........................ */
static void init(DeviceMgr *const me, RKH_EVT_T *pe);

/* ........................ Declares effect actions ........................ */
static void startPs(DeviceMgr *const me, RKH_EVT_T *pe);
static void stopPs(DeviceMgr *const me, RKH_EVT_T *pe);
static void restartPs(DeviceMgr *const me, RKH_EVT_T *pe);
static void setPollCycleTime(DeviceMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void nIdle(DeviceMgr *const me);

/* ......................... Declares exit actions ......................... */
static void xIdle(DeviceMgr *const me);

/* ............................ Declares guards ............................ */
/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(DeviceMgr_Inactive, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(DeviceMgr_Inactive)
RKH_TRREG(evOpen, NULL, startPs, &DeviceMgr_Active),
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(DeviceMgr_Active, NULL, NULL, RKH_ROOT,
                             &DeviceMgr_InCycle, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(DeviceMgr_Active)
RKH_TRREG(evClose, NULL, stopPs, &DeviceMgr_Inactive),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(DeviceMgr_InCycle, NULL, NULL, &DeviceMgr_Active, NULL);
RKH_CREATE_TRANS_TABLE(DeviceMgr_InCycle)
RKH_TRREG(evEndOfCycle, NULL, setPollCycleTime, &DeviceMgr_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(DeviceMgr_Idle, nIdle, xIdle, &DeviceMgr_Active, NULL);
RKH_CREATE_TRANS_TABLE(DeviceMgr_Idle)
RKH_TRREG(evTimeout, NULL, restartPs, &DeviceMgr_InCycle),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
RKH_SMA_CREATE(DeviceMgr, 
               deviceMgr, 
               DeviceMgrPrio, 
               HCAL, 
               &DeviceMgr_Inactive, 
               init, 
               NULL);
RKH_SMA_DEF_PTR_TYPE(DeviceMgr, deviceMgr);

/* ------------------------------- Constants ------------------------------- */
static const PS_PLBUFF_T reqs[] =
{
    {1, {CBOX_NULL}},
    {1, {CBOX_READ_ALL}}
};

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
extern Device *sprayer, *sampler, *harvest;

/* ---------------------------- Local variables ---------------------------- */
static RKH_ROM_STATIC_EVENT(evEndOfCycleObj, evEndOfCycle);
static RKH_ROM_STATIC_EVENT(evNoDevObj, evNoDev);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static uchar *
assembleShort(uchar *buf, short *data)
{
    *data = (*buf << 8) | *(buf + 1);
    buf += 2;
    return buf;
}

static Device *
getDevice(DevId devId)
{
    Device *dev;

    switch (devId)
    {
        case SPRAYER:
            dev = (Device *)sprayer;
            break;
        case SAMPLER:
            dev = (Device *)sampler;
            break;
        case HARVEST:
            dev = (Device *)harvest;
            break;
        default:
            dev = (Device *)0;
            break;
    }
    return dev;
}

static DevId
mapCboxDevToDevId(unsigned short cboxDev)
{
    DevId devId;

    switch (cboxDev)
    {
        case EQTYPE_SPRAYER:
            devId = SPRAYER;
            break;
        case EQTYPE_SAMPLER:
            devId = SAMPLER;
            break;
        case EQTYPE_HARVEST_WO_ACC:
            devId = HARVEST;
            break;
        default:
            devId = DEVNULL;
            break;
    }
    return devId;
}

/* ............................ Initial action ............................. */
static void
init(DeviceMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_TIMER(&me->tmr.tmr);
    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &DeviceMgr_Inactive);
    RKH_TR_FWK_STATE(me, &DeviceMgr_Active);
    RKH_TR_FWK_STATE(me, &DeviceMgr_Idle);
    RKH_TR_FWK_STATE(me, &DeviceMgr_InCycle);

    RKH_SET_STATIC_EVENT(&me->tmr.evt, evTimeout);
    RKH_TMR_INIT(&me->tmr.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmr), NULL);
    ps_init();
}

/* ............................ Effect actions ............................. */
static void
startPs(DeviceMgr *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    deviceMgr->tries = 0;
    deviceMgr->backoff = 0;
    deviceMgr->pollCycle = Config_getDevPollCycleTime();
    ps_start();
}

static void
restartPs(DeviceMgr *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    ps_restart();
}

static void
stopPs(DeviceMgr *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;
}

static void
setPollCycleTime(DeviceMgr *const me, RKH_EVT_T *pe)
{
    if (me->tries == 0)
    {
        me->backoff = 0;
        me->pollCycle = Config_getDevPollCycleTime();
    }
    else if (me->tries >= DEV_MAX_NUM_TRIES)
    {
        if (me->backoff < DEV_MAX_NUM_BACKOFF)
        {
            ++me->backoff;
            me->pollCycle <<= 1;
        }
        me->tries = 0;
    }
}

/* ............................. Entry actions ............................. */
static void
nIdle(DeviceMgr *const me)
{
    RKH_TMR_ONESHOT(&me->tmr.tmr,
                    RKH_UPCAST(RKH_SMA_T, me),
                    RKH_TIME_SEC(me->pollCycle));
}

/* ............................. Exit actions .............................. */
static void
xIdle(DeviceMgr *const me)
{
    rkh_tmr_stop(&me->tmr.tmr);
}

/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
/*
 * ps callback functions
 */
void
ps_onStartCycle(void)
{
}

void
ps_onStop(void)
{
    ++deviceMgr->tries;
    topic_publish(Status, RKH_UPCAST(RKH_EVT_T, &evNoDevObj), 
                          RKH_UPCAST(RKH_SMA_T, deviceMgr));
}

void
ps_onEndCycle(void)
{
    RKH_SMA_POST_FIFO(RKH_UPCAST(RKH_SMA_T, deviceMgr), 
                      &evEndOfCycleObj, 
                      deviceMgr);
}

void
ps_onStationRecv(ST_T station, PS_PLBUFF_T *pb)
{
    static CBOX_STR cbox;
    Device *dev;
    uchar *p;
    RKH_EVT_T *evt;
    FlowData flow1, flow2;
    DevId devId;

    switch (station)
    {
        case ADDR_NORIA:
            p = (uchar *)pb->payload;
            cbox.cmd = *p++;
            cbox.m = *p++;
            p = assembleShort(p, (short *)&cbox.h.hoard);
            p = assembleShort(p, (short *)&cbox.h.pqty);
            p = assembleShort(p, (short *)&cbox.h.flow);
            p = assembleShort(p, (short *)&cbox.a.x);
            p = assembleShort(p, (short *)&cbox.a.y);
            p = assembleShort(p, (short *)&cbox.a.z);
            cbox.a.m = *p++;
            cbox.hum = *p++;

            devId = mapCboxDevToDevId((unsigned short)cbox.a.x);
            dev = getDevice(devId);
            if (dev != (Device *)0)
            {
                evt = device_makeEvt(dev, &cbox);
                deviceMgr->tries = 0;
                topic_publish(Status, evt, RKH_UPCAST(RKH_SMA_T, deviceMgr));
            }
            break;
        case ADDR_CAUDALIMETRO:
            p = (uchar *)pb->payload;
            flow1.nPulses = *p++;
            flow1.dir = *p++;
            flow2.nPulses = *p++;
            flow2.dir = *p++;
            evt = Flowmeter_makeEvt(&flow1, &flow2);
            deviceMgr->tries = 0;
            topic_publish(Status, evt, RKH_UPCAST(RKH_SMA_T, deviceMgr));
            break;
        default:
            break;
    }
}

PS_PLBUFF_T *
ps_onStationReq(ST_T station)
{
    switch (station)
    {
        case ADDR_NORIA:
            return (PS_PLBUFF_T *)&reqs[CBOX_READ_ALL];
        case ADDR_CAUDALIMETRO:
        	return (PS_PLBUFF_T *)&reqs[CBOX_READ_ALL];
        default:
            return (PS_PLBUFF_T *)&reqs[CBOX_NULL];
    }
}

void
ps_onStationStop(ST_T station, uchar reason)
{
    (void)station;
    (void)reason;
}

void
ps_onStationRun(ST_T station)
{
    (void)station;
}

/*
 * tplink callback functions
 */
void
tplink_onrcv(TPLFRM_T *p)
{
    static PSM_PKT_T pskt;

    pskt.m.m = PS_SIG_RESPONSE;
    pskt.station = p->addr;
    memcpy(pskt.buff.payload, p->pload, p->qty);
    pskt.buff.qty = (uchar)p->qty;

    ps_rcv_packet(&pskt);
}

void
tplink_ontout(void)
{
    ps_timeout();
}

void
tplink_onchkerr(void)
{
    ps_timeout();
}

/* ------------------------------ End of file ------------------------------ */
