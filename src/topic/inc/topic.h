/**
 *  \file       topic.h
 *  \brief      Specifies the interface of topic module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.02  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina db@vortexmakes.com
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __TOPIC_H__
#define __TOPIC_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhsma.h"
#include "rkhevt.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
typedef enum Topics Topics;
enum Topics
{
    TCPConnection,
    ModURC,
    Status,
    NumOfTopics
};

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void topic_subscribe(Topics topic, RKH_SMA_T *ao);
void topic_publish(Topics topic, RKH_EVT_T *evt, RKH_SMA_T *ao);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
