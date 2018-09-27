/**
 *  \file       signals.h
 *  \brief      Event signal definitions.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.09.27  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Bali�a db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __SIGNALS_H__
#define __SIGNALS_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ................................ Signals ................................ */
typedef enum Signals Signals;
enum Signals
{
	evOpen,
	evClose,
    evCmd,
    evResponse,
    evNoResponse,
    evURC,
	evTimeout,
    evToutWaitResponse,
    evOk,
    evError,
    evToutDelay,
    evSimReady,
    evSimPin,
    evSimError,
    evNoReg,
    evReg,
    evIP,
    evIPInitial,
    evIPStart,
    evIPStatus,
    evIPGprsAct,
    evConnecting,
    evClosed,
    evConnected,
    evSend,
    evSendFail,
    evSent,
    evRecv,
    evRecvFail,
    evReceived,
    evNetConnected,
    evNetDisconnected,
    evDisconnected,
    evNetClockSync,
    evLocalTime,
    evUnlocked,
    evImei,
    evRMC,
    evGeoStamp,
    evGeoStampInvalid,
    evTurn,
    evIoChg,
	evRestart,
	evTerminate,     /* press the key escape on the keyboard */
};

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void signals_publishSymbols(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
