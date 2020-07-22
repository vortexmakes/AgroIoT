/**
 *  \file       priorities.h
 *  \brief      AgroIoT Active object's priorities.
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __PRIORITIES_H__
#define __PRIORITIES_H__

/* ----------------------------- Include files ----------------------------- */
/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ........................ Active object priorities ....................... */
typedef enum Priorities Priorities;
enum Priorities
{
    PowerMgrPrio,
    CollectorPrio,
    MappingPrio,
    ModMgrPrio,
    GsmMgrPrio,
    SocketPrio,
    SMSPrio,
    GeoMgrPrio,
    DeviceMgrPrio,
    CommMgrPrio,
    UsbMgrPrio
};

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
