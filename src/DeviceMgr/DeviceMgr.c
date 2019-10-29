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
#include "bsp.h"
#include "signals.h"
#include "events.h"
#include "topic.h"
#include "DeviceMgr.h"
#include "ps.h"

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
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_STATIC_EVENT(evTimeoutObj, evTimeout);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
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
/* ------------------------------ End of file ------------------------------ */
