/**
 *  \file       Trace.h
 *  \brief      Specification of application tracing module.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __TRACE_H__
#define __TRACE_H__

/* ----------------------------- Include files ----------------------------- */
#include <stddef.h>
#include <stdint.h>

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
typedef short TraceId;
typedef short TraceArg;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void Trace_init(void);
void Trace_send(TraceId id, TraceArg arg0, TraceArg arg1);
void Trace_put(TraceId id, TraceArg arg0, TraceArg arg1);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
