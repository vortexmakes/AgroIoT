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
RKH_DCLR_BASIC_STATE DeviceMgr_Inactive, DeviceMgr_inCycle, DeviceMgr_Idle; 
RKH_DCLR_COMP_STATE DeviceMgr_Active; 
                    
/* ........................ Declares initial action ........................ */
static void init(DeviceMgr *const me, RKH_EVT_T *pe);

/* ........................ Declares effect actions ........................ */
static void startPs(DeviceMgr *const me, RKH_EVT_T *pe);
static void stopPs(DeviceMgr *const me, RKH_EVT_T *pe);
static void restartPs(DeviceMgr *const me, RKH_EVT_T *pe);
static void publishData(DeviceMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void idle_entry(DeviceMgr *const me);

/* ......................... Declares exit actions ......................... */
static void idle_exit(DeviceMgr *const me);

/* ............................ Declares guards ............................ */

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(DeviceMgr_Inactive, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(DeviceMgr_Inactive)
    RKH_TRREG(evOpen, NULL, startPs, &DeviceMgr_Active),
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(DeviceMgr_Active, NULL, NULL, RKH_ROOT, 
                             &DeviceMgr_inCycle, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(DeviceMgr_Active)
    RKH_TRREG(evClose, NULL, stopPs, &DeviceMgr_Inactive),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(DeviceMgr_inCycle, NULL, NULL, &DeviceMgr_Active, NULL);
RKH_CREATE_TRANS_TABLE(DeviceMgr_inCycle)
    RKH_TRREG(evEndOfCycle, NULL, publishData, &DeviceMgr_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(DeviceMgr_Idle, idle_entry, idle_exit, &DeviceMgr_Active, NULL);
RKH_CREATE_TRANS_TABLE(DeviceMgr_Idle)
    RKH_TRREG(evTimeout,     NULL, restartPs, &DeviceMgr_inCycle),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct DeviceMgr
{
    RKH_SMA_T ao;       /* Base structure */
    RKH_TMR_T timer;    
};

RKH_SMA_CREATE(DeviceMgr, deviceMgr, 3, HCAL, 
                             &DeviceMgr_Inactive, init, NULL);
RKH_SMA_DEF_PTR(deviceMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_STATIC_EVENT(e_tout, evTimeout);
static SensorData sensorData;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
static void
init(DeviceMgr *const me, RKH_EVT_T *pe)
{
	(void)pe;

    topic_subscribe(status, me);

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_TIMER(&me->timer);

    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &DeviceMgr_Inactive);
    RKH_TR_FWK_STATE(me, &DeviceMgr_Active);
    RKH_TR_FWK_STATE(me, &DeviceMgr_Idle);
    RKH_TR_FWK_STATE(me, &DeviceMgr_inCycle);

    RKH_TMR_INIT(&me->timer, &e_tout, NULL);

    RKH_SET_STATIC_EVENT(&sensorData, evSensorData);
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

static void
publishData(DeviceMgr *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    sensorData.cbox = *cbox_getInstance();

    topic_publish(status, &sensorData, me);
}

/* ............................. Entry actions ............................. */
static void
idle_entry(DeviceMgr *const me)
{
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), DEVICE_CYCLE_TIME);
}

/* ............................. Exit actions .............................. */
static void
idle_exit(DeviceMgr *const me)
{
    rkh_tmr_stop(&me->timer);
}

/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
