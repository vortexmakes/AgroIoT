/**
 *  \file       modcmdSim5320.c
 *  \brief      Implementation of module command abstraction.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.02.12  DaBa  v1.0.01  sync
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña      db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "rkhfwk_dynevt.h"
#include "modcmd.h"
#include "modmgr.h"
#include "conmgr.h"
#include "sim5320parser.h"
#include "signals.h"
#include <string.h>
#include <stdio.h>

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct CmdTbl CmdTbl;
struct CmdTbl
{
    ModCmd sync;
    ModCmd initStr;
    ModCmd getPinStatus;
    ModCmd setPin;
    ModCmd getRegStatus;
    ModCmd enableNetTime;
    ModCmd getImei;
    ModCmd cipShutdown;
    ModCmd setManualGet;
    ModCmd getLocalTime;
    ModCmd getOper;
    ModCmd setAPN;
    ModCmd startNetwork;
    ModCmd requestIP;
    ModCmd getIpStatus;
    ModCmd getConnStatus;
    ModCmd connect;
    ModCmd disconnect;
    ModCmd sendDataRequest;
    ModCmd sendData;
    ModCmd readData;
};

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static SSP sim5320Parser;

static const CmdTbl cmdTbl =
{
    /* sync */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT\r\n",
     &conMgr,
     RKH_TIME_MS(2000), RKH_TIME_MS(2000)},

    /* initStr */
    {RKH_INIT_STATIC_EVT(evCmd),
     "ATE1+CREG=1;V1;Q0;+STK=0;+IFC=0,0;+CIPSRIP=0;+CIPHEAD=0\r\n",
     &conMgr,
     RKH_TIME_MS(5000), RKH_TIME_MS(5000)},

    /* getPinStatus */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CPIN?\r\n",
     &conMgr,
     RKH_TIME_MS(1500), RKH_TIME_MS(100)},

    /* setPin */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CPIN=%d\r\n",
     &conMgr,
     RKH_TIME_MS(500), RKH_TIME_MS(100)},

    /* getRegStatus */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CREG?;+CSQ\r\n",
     &conMgr,
     RKH_TIME_MS(300), RKH_TIME_MS(500)},

    /* enableNetTime */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CTZR=1;+COPS=3,2\r\n",
     &conMgr,
     RKH_TIME_MS(300), RKH_TIME_MS(100)},

    /* getImei */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CGSN\r\n",
     &conMgr,
     RKH_TIME_MS(500), RKH_TIME_MS(100)},

    /* cipShutdown */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CGACT=0,1\r\n",
     &conMgr,
     RKH_TIME_MS(3000), RKH_TIME_MS(200)},

    /* setManualGet */
    {RKH_INIT_STATIC_EVT(evCmd),
#ifdef GPRS_QUICK_SEND
     "AT+CIPRXGET=1;+CIPQSEND=1\r\n",
#else
     "AT+CIPRXGET=1\r\n",
#endif
     &conMgr,
     RKH_TIME_MS(300), RKH_TIME_MS(100)},

    /* getLocalTime */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CCLK?\r\n",
     &conMgr,
     RKH_TIME_MS(300), RKH_TIME_MS(100)},

    /* getOper */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+COPS?\r\n",
     &conMgr,
     RKH_TIME_MS(2000), RKH_TIME_MS(100)},

    /* setAPN */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CGSOCKCONT=1,\"IP\",\"%s\"\r\n",
     &conMgr,
     RKH_TIME_MS(1000), RKH_TIME_MS(500)},

    /* startNetwork */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CGATT=1;+CGACT=1,1;+NETOPEN\r\n",
     &conMgr,
     RKH_TIME_MS(10000), RKH_TIME_MS(200)},

    /* requestIP */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CIFSR\r\n",
     &conMgr,
     RKH_TIME_MS(1000), RKH_TIME_MS(100)},

    /* getIpStatus */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+IPADDR\r\n",
     &conMgr,
     RKH_TIME_MS(1000), RKH_TIME_MS(100)},

    /* getConnStatus */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CIPSTATUS=0;+CSQ\r\n",
     &conMgr,
     RKH_TIME_MS(100), RKH_TIME_MS(100)},

    /* connect */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CIPOPEN=0,\"%s\",\"%s\",%s\r\n",
     &conMgr,
     RKH_TIME_MS(1000), RKH_TIME_MS(300)},

    /* disconnect */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CIPCLOSE\r\n",
     &conMgr,
     RKH_TIME_MS(1000), RKH_TIME_MS(100)},

    /* sendDataRequest */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CIPSEND=0,%d\r\n",
     &conMgr,
     RKH_TIME_MS(3000), RKH_TIME_MS(100)},

    /* sendData */
    {RKH_INIT_STATIC_EVT(evCmd),
     "\x1A\r\n",
     &conMgr,
#ifdef GPRS_QUICK_SEND
     RKH_TIME_MS(10000), RKH_TIME_MS(500)},
#else
     RKH_TIME_MS(20000), RKH_TIME_MS(500)},
#endif

    /* readData */
    {RKH_INIT_STATIC_EVT(evCmd),
     "AT+CIPRXGET=2,0,1024\r\n",
     &conMgr,
     RKH_TIME_MS(3000), RKH_TIME_MS(500)},
};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
doSearch(unsigned char c)
{
    ssp_doSearch(&sim5320Parser, c);
}

