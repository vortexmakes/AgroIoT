/**
 *  \file       CommMgrActRequired.h
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
#ifndef __COMMMGRACTREQUIRED_H__
#define __COMMMGRACTREQUIRED_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"
#include "CommMgrActAccess.h"

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
void activateSync(void);
void enWaitSync(void);
void exWaitSync(void);
void sendEndOfHist(void);
void parseRecv(void);
void receive(void);
void recvFail(void);
void parseError(void);
void sendOneMsg(void);
void sendStatus(void);
void sendMsgFail(void);
void nextSend(void);
void initSendBlock(void);
void updateStatus(void);
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
