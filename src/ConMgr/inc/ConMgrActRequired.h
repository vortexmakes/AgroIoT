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
void ModCmd_getPinStatus(void);
void setPin(void);
void powerOn(void);
void powerOff(void);
void checkReg(void);
void startRegStatusTimer(void);
void rtimeSync(void);
void configureInit(void);
void storeOper(void);
void setupAPN(void);
void configTry(void);
void connectTry(void);
void ConnectInit(void);
void socketOpen(void);
void ModCmd_disconnect(void);
void ModCmd_getConnStatus(void);
void socketConnected(void);
void socketDisconnected(void);
void resetParser(void);
void ConMgr_defer(void);
void ConMgr_recall(void);
void sendOk(void);
void flushData(void);
void sendRequest(void);
void readData(void);
void recvOk(void);
void stopSMS(void);
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
