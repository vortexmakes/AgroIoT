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
#include "cbdata.h"
#include "rkhevt.h"
#include "rkhsma.h"
#include "collector.h"

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
typedef struct DevVtbl DevVtbl;
typedef int (*TestOper)(Device *const me);
typedef RKH_EVT_T *(*MakeEvtOper)(Device *const me, CBOX_STR *rawData);
typedef void (*UpdateOper)(Device *const me, RKH_EVT_T *evt);
typedef void (*UpdateRawOper)(Device *const me);

struct JobCond
{
    RKH_SMA_T *collector;
};

struct DevVtbl
{
    TestOper test;
    MakeEvtOper makeEvt;
    UpdateOper update;
    UpdateRawOper updateRaw;
};

struct Device
{
    int id;
    JobCond *jobCond;
    DevVtbl *vptr;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void device_ctor(Device *const me, int id, RKH_SMA_T *collector, 
                 JobCond *jobCond, DevVtbl *vtbl);
RKH_EVT_T *device_makeEvt(Device *const me, CBOX_STR *rawData);
void device_update(Device *const me, RKH_EVT_T *evt);
int device_test(Device *const me);
void device_updateRaw(Device *const me);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
