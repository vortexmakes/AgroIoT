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
static void enableUsbDevice(UsbMgr *const me, RKH_EVT_T *pe);
static void disableUsbDevice(UsbMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void nIdle(UsbMgr *const me);
static void nReady(UsbMgr *const me);

/* ......................... Declares exit actions ......................... */
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

RKH_CREATE_COMP_REGION_STATE(UsbMgr_Active, NULL, NULL, RKH_ROOT,
                             &UsbMgr_Idle, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(UsbMgr_Active)
RKH_TRREG(evClose, NULL, NULL, &UsbMgr_Inactive),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(UsbMgr_Idle, nIdle, xIdle, &UsbMgr_Active, NULL);
RKH_CREATE_TRANS_TABLE(UsbMgr_Idle)
RKH_TRREG(evTimeout, usbHostIsReady, enableUsbDevice, &UsbMgr_Ready),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(UsbMgr_Ready, nReady, xReady, &UsbMgr_Active, NULL);
RKH_CREATE_TRANS_TABLE(UsbMgr_Ready)
RKH_TRREG(evTimeout, usbHostIsNotReady, disableUsbDevice, &UsbMgr_Idle),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct UsbMgr
{
    RKH_SMA_T ao;       /* Base structure */
    RKHTmEvt tmr;
};

RKH_SMA_CREATE(UsbMgr, usbMgr, 7, HCAL, &UsbMgr_Inactive, init, NULL);
RKH_SMA_DEF_PTR(usbMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
static void
init(UsbMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_TIMER(&me->tmr.tmr);
    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &UsbMgr_Inactive);
    RKH_TR_FWK_STATE(me, &UsbMgr_Active);
    RKH_TR_FWK_STATE(me, &UsbMgr_Idle);
    RKH_TR_FWK_STATE(me, &UsbMgr_Ready);

    RKH_SET_STATIC_EVENT(&me->tmr.evt, evTimeout);
    RKH_TMR_INIT(&me->tmr.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmr), NULL);
}

/* ............................ Effect actions ............................. */
static void
enableUsbDevice(UsbMgr *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    bsp_usbDeviceEnable();
}

static void
disableUsbDevice(UsbMgr *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    bsp_usbDeviceDisable();
}

/* ............................. Entry actions ............................. */
static void
nIdle(UsbMgr *const me)
{
    RKH_TMR_PERIODIC(&me->tmr.tmr,
                    RKH_UPCAST(RKH_SMA_T, me),
                    USBHOST_STATUS_TIME, USBHOST_STATUS_TIME);
}

static void
nReady(UsbMgr *const me)
{
    RKH_TMR_PERIODIC(&me->tmr.tmr,
                    RKH_UPCAST(RKH_SMA_T, me),
                    USBHOST_STATUS_TIME, USBHOST_STATUS_TIME);
}

/* ............................. Exit actions .............................. */
static void
xIdle(UsbMgr *const me)
{
    rkh_tmr_stop(&me->tmr.tmr);
}

static void
xReady(UsbMgr *const me)
{
    rkh_tmr_stop(&me->tmr.tmr);
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
