/**
 *  \file       test_Trace.c
 *  \brief      Unit test for application tracing module
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
#include "Trace.h"
#include "rkhfwk_cast.h"
#include "Mock_signals.h"
#include "Mock_events.h"
#include "Mock_rkhsma.h"
#include "Mock_rkhfwk_dynevt.h"
#include "Mock_GStatus.h"
#include "Mock_geoMgr.h"
#include "Mock_Collector.h"
#include "Mock_rkhtrc_record.h"
#include "Mock_StatQue.h"
#include "Mock_ffile.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
RKH_SMA_CREATE(Collector, collector, 0, HCAL, NULL, NULL, NULL);
RKH_SMA_DEF_PTR(collector);
const RKHSmaVtbl rkhSmaVtbl =  /* Instantiate it because rkhsma is mocked */
{
    rkh_sma_activate,
    rkh_sma_dispatch,
    rkh_sma_post_fifo,
    rkh_sma_post_lifo
};
RKH_SMA_T *rkh_sptbl[RKH_CFG_FWK_MAX_SMA];  /* registered SMA table */

/* ---------------------------- Local variables ---------------------------- */
static GStatus traceGStatus = 
{
    {
        {
            "000000", {RMC_StatusInvalid},
            {GEO_INVALID_LATITUDE}, {GEO_INVALID_LATITUDE_IND},
            {GEO_INVALID_LONGITUDE}, {GEO_INVALID_LONGITUDE_IND},
            {GEO_INVALID_SPEED}, {GEO_INVALID_COURSE},
            "000000"
        },
        {0, 0, {0, 0, 0}, {0, 0, 0}, 0},
        {0, 0},
        0
    },
    0
};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static rInt
MockStatQuePutCallback(GStatus *elem, int cmock_num_calls)
{
    TEST_ASSERT_EQUAL_MEMORY(&traceGStatus.data.position, 
                             &elem->data.position, 
                             sizeof(Geo));
    TEST_ASSERT_EQUAL(0, elem->data.devData.cmd);
    TEST_ASSERT_EQUAL(0, elem->data.devData.m);
    TEST_ASSERT_EQUAL_MEMORY(&traceGStatus.data.devData.h, 
                             &elem->data.devData.h, 
                             sizeof(GRSENS_T));
    TEST_ASSERT_EQUAL(2, elem->data.devData.a.x);
    TEST_ASSERT_EQUAL_HEX32(4, elem->data.devData.a.y);
    TEST_ASSERT_EQUAL_HEX32(6, elem->data.devData.a.z);
    TEST_ASSERT_EQUAL(0, elem->data.devData.hum);
    TEST_ASSERT_EQUAL_MEMORY(&traceGStatus.data.ioStatus, 
                             &elem->data.ioStatus, 
                             sizeof(IOStatus));
    TEST_ASSERT_EQUAL_MEMORY(&traceGStatus.data.batChrStatus, 
                             &elem->data.batChrStatus, 
                             sizeof(BatChrStatus));
    return 0;
}

/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
    rkh_sptbl[0] = collector;
}

void
tearDown(void)
{
}

void
test_Init(void)
{
    rkh_trc_obj_Expect(RKH_TE_FWK_OBJ, 0, "trace");
    rkh_trc_obj_IgnoreArg_obj();

    Trace_init();
}

void
test_SendATraceEventToBeStored(void)
{
    TraceEvt traceEvtObj;
    TraceId id;
    TraceArg arg0, arg1;

    id = 2;
    arg0 = 0xdead;
    arg1 = 0xbeaf;
    rkh_fwk_ae_ExpectAndReturn(sizeof(TraceEvt), evTrace, 0,
                               (RKH_EVT_T *)&traceEvtObj);
    rkh_fwk_ae_IgnoreArg_sender();
    rkh_sma_post_fifo_Expect(collector, (RKH_EVT_T *)&traceEvtObj, 0);
    rkh_sma_post_fifo_IgnoreArg_sender();

    Trace_send(id, arg0, arg1);

    TEST_ASSERT_EQUAL(id, traceEvtObj.id);
    TEST_ASSERT_EQUAL(arg0, traceEvtObj.arg0);
    TEST_ASSERT_EQUAL(arg1, traceEvtObj.arg1);
}

void
test_PutATraceInMemory(void)
{
    TraceId id;
    TraceArg arg0, arg1;

    id = 2;
    arg0 = 4;
    arg1 = 6;
    GStatus_setChecksum_Expect(0);
    GStatus_setChecksum_IgnoreArg_me();
    StatQue_put_ExpectAndReturn(0, 0);
    StatQue_put_IgnoreArg_elem();
    StatQue_put_StubWithCallback(MockStatQuePutCallback);

    Trace_put(id, arg0, arg1);
}

/* ------------------------------ End of file ------------------------------ */
