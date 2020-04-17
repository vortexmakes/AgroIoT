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
#include "GStatus.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
typedef enum TraceId TraceId;
enum TraceId
{
    TraceId_PowerUp = 1,
    TraceId_OdmFlw,         /*  2 */
    TraceId_GpsReset,       /*  3 */
    TraceId_ModUnsync,      /*  4 */
    TraceId_CmdReset,       /*  5 */
    TraceId_CmdFmt,         /*  6 */
    TraceId_QfullServer,    /*  7 */
    TraceId_SmsRcv,         /*  8 */
    TraceId_PowerFail,      /*  9 */
    TraceId_Restore,        /* 10 */
    TraceId_FileFormat,     /* 11 */
    TraceId_CorruptStatus,  /* 12 */

    TraceId_NumOfEvents
};

/* ------------------------------- Data types ------------------------------ */
typedef short TraceArg;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void Trace_init(void);
void Trace_send(TraceId id, TraceArg arg0, TraceArg arg1);
void Trace_put(TraceId id, TraceArg arg0, TraceArg arg1);
void Trace_generate(GStatusType *status, TraceId id, TraceArg arg0, 
                    TraceArg arg1);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
