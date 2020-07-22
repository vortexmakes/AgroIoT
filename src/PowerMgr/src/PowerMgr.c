/**
 *  \file       PowerMgr.c
 *  \brief      Active object implementation.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "priorities.h"
#include "signals.h"
#include "rkhtmr.h"
#include "PowerMgr.h"
#include "Backup.h"
#include "bsp.h"
#include "genled.h"
#include "cubemx.h"
#include "PowerMgrRequired.h"


/* ----------------------------- Local macros ------------------------------ */
#define WaitTime0	RKH_TIME_SEC(2)
#define WaitTime1	RKH_TIME_SEC(10)

/* ......................... Declares active object ........................ */
typedef struct PowerMgr PowerMgr;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE PowerMgr_Ready, PowerMgr_ShuttingDown;
RKH_DCLR_FINAL_STATE PowerMgrFinal;

/* ........................ Declares effect actions ........................ */
static void ToReadyExt0(PowerMgr *const me, RKH_EVT_T *pe);
static void ShuttingDownToPowerMgrFinalExt2(PowerMgr *const me, RKH_EVT_T *pe);
static void ReadyToReadyLoc0(PowerMgr *const me, RKH_EVT_T *pe);
static void ReadyToReadyLoc1(PowerMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void enReady(PowerMgr *const me);
static void enShuttingDown(PowerMgr *const me);

/* ......................... Declares exit actions ......................... */
static void exReady(PowerMgr *const me);
static void exShuttingDown(PowerMgr *const me);

/* ............................ Declares guards ............................ */
static rbool_t isCondReadyToShuttingDown1(PowerMgr *const me, RKH_EVT_T *pe);

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(PowerMgr_Ready, enReady, exReady, RKH_ROOT, NULL);
RKH_CREATE_BASIC_STATE(PowerMgr_ShuttingDown, enShuttingDown, exShuttingDown, RKH_ROOT, NULL);


RKH_CREATE_TRANS_TABLE(PowerMgr_Ready)
	RKH_TRREG(evBatChrStatus, isCondReadyToShuttingDown1, NULL, &PowerMgr_ShuttingDown),
	RKH_TRINT(evGStatus, NULL, ReadyToReadyLoc0),
	RKH_TRINT(evTout1, NULL, ReadyToReadyLoc1),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(PowerMgr_ShuttingDown)
	RKH_TRREG(evTout0, NULL, ShuttingDownToPowerMgrFinalExt2, &PowerMgrFinal),
RKH_END_TRANS_TABLE


RKH_CREATE_FINAL_STATE(PowerMgrFinal, RKH_NULL);

/* ............................. Active object ............................. */
struct PowerMgr
{
    RKH_SMA_T sma;      /* base structure */
    RKHTmEvt tmEvtObj0;
    RKHTmEvt tmEvtObj1;
    GStatus status;
};

RKH_SMA_CREATE(PowerMgr, powerMgr, PowerMgrPrio, HCAL, &PowerMgr_Ready, ToReadyExt0, NULL);
RKH_SMA_DEF_PTR(powerMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static Backup backup;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
init(PowerMgr *const me)
{
	topic_subscribe(Status, RKH_UPCAST(RKH_SMA_T, me));
}

static void
storeStatus(PowerMgr *const me)
{
	me->status.data.batChrStatus = BatChr_getStatus();
	GStatus_setChecksum(&me->status);
	StatQue_put(&me->status);
}

static void
updateStatus(PowerMgr *const me, RKH_EVT_T *pe)
{
	GStatusEvt *realEvt;

	realEvt = RKH_DOWNCAST(GStatusEvt, pe);
	me->status.data = realEvt->status;
}

static void
tracePowerFail(void)
{
    Trace_put(TraceId_PowerFail, 0, 0);
	set_led(LED_POWER, SEQ_LSTAGE4);
}

static void
updateMemStatus(void)
{
    MInt flashCheck;

    flashCheck = flash_verify_device();
    if(flashCheck != FLASH_OK)
    {
    	switch(flashCheck)
    	{
    		case FLASH_UNKNOWN:
    			set_led(LED_POWER, SEQ_LSTAGE1);
    			break;
    		case FLASH_MUST_POWERCYCLE:
    			set_led(LED_POWER, SEQ_LSTAGE2);
    			break;
    		default:
    			set_led(LED_POWER, SEQ_LSTAGE3);
				break;
    	}
        return;
    }

    Backup_getInfo(&backup);

    if((backup.error == Backup_Ok) && (backup.nFiles > 0))
        set_led(LED_POWER, SEQ_LIT);
    else
        set_led(LED_POWER, SEQ_NO_LIT);
}

#if (_USB_PERIODIC_SYNC_== 1)
static void
usbSync(void)
{
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);
	Backup_sync();
	trace_msd_sync();
}
#else
static void
usbSync(void)
{
}
#endif


static rbool_t
isPowerFail(RKH_EVT_T *pe)
{
	BatChrEvt *p;

	p = RKH_DOWNCAST(BatChrEvt, pe);
	return (p->status == NOLINE_BATT) ? true : false;
}

/* ............................ Effect actions ............................. */
static void 
ToReadyExt0(PowerMgr *const me, RKH_EVT_T *pe)
{
		
	RKH_TR_FWK_AO(me);
	RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
	RKH_TR_FWK_STATE(me, &PowerMgr_Ready);
	RKH_TR_FWK_STATE(me, &PowerMgr_ShuttingDown);
	RKH_TR_FWK_STATE(me, &PowerMgrFinal);
	RKH_TR_FWK_SIG(evGStatus);
	RKH_TR_FWK_SIG(evBatChrStatus);
	RKH_TR_FWK_TIMER(&me->tmEvtObj0.tmr);
	RKH_TR_FWK_TIMER(&me->tmEvtObj1.tmr);
	#if 0
		RKH_TR_FWK_OBJ_NAME(ToReadyExt0, "ToReadyExt0");
		RKH_TR_FWK_OBJ_NAME(ShuttingDownToPowerMgrFinalExt2, "ShuttingDownToPowerMgrFinalExt2");
		RKH_TR_FWK_OBJ_NAME(ReadyToReadyLoc0, "ReadyToReadyLoc0");
        RKH_TR_FWK_OBJ_NAME(ReadyToReadyLoc1, "ReadyToReadyLoc1");
		RKH_TR_FWK_OBJ_NAME(enShuttingDown, "enShuttingDown");
		RKH_TR_FWK_OBJ_NAME(isCondReadyToShuttingDown1, "isCondReadyToShuttingDown1");
	#endif
	
	init(me);
}

static void 
ShuttingDownToPowerMgrFinalExt2(PowerMgr *const me, RKH_EVT_T *pe)
{
	BatChr_shutDown();
}

static void 
ReadyToReadyLoc0(PowerMgr *const me, RKH_EVT_T *pe)
{
    updateStatus(me, pe);
}

static void
ReadyToReadyLoc1(PowerMgr *const me, RKH_EVT_T *pe)
{
    updateMemStatus();
    usbSync();
}

/* ............................. Entry actions ............................. */
static void 
enReady(PowerMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj1, evTout1);
	RKH_TMR_INIT(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj1), NULL);
	RKH_TMR_PERIODIC(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime1, WaitTime1);
}

static void 
enShuttingDown(PowerMgr *const me)
{
    storeStatus(me);
    tracePowerFail();
    Backup_sync();
    RKH_TRC_FLUSH();
    trace_msd_close();
    RKH_SET_STATIC_EVENT(&me->tmEvtObj0, evTout0);
    RKH_TMR_INIT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_EVT_T, &me->tmEvtObj0), NULL);
    RKH_TMR_ONESHOT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime0);
}

/* ............................. Exit actions .............................. */
static void 
exReady(PowerMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj1.tmr);
}

static void 
exShuttingDown(PowerMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj0.tmr);
}

/* ................................ Guards ................................. */
static rbool_t
isCondReadyToShuttingDown1(PowerMgr *const me, RKH_EVT_T *pe)
{
	return (isPowerFail(pe)) ? true : false;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
