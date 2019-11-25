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
#include "YFrame.h"
#include "events.h"
#include "settings.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ........................ Declares active object ......................... */
RKH_SMA_DCLR(commMgr);

/* ------------------------------- Data types ------------------------------ */
/* ............................. Active object ............................. */
typedef struct CommMgr CommMgr;
struct CommMgr
{
    RKH_SMA_T sma;      /* base structure */
    RKHTmEvt tmEvtObj0;
    GStatus status;
    rbool_t isPendingStatus;
    TypeOfResp lastRecvResponse;
    SendEvt evSendObj;
    ruint nFramesToSend;
    ruint framesToSend;
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
