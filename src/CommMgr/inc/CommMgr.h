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

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE Idle, WaitSync, SendingStatus, ReceivingStatusAck, 
                     SendingEndOfHist, SendingHist, ReceivingMsgAck;
RKH_DCLR_COMP_STATE Active, Current, History;
RKH_DCLR_CHOICE_STATE C0, C1, C2, C3, C4;
RKH_DCLR_FINAL_STATE CurrentFinal, HistoryFinal;

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
