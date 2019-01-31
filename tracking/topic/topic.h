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
/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ................................ Signals ................................ */
typedef enum Topics Topics;
enum Topics
{
	TopicConnection,
    TopicModURC,
	TopicGeo,
    TopicIoChg,
    TopicSensor
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
