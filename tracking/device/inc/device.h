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
/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
typedef enum DevId DevId;
enum DevId
{
    SPRAYER, NUM_DEVS
};

/* ------------------------------- Data types ------------------------------ */
typedef struct JobCond JobCond;
typedef struct Device Device;
typedef int (*TestOper)(Device *const me);

struct JobCond
{
    TestOper test;
    Device *dev;
};

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
