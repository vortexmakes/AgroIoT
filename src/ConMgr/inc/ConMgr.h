/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __CONMGR_H__
#define __CONMGR_H__

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
RKH_SMA_DCLR(conMgr);

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE inactive, sync, init, pin, setPin, enableNetTime, getImei, cipShutdown, setManualGet, failure, unregistered, localTime, waitingServer, idle, receiving, getStatus, waitOk, waitPrompt, restarting, wReopen, waitRetryConfig, waitNetClockSync, setAPN, enableNetwork, checkIP, getOper, waitRetryConnect, disconnecting, SMS;
RKH_DCLR_COMP_STATE Active, initialize, registered, connecting, connected, sending, configure;
RKH_DCLR_CHOICE_STATE C0, C1, C2;
RKH_DCLR_FINAL_STATE initialize_Final, sending_Final, configure_Final, registered_Final, registered_Final, Active_Final;
RKH_DCLR_SHIST_STATE HConfigure;

/* ------------------------------- Data types ------------------------------ */
/* ............................. Active object ............................. */
typedef struct ConMgr ConMgr;
struct ConMgr
{
    RKH_SMA_T sma;      /* base structure */
    RKHTmEvt tmEvtObj0;
    RKHTmEvt tmEvtObj1;
    RKHTmEvt tmEvtObj2;
    RKHTmEvt tmEvtObj3;
    RKHTmEvt tmEvtObj4;
    RKHTmEvt tmEvtObj5;
    RKHTmEvt tmEvtObj6;
    RKHTmEvt tmEvtObj7;
    rInt FAILURE_TRY_DELAY;
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
