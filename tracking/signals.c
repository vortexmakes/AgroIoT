/**
 *  \file       signales.c
 *  \brief      Event signal RKH trace publishing.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.09.27  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "signals.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
signals_publishSymbols(void)
{
	RKH_TR_FWK_SIG(evOpen);
	RKH_TR_FWK_SIG(evClose);
    RKH_TR_FWK_SIG(evCmd);
    RKH_TR_FWK_SIG(evResponse);
    RKH_TR_FWK_SIG(evNoResponse);
    RKH_TR_FWK_SIG(evURC);
	RKH_TR_FWK_SIG(evTimeout);
	RKH_TR_FWK_SIG(evRegTimeout);
    RKH_TR_FWK_SIG(evToutWaitResponse);
    RKH_TR_FWK_SIG(evOk);
    RKH_TR_FWK_SIG(evError);
    RKH_TR_FWK_SIG(evToutDelay);
    RKH_TR_FWK_SIG(evSimReady);
    RKH_TR_FWK_SIG(evSimPin);
    RKH_TR_FWK_SIG(evSimError);
    RKH_TR_FWK_SIG(evNoReg);
    RKH_TR_FWK_SIG(evOper);
    RKH_TR_FWK_SIG(evReg);
    RKH_TR_FWK_SIG(evIP);
    RKH_TR_FWK_SIG(evIPInitial);
    RKH_TR_FWK_SIG(evIPStart);
    RKH_TR_FWK_SIG(evIPStatus);
    RKH_TR_FWK_SIG(evIPGprsAct);
    RKH_TR_FWK_SIG(evConnecting);
    RKH_TR_FWK_SIG(evClosed);
    RKH_TR_FWK_SIG(evConnected);
    RKH_TR_FWK_SIG(evSend);
    RKH_TR_FWK_SIG(evSendFail);
    RKH_TR_FWK_SIG(evSent);
    RKH_TR_FWK_SIG(evRecv);
    RKH_TR_FWK_SIG(evRecvFail);
    RKH_TR_FWK_SIG(evReceived);
    RKH_TR_FWK_SIG(evNetConnected);
    RKH_TR_FWK_SIG(evNetDisconnected);
    RKH_TR_FWK_SIG(evDisconnected);
    RKH_TR_FWK_SIG(evNetClockSync);
    RKH_TR_FWK_SIG(evLocalTime);
    RKH_TR_FWK_SIG(evUnlocked);
    RKH_TR_FWK_SIG(evImei);
    RKH_TR_FWK_SIG(evSigLevel);
    RKH_TR_FWK_SIG(evRMC);
    RKH_TR_FWK_SIG(evGeoStamp);
    RKH_TR_FWK_SIG(evGeoStampInvalid);
    RKH_TR_FWK_SIG(evTurn);
    RKH_TR_FWK_SIG(evIoChg);
	RKH_TR_FWK_SIG(evRestart);
	RKH_TR_FWK_SIG(evEndOfCycle);
	RKH_TR_FWK_SIG(evSensorData);
	RKH_TR_FWK_SIG(evTerminate);
    RKH_TR_FWK_SIG(evDevData);
    RKH_TR_FWK_SIG(evRawData);
    RKH_TR_FWK_SIG(evSyncTout);
}

/* ------------------------------ End of file ------------------------------ */
