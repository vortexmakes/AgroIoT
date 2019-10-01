/**
 *  \file       FsMgr.c
 *  \brief      Filesystem Manager active object implementation.
 */

/* -------------------------- Development history -------------------------- */
/*
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
#include "bsp.h"
#include "signals.h"
#include "events.h"
#include "topic.h"
#include "FsMgr.h"

/* ----------------------------- Local macros ------------------------------ */
/* ......................... Declares active object ........................ */
typedef struct FsMgr FsMgr;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE Off;
                    
/* ........................ Declares initial action ........................ */
static void init(FsMgr *const me, RKH_EVT_T *pe);

/* ........................ Declares effect actions ........................ */
/* ......................... Declares entry actions ........................ */
/* ......................... Declares exit actions ......................... */
/* ............................ Declares guards ............................ */
/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(Off, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(Off)
    RKH_TRINT(evDiskConnected, NULL, NULL),
    RKH_TRINT(evDiskDisconnected, NULL, NULL),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct FsMgr
{
    RKH_SMA_T ao;
};

RKH_SMA_CREATE(FsMgr, fsMgr, 5, HCAL, &Off, init, NULL);
RKH_SMA_DEF_PTR(fsMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
static void
init(FsMgr *const me, RKH_EVT_T *pe)
{
	(void)pe;

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &Off);
}

/* ............................ Effect actions ............................. */
/* ............................. Entry actions ............................. */
/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
