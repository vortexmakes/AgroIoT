/**
 *  \file       modcmd.h
 *  \brief      Specification of module command abstraction.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.02.12  DaBa  v1.0.01  sync
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina      db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __MODCMD_H__
#define __MODCMD_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "events.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ................................ Signals ................................ */
/* ........................ Declares active object ......................... */
/* ------------------------------- Data types ------------------------------ */
typedef void (*ModCmdRcvHandler)(unsigned char c);

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
ModCmdRcvHandler ModCmd_init(void);
void ModCmd_sync(void);
void ModCmd_initStr(void);
void ModCmd_getPinStatus(void);
void ModCmd_setPin(rui16_t pin);
void ModCmd_getRegStatus(void);
void ModCmd_enableNetTime(void);
void ModCmd_getImei(void);
void ModCmd_cipShutdown(void);
void ModCmd_setManualGet(void);
void ModCmd_getLocalTime(void);
void ModCmd_getOper(void);
void ModCmd_setupAPN(char *apn, char *usr, char *nm);
void ModCmd_startNetwork(void);
void ModCmd_requestIP(void);
void ModCmd_getIpStatus(void);
void ModCmd_getConnStatus(void);
void ModCmd_connect(char *prot, char *dmn, char *port);
void ModCmd_disconnect(void);
void ModCmd_sendData(unsigned char *buf, ruint size);
void ModCmd_readData(void);
char *ModCmd_endOfXmitStr(void);
void ModCmd_checkSMS(void);
void ModCmd_deleteSMS(unsigned char index);
void ModCmd_sendSMS(char *dest, char *text, ruint size);


/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
