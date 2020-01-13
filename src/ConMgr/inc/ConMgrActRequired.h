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
void init(ConMgr *const me);
void open(void);
void close(void);
void sendSync(void);
void ModCmd_getPinStatus(void);
void setPin(void);
void powerOn(void);
void powerOff(void);
void checkReg(void);
void configureInit(void);
void configTry(void);
void connectTry(void);
void ConnectInit(void);
void socketOpen(void);
void ModCmd_disconnect(void);
void ModCmd_getConnStatus(void);
void resetParser(void);
void readData(void);
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
