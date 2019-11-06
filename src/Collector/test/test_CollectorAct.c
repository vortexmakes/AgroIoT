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
#include "Mock_rkhsma.h"
#include "Mock_rkhsm.h"
#include "rkhfwk_cast.h"
#include "signals.h"
#include "Mock_topic.h"
#include "Mock_rkhtrc_record.h"
#include "Mock_rkhtrc_filter.h"
#include "events.h"
#include "Mock_rkhtmr.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
RKHROM RKH_SCMP_T Mapping_Active;
#if 0
RKHROM RKH_SBSC_T SMInactive, WaitSyncSeq, Seq0, Seq2, Seq3, Seq4, Seq5, 
                  Seq11, OutOfSeq, Seq1, Seq10, Seq8, Seq7, Seq6, Seq9;
RKHROM RKH_SCMP_T SMActive;
RKHROM RKH_FINAL_T Collector_Final;
#endif

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
RKH_SMA_CREATE(Collector, collector, 0, HCAL, NULL, Collector_init, NULL);
RKH_SMA_DEF_PTR(collector);
RKH_SM_CONST_CREATE(mapping, 0, HCAL, &Mapping_Active, NULL, NULL);

static Collector *me;
static RKH_EVT_T evtObj, *evt;
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
    evt = &evtObj;
}

void
tearDown(void)
{
}

void
test_Constructor(void)
{
    rkh_sma_ctor_Expect(RKH_UPCAST(RKH_SMA_T, me), &me->vtbl);

    Collector_ctor();
    TEST_ASSERT_NOT_NULL(me->vtbl.task);
    TEST_ASSERT_NOT_NULL(me->itsMapping.itsCollector);
}

void
test_Init(void)
{
    rkh_trc_ao_Ignore();
    rkh_trc_ao_Ignore();
    rkh_trc_obj_Ignore();
    rkh_trc_symFil_Ignore();
    topic_subscribe_Expect(status, RKH_UPCAST(RKH_SMA_T, me));
    rkh_sm_init_Expect(RKH_UPCAST(RKH_SM_T, &me->itsMapping));

    Collector_init(me, evt);
}

void
test_UpdatePosition(void)
{
    GeoEvt event;

    Collector_updatePosition(me, RKH_UPCAST(RKH_EVT_T, &event));
    TEST_ASSERT_EQUAL_MEMORY(&me->status.position, &event.gps, sizeof(Geo));
}

void
test_UpdateDigOut(void)
{
    DigOutChangedEvt event;

    event.dout = 99;
    Collector_updateDigOut(me, RKH_UPCAST(RKH_EVT_T, &event));
    TEST_ASSERT_EQUAL(event.dout, me->status.ioStatus.digOut);
}

void
test_PubTmr(void)
{
    rkh_tmr_init__Expect(&me->updateStatusTmr.tmr, 
                         RKH_UPCAST(RKH_EVT_T, &me->updateStatusTmr));
    rkh_tmr_start_Expect(&me->updateStatusTmr.tmr, 
                         RKH_UPCAST(RKH_SMA_T, me), 
                         UPDATING_STATUS_TIME, 0);

    Collector_enActive(me);

    rkh_tmr_stop_ExpectAndReturn(&me->updateStatusTmr.tmr, 0);
    Collector_exActive(me);
}

/* ------------------------------ End of file ------------------------------ */
