/**
 *  \file       geoMgr.c
 *  \brief      Geolocalization Context Manager.
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
#include "topics.h"
#include "geoMgr.h"
#include "rmc.h"
#include "ubx.h"
#include "gps.h"

/* ----------------------------- Local macros ------------------------------ */
/* ......................... Declares active object ........................ */
typedef struct GeoMgr GeoMgr;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE GeoMgr_Configure, GeoMgr_Failure, GeoMgr_WaitTimeSync,
                     GeoMgr_Active, GeoMgr_Void; 
RKH_DCLR_COMP_STATE  GeoMgr_OnTimeSync; 
RKH_DCLR_CHOICE_STATE GeoMgr_ChoiceInit, GeoMgr_ChoiceTimeSync, 
                      GeoMgr_ChoiceActive;
                    
/* ........................ Declares initial action ........................ */
static void init(GeoMgr *const me, RKH_EVT_T *pe);

/* ........................ Declares effect actions ........................ */
static void configInc(GeoMgr *const me, RKH_EVT_T *pe);
static void startRMCParser(GeoMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void configInit(GeoMgr *const me);
static void configSend(GeoMgr *const me);
static void startWaitSync(GeoMgr *const me);
static void ontimeEntry(GeoMgr *const me);

/* ......................... Declares exit actions ......................... */
static void waitSyncExit(GeoMgr *const me);
static void ontimeExit(GeoMgr *const me);

/* ............................ Declares guards ............................ */
rbool_t chkConfigNotEnd(GeoMgr *const me, RKH_EVT_T *pe);
rbool_t checkRMCTime(GeoMgr *const me, RKH_EVT_T *pe);
rbool_t chkRMCActive(GeoMgr *const me, RKH_EVT_T *pe);


/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(GeoMgr_Configure, configSend, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(GeoMgr_Configure)
    RKH_TRREG(evTimeout, NULL, configInc, &GeoMgr_ChoiceInit),
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(GeoMgr_ChoiceInit);
RKH_CREATE_BRANCH_TABLE(GeoMgr_ChoiceInit)
    RKH_BRANCH(chkConfigNotEnd,  NULL,   &GeoMgr_Configure),
    RKH_BRANCH(ELSE,             startRMCParser,   &GeoMgr_WaitTimeSync),
RKH_END_BRANCH_TABLE

RKH_CREATE_BASIC_STATE(GeoMgr_WaitTimeSync, startWaitSync, 
                                            waitSyncExit, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(GeoMgr_WaitTimeSync)
    RKH_TRREG(evTimeout, NULL, NULL, &GeoMgr_Failure),
    RKH_TRREG(evRMC, NULL, NULL, &GeoMgr_ChoiceTimeSync),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(GeoMgr_Failure, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(GeoMgr_Failure)
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(GeoMgr_ChoiceTimeSync);
RKH_CREATE_BRANCH_TABLE(GeoMgr_ChoiceTimeSync)
    RKH_BRANCH(checkRMCTime,   NULL,            &GeoMgr_WaitTimeSync),
    RKH_BRANCH(ELSE,           NULL, &GeoMgr_OnTimeSync),
RKH_END_BRANCH_TABLE

RKH_CREATE_COMP_REGION_STATE(GeoMgr_OnTimeSync, ontimeEntry, ontimeExit,
                             RKH_ROOT, &GeoMgr_ChoiceActive, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(GeoMgr_OnTimeSync)
    RKH_TRREG(evTimeout, NULL, NULL, &GeoMgr_Failure),
    RKH_TRREG(evRMC, NULL, NULL, &GeoMgr_OnTimeSync),
RKH_END_TRANS_TABLE


RKH_CREATE_CHOICE_STATE(GeoMgr_ChoiceActive);
RKH_CREATE_BRANCH_TABLE(GeoMgr_ChoiceActive)
    RKH_BRANCH(chkRMCActive, NULL,   &GeoMgr_Active),
    RKH_BRANCH(ELSE,         NULL,   &GeoMgr_Void),
RKH_END_BRANCH_TABLE

RKH_CREATE_BASIC_STATE(GeoMgr_Active, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(GeoMgr_Active)
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(GeoMgr_Void, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(GeoMgr_Void)
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct GeoMgr
{
    RKH_SMA_T ao;       /* Base structure */
    RKH_TMR_T timer;    
    UBXCmd_t *pInitCmd;
};

RKH_SMA_CREATE(GeoMgr, geoMgr, 2, HCAL, &GeoMgr_Configure, init, NULL);
RKH_SMA_DEF_PTR(geoMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static const unsigned char gsaOff[] = UBX_GSA_OFF;
static const unsigned char vtgOff[] = UBX_VTG_OFF;
static const unsigned char gsvOff[] = UBX_GSV_OFF;
static const unsigned char gllOff[] = UBX_GLL_OFF;
static const unsigned char ggaOff[] = UBX_GGA_OFF;
static const unsigned char rmc2Hz[] = UBX_RMC_2HZ;

static UBXCmd_t InitCmds[] =
{
    { gsaOff, sizeof(gsaOff) },
    { vtgOff, sizeof(vtgOff) },
    { gsvOff, sizeof(gsvOff) },
    { gllOff, sizeof(gllOff) },
    { ggaOff, sizeof(ggaOff) },
    //{ rmc2Hz, sizeof(rmc2Hz) },
    { NULL, 0 }
};

static RKH_STATIC_EVENT(e_tout, evTimeout);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
static void
init(GeoMgr *const me, RKH_EVT_T *pe)
{
	(void)pe;

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_TIMER(&me->timer);

    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &GeoMgr_Configure);
    RKH_TR_FWK_STATE(me, &GeoMgr_ChoiceInit);
    RKH_TR_FWK_STATE(me, &GeoMgr_WaitTimeSync);
    RKH_TR_FWK_STATE(me, &GeoMgr_Failure);
    RKH_TR_FWK_STATE(me, &GeoMgr_ChoiceTimeSync);
    RKH_TR_FWK_STATE(me, &GeoMgr_OnTimeSync);
    RKH_TR_FWK_STATE(me, &GeoMgr_ChoiceActive);
    RKH_TR_FWK_STATE(me, &GeoMgr_Active);
    RKH_TR_FWK_STATE(me, &GeoMgr_Void);
	RKH_TR_FWK_SIG(evRMC);

    RKH_TMR_INIT(&me->timer, &e_tout, NULL);

    bsp_serial_open(GPS_PORT);

	configInit(me);
}

/* ............................ Effect actions ............................. */
static void
configInc(GeoMgr *const me, RKH_EVT_T *pe)
{
	(void)pe;
	
    ++me->pInitCmd;
}

static void
startRMCParser(GeoMgr *const me, RKH_EVT_T *pe)
{
	(void)me;
	(void)pe;
	
    bsp_gpsParserHandler_set((void *)gps_parserInit());
}

/* ............................. Entry actions ............................. */
static void
configInit(GeoMgr *const me)
{
    me->pInitCmd = InitCmds;
}

static void
configSend(GeoMgr *const me)
{
	ubx_sendCmd(me->pInitCmd->cmd, me->pInitCmd->size);
	RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), UBX_INTERCMD_TIME);
}

static void
startWaitSync(GeoMgr *const me)
{
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), RMC_PERIOD_TIMEOUT);
}

static void
ontimeEntry(GeoMgr *const me)
{
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), RMC_PERIOD_TIMEOUT);
}


/* ............................. Exit actions .............................. */
static void
waitSyncExit(GeoMgr *const me)
{
    rkh_tmr_stop(&me->timer);
}

static void
ontimeExit(GeoMgr *const me)
{
    rkh_tmr_stop(&me->timer);
}

/* ................................ Guards ................................. */
rbool_t
chkConfigNotEnd(GeoMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    
    return (me->pInitCmd->cmd != NULL) ? RKH_TRUE : RKH_FALSE;
}

rbool_t
checkRMCTime(GeoMgr *const me, RKH_EVT_T *pe)
{
	(void)me;
	(void)pe;
     
    return (rmc_time((RMC_t *)(pe)) < 0) ? RKH_TRUE : RKH_FALSE;
}

rbool_t
chkRMCActive(GeoMgr *const me, RKH_EVT_T *pe)
{
	(void)me;
	(void)pe;
    
    return (rmc_status((RMC_t *(pe))) == RMC_StatusActive) ? 
                                            RKH_TRUE : RKH_FALSE;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
