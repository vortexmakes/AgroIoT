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
static void ReadyToPowerMgrFinalExt1(PowerMgr *const me, RKH_EVT_T *pe);
static void ReadyToReadyLoc0(PowerMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */

/* ......................... Declares exit actions ......................... */

/* ............................ Declares guards ............................ */

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(PowerMgr_Ready, NULL, NULL, RKH_ROOT, NULL);


RKH_CREATE_TRANS_TABLE(PowerMgr_Ready)
	RKH_TRREG(evPowerFail, NULL, ReadyToPowerMgrFinalExt1, &PowerMgrFinal),
	RKH_TRINT(evGStatus, NULL, ReadyToReadyLoc0),
RKH_END_TRANS_TABLE


RKH_CREATE_FINAL_STATE(PowerMgrFinal, RKH_NULL);

/* ............................. Active object ............................. */
struct PowerMgr
{
    RKH_SMA_T sma;      /* base structure */
};

RKH_SMA_CREATE(PowerMgr, powerMgr, 0, HCAL, &PowerMgr_Ready, NULL, NULL);
RKH_SMA_DEF_PTR(powerMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Effect actions ............................. */
static void 
ReadyToPowerMgrFinalExt1(PowerMgr *const me, RKH_EVT_T *pe)
{
	storeStatus();
	ffile_sync();
	trace_msd_close();
	BatChr_shutDown();
}

static void 
ReadyToReadyLoc0(PowerMgr *const me, RKH_EVT_T *pe)
{
	updateStatus();
}

/* ............................. Entry actions ............................. */
/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
