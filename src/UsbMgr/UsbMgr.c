/**
 *  \file       UsbMgr.c
 *  \brief      USB Host device Manager
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.06.05  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina db@vortexmakes.com
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
#include "UsbMgr.h"
#include "Config.h"

/* ----------------------------- Local macros ------------------------------ */
/* ......................... Declares active object ........................ */
typedef struct UsbMgr UsbMgr;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE UsbMgr_Inactive, UsbMgr_Idle, UsbMgr_Ready;
RKH_DCLR_COMP_STATE UsbMgr_Active;

/* ........................ Declares initial action ........................ */
static void init(UsbMgr *const me, RKH_EVT_T *pe);

/* ........................ Declares effect actions ........................ */
static void usbHostProcess(UsbMgr *const me, RKH_EVT_T *pe);
static void enableUsbDevice(UsbMgr *const me, RKH_EVT_T *pe);
static void disableUsbDevice(UsbMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void nActive(UsbMgr *const me);
static void nIdle(UsbMgr *const me);
static void nReady(UsbMgr *const me);

/* ......................... Declares exit actions ......................... */
static void xActive(UsbMgr *const me);
static void xIdle(UsbMgr *const me);
static void xReady(UsbMgr *const me);

/* ............................ Declares guards ............................ */
rbool_t usbHostIsReady(const RKH_SM_T *me, RKH_EVT_T *pe);
rbool_t usbHostIsNotReady(const RKH_SM_T *me, RKH_EVT_T *pe);

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(UsbMgr_Inactive, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(UsbMgr_Inactive)
RKH_TRREG(evOpen, NULL, NULL, &UsbMgr_Active),
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(UsbMgr_Active, nActive, xActive, RKH_ROOT,
                             &UsbMgr_Idle, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(UsbMgr_Active)
RKH_TRINT(evTout0, NULL, usbHostProcess),
RKH_TRREG(evClose, NULL, NULL, &UsbMgr_Inactive),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(UsbMgr_Idle, nIdle, xIdle, &UsbMgr_Active, NULL);
RKH_CREATE_TRANS_TABLE(UsbMgr_Idle)
RKH_TRREG(evTout1, usbHostIsReady, enableUsbDevice, &UsbMgr_Ready),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(UsbMgr_Ready, nReady, xReady, &UsbMgr_Active, NULL);
RKH_CREATE_TRANS_TABLE(UsbMgr_Ready)
RKH_TRREG(evTout1, usbHostIsNotReady, disableUsbDevice, &UsbMgr_Idle),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct UsbMgr
{
    RKH_SMA_T ao;       /* Base structure */
    RKHTmEvt tmEvtObj0;
    RKHTmEvt tmEvtObj1;
};

RKH_SMA_CREATE(UsbMgr, usbMgr, 10, HCAL, &UsbMgr_Inactive, init, NULL);
RKH_SMA_DEF_PTR(usbMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_ROM_STATIC_EVENT(e_UsbMounted, evUsbMounted);
static RKH_ROM_STATIC_EVENT(e_UsbUnmounted, evUsbUnmounted);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
static void
init(UsbMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_TIMER(&me->tmEvtObj0.tmr);
    RKH_TR_FWK_TIMER(&me->tmEvtObj1.tmr);
    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &UsbMgr_Inactive);
    RKH_TR_FWK_STATE(me, &UsbMgr_Active);
    RKH_TR_FWK_STATE(me, &UsbMgr_Idle);
    RKH_TR_FWK_STATE(me, &UsbMgr_Ready);

    RKH_SET_STATIC_EVENT(&me->tmEvtObj0.evt, evTout0);
    RKH_TMR_INIT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj0), NULL);

    RKH_SET_STATIC_EVENT(&me->tmEvtObj1.evt, evTout1);
    RKH_TMR_INIT(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj1), NULL);
}

/* ............................ Effect actions ............................. */
static void
usbHostProcess(UsbMgr *const me, RKH_EVT_T *pe)
{
    bsp_usbHostProcess();
}

static void
enableUsbDevice(UsbMgr *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    bsp_usbDeviceEnable();
    topic_publish(Status, RKH_UPCAST(RKH_EVT_T, &e_UsbMounted),
                          RKH_UPCAST(RKH_SMA_T, me));
}

static void
disableUsbDevice(UsbMgr *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    bsp_usbDeviceDisable();
    topic_publish(Status, RKH_UPCAST(RKH_EVT_T, &e_UsbUnmounted),
                          RKH_UPCAST(RKH_SMA_T, me));
}

/* ............................. Entry actions ............................. */
static void
nActive(UsbMgr *const me)
{
/*
    RKH_TMR_PERIODIC(&me->tmEvtObj0.tmr,
                    RKH_UPCAST(RKH_SMA_T, me),
                    USBHOST_PROCESS_TIME, USBHOST_PROCESS_TIME);
*/
}

static void
nIdle(UsbMgr *const me)
{
    RKH_TMR_PERIODIC(&me->tmEvtObj1.tmr,
                    RKH_UPCAST(RKH_SMA_T, me),
                    USBHOST_STATUS_TIME, USBHOST_STATUS_TIME);
}

static void
nReady(UsbMgr *const me)
{
    RKH_TMR_PERIODIC(&me->tmEvtObj1.tmr,
                    RKH_UPCAST(RKH_SMA_T, me),
                    USBHOST_STATUS_TIME, USBHOST_STATUS_TIME);
}

/* ............................. Exit actions .............................. */
static void
xActive(UsbMgr *const me)
{
//    rkh_tmr_stop(&me->tmEvtObj0.tmr);
}

static void
xIdle(UsbMgr *const me)
{
    rkh_tmr_stop(&me->tmEvtObj1.tmr);
}

static void
xReady(UsbMgr *const me)
{
    rkh_tmr_stop(&me->tmEvtObj1.tmr);
}

/* ................................ Guards ................................. */
rbool_t
usbHostIsReady(const RKH_SM_T *me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    return (bsp_usbHostStatus() == UsbHostClassReady) ? RKH_TRUE : RKH_FALSE;
}

rbool_t
usbHostIsNotReady(const RKH_SM_T *me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    return (bsp_usbHostStatus() != UsbHostClassReady) ? RKH_TRUE : RKH_FALSE;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
