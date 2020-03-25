/*
 */

/**
 *  \file   GsmMgrAccess.h
 *  \brief
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __GSMMGRACCESS_H__
#define __GSMMGRACCESS_H__

/* ----------------------------- Include files ----------------------------- */
#include "Config.h"
#include "GsmMgr.h"
#include "ModMgr.h"
#include "modcmd.h"
#include "rtime.h"
#include "topic.h"

#include <string.h>

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define CONNECTION_PROT     "TCP"

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

/**
 * Size of defer events queue
 */
#define SIZEOF_QDEFER       1

/**
 * Period for checking reg status on unregistered state
 */ 
#define CHECK_REG_STATUS_PERIOD     RKH_TIME_MS(2000)

/**
 * GSM Network Provider and Connection specific configurations.
 */
/* .................................. APN .................................. */
#define MOVISTAR_OPERATOR       "72207"
#define MOVISTAR_APN_ADDR       "INTERNET.GPRS.UNIFON.COM.AR"
#define MOVISTAR_APN_USER       "WAP"
#define MOVISTAR_APN_PASS       "WAP"

#define CLARO_OPERATOR          "722310"
#define CLARO_APN_ADDR          "internet.ctimovil.com.ar"
#define CLARO_APN_USER          "clarogprs"
#define CLARO_APN_PASS          "clarogprs999"

#define PERSONAL_OPERATOR       "72234"
#define PERSONAL_APN_ADDR       "datos.personal.com"
#define PERSONAL_APN_USER       "datos"
#define PERSONAL_APN_PASS       "datos"

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void powerOn(void);
void powerOff(void);
void modemFound(void);
void registered(void);
void unregistered(void);
void stopSMS(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
