/**
 *  \file       Sampler.h
 *  \brief      Specifies the interface of Sampler concrete module
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __SAMPLER_H__
#define __SAMPLER_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhevt.h"
#include "device.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
typedef struct Sampler Sampler;
struct Sampler
{
    Device base;        /* Base class */
    int harness;
    int deep;
    bool isValid;
};

typedef struct EvtSamplerData EvtSamplerData;
struct EvtSamplerData
{
    EvtDevData base;    /* Base class */
    int harness;
    int deep;
    bool isValid;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
Device *Sampler_ctor(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
