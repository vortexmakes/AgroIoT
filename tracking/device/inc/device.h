/**
 *  \file       device.h
 *  \brief      Specifies the interface of Device module
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.16.10  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __DEVICE_H__
#define __DEVICE_H__

/* ----------------------------- Include files ----------------------------- */
#include "jobcond.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
typedef struct Device Device;
struct Device
{
    int id;
    JobCond *jobCond;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void device_ctor(Device *const me, int id, JobCond *jobCond, 
                 TestOper testOper);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
