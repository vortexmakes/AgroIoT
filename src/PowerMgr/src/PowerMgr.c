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
#include "signals.h"
#include "PowerMgr.h"
#include "bsp.h"
#include "PowerMgrRequired.h"

/* ----------------------------- Local macros ------------------------------ */

/* ......................... Declares active object ........................ */
typedef struct PowerMgr PowerMgr;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE PowerMgr_Ready;
RKH_DCLR_FINAL_STATE PowerMgrFinal;

/* ........................ Declares effect actions ........................ */
static void ToReadyExt0(PowerMgr *const me, RKH_EVT_T *pe);
static void ReadyToPowerMgrFinalExt1(PowerMgr *const me, RKH_EVT_T *pe);
static void ReadyToReadyLoc0(PowerMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */

/* ......................... Declares exit actions ......................... */

/* ............................ Declares guards ............................ */
static rbool_t isCondReadyToPowerMgrFinal1(PowerMgr *const me, RKH_EVT_T *pe);

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(PowerMgr_Ready, NULL, NULL, RKH_ROOT, NULL);


RKH_CREATE_TRANS_TABLE(PowerMgr_Ready)
    RKH_TRREG(evBatChrStatus, isCondReadyToPowerMgrFinal1, ReadyToPowerMgrFinalExt1, &PowerMgrFinal),
	RKH_TRINT(evGStatus, NULL, ReadyToReadyLoc0),
RKH_END_TRANS_TABLE


RKH_CREATE_FINAL_STATE(PowerMgrFinal, RKH_NULL);

/* ............................. Active object ............................. */
struct PowerMgr
{
    RKH_SMA_T sma;      /* base structure */
    GStatus status;
};

RKH_SMA_CREATE(PowerMgr, powerMgr, 0, HCAL, &PowerMgr_Ready, ToReadyExt0, NULL);
RKH_SMA_DEF_PTR(powerMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
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
	RKH_TR_FWK_STATE(me, &PowerMgrFinal);
	RKH_TR_FWK_SIG(evGStatus);
	RKH_TR_FWK_SIG(evBatChrStatus);
	#if 0
		RKH_TR_FWK_OBJ_NAME(ToReadyExt0, "ToReadyExt0");
		RKH_TR_FWK_OBJ_NAME(ReadyToPowerMgrFinalExt1, "ReadyToPowerMgrFinalExt1");
		RKH_TR_FWK_OBJ_NAME(ReadyToReadyLoc0, "ReadyToReadyLoc0");
		RKH_TR_FWK_OBJ_NAME(isCondReadyToPowerMgrFinal1, "isCondReadyToPowerMgrFinal1");
	#endif
	
	init(me);
}

static void 
ReadyToPowerMgrFinalExt1(PowerMgr *const me, RKH_EVT_T *pe)
{
	storeStatus(me);
	ffile_sync();
	trace_msd_close();
	BatChr_shutDown();
}

static void 
ReadyToReadyLoc0(PowerMgr *const me, RKH_EVT_T *pe)
{
	updateStatus(me, pe);
}

/* ............................. Entry actions ............................. */
/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
static rbool_t
isCondReadyToPowerMgrFinal1(PowerMgr *const me, RKH_EVT_T *pe)
{
	return (isPowerFail(pe)) ? true : false;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
