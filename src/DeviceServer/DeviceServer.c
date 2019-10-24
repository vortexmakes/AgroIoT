/**
 *  \file       DeviceServer.c
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
#include "DeviceServer.h"
#include "ps.h"

/* ----------------------------- Local macros ------------------------------ */
/* ......................... Declares active object ........................ */
typedef struct DeviceServer DeviceServer;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE DevSvr_Inactive, DevSvr_inCycle, DevSvr_Idle; 
RKH_DCLR_COMP_STATE DevSvr_Active; 
                    
/* ........................ Declares initial action ........................ */
static void init(DeviceServer *const me, RKH_EVT_T *pe);

/* ........................ Declares effect actions ........................ */
static void startPs(DeviceServer *const me, RKH_EVT_T *pe);
static void stopPs(DeviceServer *const me, RKH_EVT_T *pe);
static void restartPs(DeviceServer *const me, RKH_EVT_T *pe);
static void publishData(DeviceServer *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void idle_entry(DeviceServer *const me);

/* ......................... Declares exit actions ......................... */
static void idle_exit(DeviceServer *const me);

/* ............................ Declares guards ............................ */

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(DevSvr_Inactive, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(DevSvr_Inactive)
    RKH_TRREG(evOpen, NULL, startPs, &DevSvr_Active),
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(DevSvr_Active, NULL, NULL, RKH_ROOT, 
                             &DevSvr_inCycle, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(DevSvr_Active)
    RKH_TRREG(evClose, NULL, stopPs, &DevSvr_Inactive),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(DevSvr_inCycle, NULL, NULL, &DevSvr_Active, NULL);
RKH_CREATE_TRANS_TABLE(DevSvr_inCycle)
    RKH_TRREG(evEndOfCycle, NULL, publishData, &DevSvr_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(DevSvr_Idle, idle_entry, idle_exit, &DevSvr_Active, NULL);
RKH_CREATE_TRANS_TABLE(DevSvr_Idle)
    RKH_TRREG(evTimeout,     NULL, restartPs, &DevSvr_inCycle),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct DeviceServer
{
    RKH_SMA_T ao;       /* Base structure */
    RKH_TMR_T timer;    
};

RKH_SMA_CREATE(DeviceServer, deviceServer, 3, HCAL, 
                             &DevSvr_Inactive, init, NULL);
RKH_SMA_DEF_PTR(deviceServer);

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
init(DeviceServer *const me, RKH_EVT_T *pe)
{
	(void)pe;

    topic_subscribe(status, me);

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_TIMER(&me->timer);

    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &DevSvr_Inactive);
    RKH_TR_FWK_STATE(me, &DevSvr_Active);
    RKH_TR_FWK_STATE(me, &DevSvr_Idle);
    RKH_TR_FWK_STATE(me, &DevSvr_inCycle);

    RKH_TMR_INIT(&me->timer, &e_tout, NULL);

    RKH_SET_STATIC_EVENT(&sensorData, evSensorData);
    ps_init();
}

/* ............................ Effect actions ............................. */
static void
startPs(DeviceServer *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    ps_start();
}

static void
restartPs(DeviceServer *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

	ps_restart();
}

static void
stopPs(DeviceServer *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;
}

static void
publishData(DeviceServer *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    sensorData.cbox = *cbox_getInstance();

    topic_publish(status, &sensorData, me);
}

/* ............................. Entry actions ............................. */
static void
idle_entry(DeviceServer *const me)
{
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), DEVICE_CYCLE_TIME);
}

/* ............................. Exit actions .............................. */
static void
idle_exit(DeviceServer *const me)
{
    rkh_tmr_stop(&me->timer);
}

/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
