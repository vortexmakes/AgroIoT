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
 *  DaBa  Dario Baliña db@vortexmakes.com
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdio.h>
#include <string.h>
#include "rkh.h"
#include "rkhfwk_pubsub.h"
#include "rkhtmr.h"
#include "mytypes.h"
#include "bsp.h"
#include "signals.h"
#include "events.h"
#include "topic.h"
#include "DeviceMgr.h"
#include "ps.h"
#include "tplink.h"
#include "tplhal.h"

/* ----------------------------- Local macros ------------------------------ */
/* ......................... Declares active object ........................ */
typedef struct DeviceMgr DeviceMgr;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE DeviceMgr_Inactive, DeviceMgr_InCycle, DeviceMgr_Idle;
RKH_DCLR_COMP_STATE DeviceMgr_Active;

/* ........................ Declares initial action ........................ */
static void init(DeviceMgr *const me, RKH_EVT_T *pe);

/* ........................ Declares effect actions ........................ */
static void startPs(DeviceMgr *const me, RKH_EVT_T *pe);
static void stopPs(DeviceMgr *const me, RKH_EVT_T *pe);
static void restartPs(DeviceMgr *const me, RKH_EVT_T *pe);

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
RKH_TRREG(evEndOfCycle, NULL, NULL, &DeviceMgr_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(DeviceMgr_Idle, nIdle, xIdle, &DeviceMgr_Active, NULL);
RKH_CREATE_TRANS_TABLE(DeviceMgr_Idle)
RKH_TRREG(evTimeout, NULL, restartPs, &DeviceMgr_InCycle),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct DeviceMgr
{
    RKH_SMA_T ao;       /* Base structure */
    RKH_TMR_T timer;
};

RKH_SMA_CREATE(DeviceMgr, deviceMgr, 3, HCAL, &DeviceMgr_Inactive, init, NULL);
RKH_SMA_DEF_PTR(deviceMgr);

/* ------------------------------- Constants ------------------------------- */
static const PS_PLBUFF_T reqs[] =
{
    {1, {CBOX_NULL}},
    {1, {CBOX_READ_ALL}}
};

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_STATIC_EVENT(endOfCycle, evEndOfCycle);
static RKH_STATIC_EVENT(evTimeoutObj, evTimeout);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
assembleShort(uchar *buf, short data)
{
    data = (*buf << 8) | *(buf + 1);
    buf += 2;
}

/* ............................ Initial action ............................. */
static void
init(DeviceMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_TIMER(&me->timer);
    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &DeviceMgr_Inactive);
    RKH_TR_FWK_STATE(me, &DeviceMgr_Active);
    RKH_TR_FWK_STATE(me, &DeviceMgr_Idle);
    RKH_TR_FWK_STATE(me, &DeviceMgr_InCycle);

    RKH_TMR_INIT(&me->timer, &evTimeoutObj, NULL);
    ps_init();
}

/* ............................ Effect actions ............................. */
static void
startPs(DeviceMgr *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

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

/* ............................. Entry actions ............................. */
static void
nIdle(DeviceMgr *const me)
{
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), DEVICE_CYCLE_TIME);
}

/* ............................. Exit actions .............................. */
static void
xIdle(DeviceMgr *const me)
{
    rkh_tmr_stop(&me->timer);
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
}

void
ps_onEndCycle(void)
{
    topic_publish(status, &endOfCycle, &deviceMgr);
}

void
ps_onStationRecv(ST_T station, PS_PLBUFF_T *pb)
{
    static CBOX_STR cbox;
    uchar *p;

    switch (station)
    {
        case ADDR_NORIA:
            p = (uchar *)pb->payload;
            cbox.cmd = *p++;
            cbox.m = *p++;
            assembleShort(p, cbox.h.hoard);
            assembleShort(p, cbox.h.pqty);
            assembleShort(p, cbox.h.flow);
            assembleShort(p, cbox.a.x);
            assembleShort(p, cbox.a.y);
            assembleShort(p, cbox.a.z);
            cbox.a.m = *p++;
            cbox.hum = *p++;
            break;

        case ADDR_CAUDALIMETRO:
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
