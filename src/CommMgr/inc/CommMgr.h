/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __COMMMGR_H__
#define __COMMMGR_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhsma.h"
#include "rkhtmr.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ........................ Declares active object ......................... */
RKH_SMA_DCLR(commMgr);

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE idle, WaitSync, SendingStatus, ReceivingAck, SendingEndOfHist, SendingHist, EndCycle;
RKH_DCLR_COMP_STATE active;
RKH_DCLR_CHOICE_STATE C0, C1, C2, C3;

/* ------------------------------- Data types ------------------------------ */
/* ............................. Active object ............................. */
typedef struct CommMgr CommMgr;
struct CommMgr
{
    RKH_SMA_T sma;      /* base structure */
    RKHTmEvt tmEvtObj0;
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
