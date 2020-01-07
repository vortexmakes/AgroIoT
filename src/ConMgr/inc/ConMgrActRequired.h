/**
 *  \file       ConMgrActRequired.h
 *  \brief
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __CONMGRACTREQUIRED_H__
#define __CONMGRACTREQUIRED_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"
#include "ConMgrActAccess.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
	extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void init(void);
void open(void);
void close(void);
void initializeInit(void);
void recvFail(void);
void sendFail(void);
void sendSync(void);
void setSigLevel(void);
void sendInit(void);
void registered(void);
void unregistered(void);
void checkPin(void);
void setPin(void);
void PowerOn(void);
void PowerOff(void);
void ModCmd_init(void);
void netTimeEnable(void);
void getImei(void);
void cipShutdown(void);
void setManualGet(void);
void checkReg(void);
void checkRegStatus(void);
void startRegStatusTimer(void);
void localTimeGet(void);
void rtimeSync(void);
void configureInit(void);
void getOper(void);
void storeOper(void);
void setupAPN(void);
void requestIP(void);
void startNetwork(void);
void getIpStatus(void);
void configTry(void);
void connectTry(void);
void ConnectInit(void);
void socketOpen(void);
void socketClose(void);
void getConnStatus(void);
void socketConnected(void);
void socketDisconnected(void);
void resetParser(void);
void defer(void);
void sendOk(void);
void flushData(void);
void incRetryCount(void);
void resetRetryCount(void);
void sendRequest(void);
void readData(void);
void recvOk(void);
void stopSMS(void);
rbool_t keepTrying(void);
rbool_t checkConfigTry(void);
rbool_t checkConnectTry(void);
rbool_t inTryGetStatus(void);
rbool_t isPending(void);
rbool_t isAck(void);
rbool_t isThereMsg(void);
rbool_t isEndOfBlock(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
