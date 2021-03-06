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
#include "cbox.h"
#include "rkhevt.h"
#include "rkhsma.h"
#include "Collector.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
typedef enum DevId DevId;
enum DevId
{
    SPRAYER, HARVEST, SAMPLER, DEVNULL, SKELETON, NUM_DEVS
};

/* ------------------------------- Data types ------------------------------ */
typedef struct JobCond JobCond;
typedef struct Device Device;
typedef struct DevVtbl DevVtbl;

/** Collector probes the device's job condition */
typedef int (*TestOper)(Device *const me);

/** DeviceServer makes an event from the device response */
typedef RKH_EVT_T *(*MakeEvtOper)(Device *const me, CBOX_STR *rawData);

/** 
 *  \brief
 *  Collector updates device's data from received event
 *
 *  \return
 *  If the device wants Collector stores the updated system's status, 
 *  the device will return true. Otherwise, it will return false.
 */
typedef bool (*UpdateOper)(Device *const me, RKH_EVT_T *evt);

/** Collector transforms device's data to CBOX_STR class */
typedef void (*UpdateRawOper)(Device *const me);

/** Collector clear device's data */
typedef void (*ClearOper)(Device *const me);

struct JobCond
{
    int id;
};

struct DevVtbl
{
    TestOper test;
    MakeEvtOper makeEvt;
    UpdateOper update;
    UpdateRawOper updateRaw;
    ClearOper clear;
};

struct Device
{
    int id;
    JobCond *jobCond;
    DevVtbl *vptr;
    RKH_SMA_T *collector;
};

typedef struct EvtDevData EvtDevData;
struct EvtDevData
{
    RKH_EVT_T base;
    Device *dev;        /* Points to a device's instance */
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void device_ctor(Device *const me, int id, RKH_SMA_T *collector,
                 JobCond *jobCond, DevVtbl *vtbl);
RKH_EVT_T *device_makeEvt(Device *const me, CBOX_STR *rawData);
bool device_update(Device *const me, RKH_EVT_T *evt);
int device_test(Device *const me);
void device_updateRaw(Device *const me);
void device_clear(Device *const me);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
