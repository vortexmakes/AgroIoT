/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __CONMGR_H__
#define __CONMGR_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhsma.h"
#include "rkhtmr.h"
#include "Config.h"
#include "events.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/**
 * Specifies sizeof send / receive buffers.
 */
#define SEND_BUFF_SIZE      2048
#define RECV_BUFF_SIZE      1024

/**
 *  ImeiEvt process definitions
 */
#define IMEI_LENGTH         15
#define IMEI_BUF_SIZE       IMEI_LENGTH + 1

/**
 *  ImeiEvt process definitions
 */
#define OPER_LENGTH         10
#define OPER_BUF_SIZE       OPER_LENGTH + 1

/**
 *  Domain process definitions
 */
#define DOMAIN_BUF_SIZE     IP_LENGTH + 1

/**
 *  Port process definitions
 */
#define PORT_BUF_SIZE       PORT_LENGTH + 1

/* ........................ Declares active object ......................... */
RKH_SMA_DCLR(conMgr);

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE ConMgr_Inactive, ConMgr_Sync, ConMgr_Init, ConMgr_Pin, ConMgr_SetPin, ConMgr_EnableNetTime, ConMgr_GetImei, ConMgr_CipShutdown, ConMgr_SetManualGet, ConFailure, Unregistered, ConMgr_LocalTime, ConMgr_WaitingServer, ConMgr_Idle, ConMgr_receiving, ConMgr_GetStatus, ConMgr_waitOk, ConMgr_WaitPrompt, ConMgr_Restarting, ConMgr_WaitReopen, ConMgr_WaitRetryConfig, ConMgr_WaitNetClockSync, ConMgr_SetAPN, ConMgr_EnableNetwork, ConMgr_CheckIP, ConMgr_GetOper, ConMgr_WaitRetryConnect, ConMgr_Disconnecting, ConMgr_SMS;
RKH_DCLR_COMP_STATE ConMgr_Active, ConMgr_Initialize, ConMgr_Registered, ConMgr_Connecting, ConMgr_Connected, ConMgr_Sending, ConMgr_Configure;
RKH_DCLR_CHOICE_STATE C0, C1, C2;
RKH_DCLR_FINAL_STATE ConMgr_Initialize_Final, ConMgr_Sending_Final, ConMgr_Configure_Final, ConMgr_Registered_Final, ConMgr_Registered_Final, ConMgr_Active_Final;
RKH_DCLR_SHIST_STATE ConMgr_HConfigure;

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
    rInt retryCount;

    SendEvt *psend;
    int sigLevel;
    char Imei[IMEI_BUF_SIZE];
    char Oper[OPER_BUF_SIZE];
    char Domain[DOMAIN_BUF_SIZE];
    char Port[PORT_BUF_SIZE];
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
