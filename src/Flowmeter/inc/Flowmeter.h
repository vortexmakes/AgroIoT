/**
 *  \file       Flowmeter.h
 *  \brief      Specifies the flowmeter device.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.17.06  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __FLOWMETER_H__
#define __FLOWMETER_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"
#include "rkhevt.h"
#include "GStatus.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
typedef enum FlowDir FlowDir;
enum FlowDir
{
    Forward, Reverse
};

/* ------------------------------- Data types ------------------------------ */
typedef struct FlowData FlowData;
struct FlowData
{
    uint8_t nPulses;
    FlowDir dir;
};

typedef struct Flowmeter Flowmeter;
struct Flowmeter
{
    FlowData flow1;
    FlowData flow2;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void Flowmeter_init(Flowmeter *me);
void Flowmeter_update(Flowmeter *me, RKH_EVT_T *event);
void Flowmeter_updateRaw(Flowmeter *me, CBOX_STR *status);
RKH_EVT_T *Flowmeter_makeEvt(FlowData *flow1, FlowData *flow2);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