static void
postFIFOEvtCmd(ModMgrEvt *pe, const ModCmd *pc, unsigned char *data,
               ruint nData)
{
    pe->data = data;
    pe->nData = nData;
    pe->args.fmt = pc->fmt;
    pe->args.aoDest = pc->aoDest;
    pe->args.waitResponseTime = pc->waitResponseTime;
    pe->args.interCmdTime = pc->interCmdTime;

    RKH_SMA_POST_FIFO(modMgr, RKH_UPCAST(RKH_EVT_T, pe), *pc->aoDest);
}

static void
sendModCmd_noArgs(const ModCmd *p)
{
    ModMgrEvt *evtCmd;

    evtCmd = RKH_ALLOC_EVT(ModMgrEvt, evCmd, *p->aoDest);

    strncpy(evtCmd->cmd, p->fmt, MODMGR_MAX_SIZEOF_CMDSTR);

    postFIFOEvtCmd(evtCmd, p, NULL, 0);
}

static void
sendModCmd_rui16(const ModCmd *p, rui16_t arg)
{
    ModMgrEvt *evtCmd;

    evtCmd = RKH_ALLOC_EVT(ModMgrEvt, evCmd, *p->aoDest);

    snprintf(evtCmd->cmd, MODMGR_MAX_SIZEOF_CMDSTR, p->fmt, arg);

    postFIFOEvtCmd(evtCmd, p, NULL, 0);
}

static void
sendModCmd_3StrArgs(const ModCmd *p, char *s1, char *s2, char *s3)
{
    ModMgrEvt *evtCmd;

    evtCmd = RKH_ALLOC_EVT(ModMgrEvt, evCmd, *p->aoDest);

    snprintf(evtCmd->cmd, MODMGR_MAX_SIZEOF_CMDSTR, p->fmt, s1, s2, s3);

    postFIFOEvtCmd(evtCmd, p, NULL, 0);
}

/* ---------------------------- Global functions --------------------------- */
ModCmdRcvHandler
ModCmd_init(void)
{
    RKH_SR_ALLOC();

    RKH_ENTER_CRITICAL_();
    ssp_init(&sim5320Parser, &rootCmdParser);
    RKH_EXIT_CRITICAL_();
    return &doSearch;
}

void
ModCmd_sync(void)
{
    sendModCmd_noArgs(&cmdTbl.sync);
}

void
ModCmd_initStr(void)
{
    sendModCmd_noArgs(&cmdTbl.initStr);
}

void
ModCmd_getPinStatus(void)
{
    sendModCmd_noArgs(&cmdTbl.getPinStatus);
}

void
ModCmd_setPin(rui16_t pin)
{
    sendModCmd_rui16(&cmdTbl.setPin, pin);
}

void
ModCmd_getRegStatus(void)
{
    sendModCmd_noArgs(&cmdTbl.getRegStatus);
}

void
ModCmd_enableNetTime(void)
{
    sendModCmd_noArgs(&cmdTbl.enableNetTime);
}

void
ModCmd_getImei(void)
{
    sendModCmd_noArgs(&cmdTbl.getImei);
}

void
ModCmd_cipShutdown(void)
{
    sendModCmd_noArgs(&cmdTbl.cipShutdown);
}

void
ModCmd_setManualGet(void)
{
    sendModCmd_noArgs(&cmdTbl.setManualGet);
}

void
ModCmd_getLocalTime(void)
{
    sendModCmd_noArgs(&cmdTbl.getLocalTime);
}

void
ModCmd_getOper(void)
{
    sendModCmd_noArgs(&cmdTbl.getOper);
}

void
ModCmd_setupAPN(char *apn, char *usr, char *nm)
{
    /*sendModCmd_3StrArgs(&cmdTbl.setAPN, apn, usr, nm); */

    const ModCmd *p;
    ModMgrEvt *evtCmd;

    p = &cmdTbl.setAPN;
    evtCmd = RKH_ALLOC_EVT(ModMgrEvt, evCmd, *p->aoDest);

    snprintf(evtCmd->cmd, MODMGR_MAX_SIZEOF_CMDSTR, p->fmt, apn);

    postFIFOEvtCmd(evtCmd, p, NULL, 0);
}

void
ModCmd_startNetwork(void)
{
    sendModCmd_noArgs(&cmdTbl.startNetwork);
}

void
ModCmd_requestIP(void)
{
    sendModCmd_noArgs(&cmdTbl.requestIP);
}

void
ModCmd_getIpStatus(void)
{
    sendModCmd_noArgs(&cmdTbl.getIpStatus);
}

void
ModCmd_getConnStatus(void)
{
    sendModCmd_noArgs(&cmdTbl.getConnStatus);
}

void
ModCmd_connect(char *prot, char *dmn, char *port)
{
    sendModCmd_3StrArgs(&cmdTbl.connect, prot, dmn, port);
}

void
ModCmd_disconnect(void)
{
    sendModCmd_noArgs(&cmdTbl.disconnect);
}

void
ModCmd_sendDataRequest(rui16_t len)
{
    sendModCmd_rui16(&cmdTbl.sendDataRequest, len);
}

void
ModCmd_sendData(unsigned char *buf, ruint size)
{
    ModMgrEvt *evtCmd;
    const ModCmd *p;

    p = &cmdTbl.sendData;

    evtCmd = RKH_ALLOC_EVT(ModMgrEvt, evCmd, *p->aoDest);

    evtCmd->data = buf;
    evtCmd->nData = size;

    postFIFOEvtCmd(evtCmd, p, buf, size);
}

void
ModCmd_readData(void)
{
    sendModCmd_noArgs(&cmdTbl.readData);
}

char *
ModCmd_endOfXmitStr(void)
{
    return (char *)cmdTbl.sendData.fmt;
}

/* ------------------------------ End of file ------------------------------ */
