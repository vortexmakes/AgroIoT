/**
 *  \file       test_topic.c
 *  \brief      Unit test for topic module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.30.10  LeFr  v1.0.00  ---
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "topic.h"
#include "Mock_rkhsma.h"
#include "Mock_rkhfwk_pubsub.h"
#include "Mock_rkhfwk_module.h"
#include "Mock_rkhassert.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_SMA_T aoObj, *ao;
static RKH_EVT_T evtObj, *evt;
static Topics topic;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void 
MockAssertCallback(const char* const file, int line, int cmock_num_calls)
{
    TEST_PASS();
}

/* ---------------------------- Global functions --------------------------- */
void 
setUp(void)
{
    evt = &evtObj;
    ao = &aoObj;
    topic = status;
}

void 
tearDown(void)
{
}

void
test_Publish(void)
{
    rkh_pubsub_publish_ExpectAndReturn(topic, evt, ao, 0);
    topic_publish(topic, evt, ao);
}

void
test_PublishOnAnInvalidTopic(void)
{
    topic = NumOfTopics;

    rkh_assert_Expect("topic", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    topic_publish(topic, evt, ao);
}

void
test_PublishInvalidEvent(void)
{
    evt = (RKH_EVT_T *)0;
    rkh_assert_Expect("topic", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    topic_publish(topic, evt, ao);
}

void
test_PublishInvalidActObj(void)
{
    ao = (RKH_SMA_T *)0;
    rkh_assert_Expect("topic", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    topic_publish(topic, evt, ao);
}

void
test_Subscribe(void)
{
    rkh_pubsub_subscribe_Expect(topic, ao);
    topic_subscribe(topic, ao);
}

/* ------------------------------ End of file ------------------------------ */
