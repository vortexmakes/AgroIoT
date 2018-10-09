/**
 *  \file       topics.h
 *  \brief      Event topics definitions.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.02  DaBa  v1.0.00  Initial version
 *  2018.05.11  LeFr  v1.0.01
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Bali�a db@vortexmakes.com
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __TOPICS_H__
#define __TOPICS_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
#define tpConnection_subscribe(me)   rkh_pubsub_subscribe(tpConnection, \
                                                RKH_UPCAST(RKH_SMA_T, (me)))

#define tpConnection_publish(ev, me) rkh_pubsub_publish(tpConnection, \
                                                RKH_UPCAST(RKH_EVT_T, (ev)), \
                                                RKH_UPCAST(RKH_SMA_T, (me)))

#define tpModURC_subscribe(me)       rkh_pubsub_subscribe(tpModURC, \
                                                RKH_UPCAST(RKH_SMA_T, (me)))

#define tpModURC_publish(ev, me)     rkh_pubsub_publish(tpModURC, \
                                                RKH_UPCAST(RKH_EVT_T, (ev)), \
                                                RKH_UPCAST(RKH_SMA_T, (me)))

#define tpGeo_subscribe(me)          rkh_pubsub_subscribe(tpGeo, \
                                                RKH_UPCAST(RKH_SMA_T, (me)))

#define tpGeo_publish(ev, me)        rkh_pubsub_publish(tpGeo, \
                                                RKH_UPCAST(RKH_EVT_T, (ev)), \
                                                RKH_UPCAST(RKH_SMA_T, (me)))

#define tpIoChg_subscribe(me)          rkh_pubsub_subscribe(tpIoChg, \
                                                RKH_UPCAST(RKH_SMA_T, (me)))

#define tpIoChg_publish(ev, me)        rkh_pubsub_publish(tpIoChg, \
                                                RKH_UPCAST(RKH_EVT_T, (ev)), \
                                                RKH_UPCAST(RKH_SMA_T, (me)))

#define tpSensor_subscribe(me)          rkh_pubsub_subscribe(tpSensor, \
                                                RKH_UPCAST(RKH_SMA_T, (me)))

#define tpSensor_publish(ev, me)        rkh_pubsub_publish(tpSensor, \
                                                RKH_UPCAST(RKH_EVT_T, (ev)), \
                                                RKH_UPCAST(RKH_SMA_T, (me)))

/* -------------------------------- Constants ------------------------------ */
/* ................................ Signals ................................ */
typedef enum Topics Topics;
enum Topics
{
	tpConnection,
    tpModURC,
	tpGeo,
    tpIoChg,
    tpSensor
};

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
