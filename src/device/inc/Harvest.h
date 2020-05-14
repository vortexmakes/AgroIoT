/**
 *  \file       Harvest.h
 *  \brief      Specifies the interface of Harvest concrete module
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __HARVEST_H__
#define __HARVEST_H__

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
typedef struct Harvest Harvest;
struct Harvest
{
    Device base;    /* Base class */
    uint16_t hoard; /* Device's attribute */
    uint16_t nPail; /* Device's attribute */
    uint16_t flow;  /* Device's attribute */
};

typedef struct EvtHarvestData EvtHarvestData;
struct EvtHarvestData
{
    EvtDevData base;    /* Base class */
    uint16_t hoard;     /* Device's attribute */
    uint16_t nPail;     /* Device's attribute */
    uint16_t flow;      /* Device's attribute */
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
Device *Harvest_ctor(uint16_t nPailMin);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
