/**
 *  \file       topic.c
 *  \brief      Implementation of topic module.
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
#include "topic.h"
#include "rkhsma.h"
#include "rkhevt.h"
#include "rkhfwk_pubsub.h"
#include "rkhfwk_module.h"
#include "rkhassert.h"

RKH_MODULE_NAME(topic)

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
topic_subscribe(Topics topic, RKH_SMA_T *ao)
{
    rkh_pubsub_subscribe(topic, ao);
}

void
topic_publish(Topics topic, RKH_EVT_T *evt, RKH_SMA_T *ao)
{
    RKH_REQUIRE((topic < NumOfTopics) &&
                (evt != (RKH_EVT_T *)0) &&
                (ao != (RKH_SMA_T *)0));
    rkh_pubsub_publish(topic, evt, ao);
}

/* ------------------------------ End of file ------------------------------ */
