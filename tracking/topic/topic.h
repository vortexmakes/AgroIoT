/**
 *  \file       topic.h
 *  \brief      Event topics definitions.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.02  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Bali�a db@vortexmakes.com
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __TOPIC_H__
#define __TOPIC_H__

/* ----------------------------- Include files ----------------------------- */
/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
#define topic_subscribe(t, ao)           rkh_pubsub_subscribe(t, \
                                                RKH_UPCAST(RKH_SMA_T, (ao)))

#define topic_publish(t, e, ao)          rkh_pubsub_publish(t, \
                                                RKH_UPCAST(RKH_EVT_T, (e)), \
                                                RKH_UPCAST(RKH_SMA_T, (ao)))
/* -------------------------------- Constants ------------------------------ */
typedef enum Topics Topics;
enum Topics
{
    tcpConnection,
    gsmURC,
    deviceStatus,
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
