/**
 *  \file       test_CollectorAct.c
 *  \brief      Unit test for state machine module.
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
#include "unity.h"
#include "Collector.h"
#include "CollectorAct.h"
#include "signals.h"
#include "Mock_rkhfwk_cast.h"
#include "Mock_rkhtmr.h"
#include "Mock_rkhtrc_record.h"
#include "Mock_rkhsma.h"
#include "Mock_rkhassert.h"
#include "Mock_GStatus.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#if 0
RKHROM RKH_SBSC_T SMInactive, WaitSyncSeq, Seq0, Seq2, Seq3, Seq4, Seq5, 
                  Seq11, OutOfSeq, Seq1, Seq10, Seq8, Seq7, Seq6, Seq9;
RKHROM RKH_SCMP_T SMActive;
RKHROM RKH_FINAL_T Collector_Final;
#endif

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
RKH_SMA_CREATE(Collector, collector, 0, HCAL, NULL, NULL, NULL);
RKH_SMA_DEF_PTR_TYPE(Collector, collector);
static Collector *me;
const RKHSmaVtbl rkhSmaVtbl =  /* Instantiate it because rkhsma is mocked */
{
    rkh_sma_activate,
    rkh_sma_dispatch,
    rkh_sma_post_fifo,
    rkh_sma_post_lifo
};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
    me = RKH_DOWNCAST(Collector, collector);
}

void
tearDown(void)
{
}

void
test_Constructor(void)
{
    rkh_sma_ctor_Expect(RKH_UPCAST(RKH_SMA_T, collector), &collector->vtbl);
    Collector_ctor();
}

/* ------------------------------ End of file ------------------------------ */
