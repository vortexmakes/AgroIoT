/**
 *  \file       Skeleton.h
 *  \brief      Specifies the interface of Skeleton concrete module
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __SKELETON_H__
#define __SKELETON_H__

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
typedef struct Skeleton Skeleton;
struct Skeleton
{
    Device base;
    int x;
};

typedef struct EvtSkeletonData EvtSkeletonData;
struct EvtSkeletonData
{
    EvtDevData base;
    int x;       /* Device's attributes */
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
Device *Skeleton_ctor(int x);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
