/**
 *  \file       Collector.c
 *  \brief      Active object's action implementations.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkhsma.h"
#include "rkhfwk_cast.h"
#include "rkhtrc.h"
#include "rkhtrc_define.h"
#include "rkhtrc_filter.h"
#include "rkhtrc_record.h"
#include "rkhassert.h"
#include "signals.h"
#include "Collector.h"
#include "CollectorAct.h"
#include "bsp.h"
#include "CollectorActRequired.h"

RKH_MODULE_NAME(CollectorAct)

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void 
dispatch(RKH_SMA_T *me, void *arg)
{
}

/* ............................ Effect actions ............................. */
/* ............................. Entry actions ............................. */
/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
void 
Collector_ctor(void)
{
    Collector *me;

    me = RKH_DOWNCAST(Collector, collector);
    me->vtbl = rkhSmaVtbl;
    me->vtbl.task = dispatch;
    rkh_sma_ctor(RKH_UPCAST(RKH_SMA_T, me), &me->vtbl);
}

/* ------------------------------ End of file ------------------------------ */
