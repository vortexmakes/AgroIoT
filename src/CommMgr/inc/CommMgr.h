/**
 *  \file       CommMgr.h
 *  \brief      Active object specification.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __COMMMGR_H__
#define __COMMMGR_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhsma.h"
#include "rkhtmr.h"
#include "GStatus.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ........................ Declares active object ......................... */
RKH_SMA_DCLR(commMgr);

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE CommMgr_Idle, CommMgr_WaitSync, CommMgr_SendingStatus, 
                     CommMgr_ReceivingAck, CommMgr_SendingEndOfHist, 
                     CommMgr_SendingHist, CommMgr_EndCycle;
RKH_DCLR_COMP_STATE CommMgr_Active;
RKH_DCLR_CHOICE_STATE CommMgr_C0, CommMgr_C1, CommMgr_C2, CommMgr_C3;

/* ------------------------------- Data types ------------------------------ */
/* ............................. Active object ............................. */
typedef struct CommMgr CommMgr;
struct CommMgr
{
    RKH_SMA_T sma;      /* base structure */
    RKHTmEvt tmEvtObj0;
    GStatus status;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
