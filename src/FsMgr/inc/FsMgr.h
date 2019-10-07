/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __FSMGR_H__
#define __FSMGR_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhsma.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ........................ Declares active object ......................... */
RKH_SMA_DCLR(fsMgr);

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE DiskDisconnected, FsActive, FsError;
RKH_DCLR_COMP_STATE DiskConnected;
RKH_DCLR_CHOICE_STATE C0;

/* ------------------------------- Data types ------------------------------ */
/* ............................. Active object ............................. */
typedef struct FsMgr FsMgr;
struct FsMgr
{
    RKH_SMA_T sma;      /* base structure */
    rInt result;
    rInt FR_OK;
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
