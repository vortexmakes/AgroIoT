/**
 *  \file       events.h
 *  \brief      Event data types definitions.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.02  DaBa  v1.0.00  Initial version
 *  2018.05.11  LeFr  v1.0.01
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina db@vortexmakes.com
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __EVENTS_H__
#define __EVENTS_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhevt.h"
#include "rkhtmr.h"
#include "epoch.h"
#include "rmc.h"
#include "Geo.h"
#include "cbox.h"
#include "GStatus.h"
#include "IOStatus.h"
#include "Trace.h"
#include "Flowmeter.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/**
 *  ImeiEvt process definitions
 */
#define IMEI_LENGTH         15
#define IMEI_BUF_SIZE       IMEI_LENGTH + 1

/**
 * Specifies sizeof send / receive buffers.
 */
#define SEND_BUFF_SIZE      2048
#define RECV_BUFF_SIZE      1024

/**
 *  Oper process definitions
 */
#define OPER_LENGTH         10
#define OPER_BUF_SIZE       OPER_LENGTH + 1

/**
 *  Oper process definitions
 */
#define SMS_MESSAGE_SIZE    160
#define SMS_BUF_SIZE        SMS_MESSAGE_SIZE + 3 // includes \r\n
#define SMS_FROM_BUF_SIZE   20
#define SMS_DEST_BUF_SIZE   20

/**
 * Specifies max modCmd length
 */
#define MODMGR_MAX_SIZEOF_CMDSTR    70

/* ------------------------------- Data types ------------------------------ */
typedef struct ModCmd ModCmd;
struct ModCmd
{
    RKH_EVT_T evt;
    const char *fmt;            /** contains a format string of AT command */
    RKH_SMA_T * const *aoDest;   /** pointer to SMA that receive the modem */
                                 /*  response */
    RKH_TNT_T waitResponseTime; /** timeout waiting for modem response */
    RKH_TNT_T interCmdTime;     /** inter-command delay */
};

typedef struct ModMgrEvt ModMgrEvt;
struct ModMgrEvt
{
    ModCmd args;
    char cmd[MODMGR_MAX_SIZEOF_CMDSTR];
    unsigned char *data;
    ruint nData;
};

typedef struct ModMgrResp ModMgrResp;
struct ModMgrResp
{
    RKH_EVT_T evt;
    RKH_SIG_T fwdEvt;
};

typedef struct SendEvt SendEvt;
struct SendEvt
{
    RKH_EVT_T evt;
    unsigned char buf[SEND_BUFF_SIZE];
    ruint size;
};

typedef struct SendSMSEvt SendSMSEvt;
struct SendSMSEvt
{
    RKH_EVT_T evt;
    char dest[SMS_DEST_BUF_SIZE];
    char buf[SMS_BUF_SIZE];
    ruint size;
};

typedef struct ReceivedEvt ReceivedEvt;
struct ReceivedEvt
{
//    RKH_EVT_T evt;
    ModMgrResp resp;
    unsigned char buf[RECV_BUFF_SIZE];
    ruint size;
};

typedef struct LocalTimeEvt LocalTimeEvt;
struct LocalTimeEvt
{
    ModMgrResp e;
    Time time;
};

typedef struct ImeiEvt ImeiEvt;
struct ImeiEvt
{
    ModMgrResp e;
    char buf[IMEI_BUF_SIZE];
};

typedef struct OperEvt OperEvt;
struct OperEvt
{
    ModMgrResp e;
    char buf[OPER_BUF_SIZE];
};

typedef struct SigLevelEvt SigLevelEvt;
struct SigLevelEvt
{
    ModMgrResp e;
    int value;
};

typedef struct SMSEvt SMSEvt;
struct SMSEvt
{
    ModMgrResp e;
    int index;
    char from[SMS_FROM_BUF_SIZE];
    int size;
    char data[SMS_BUF_SIZE];
};

typedef struct RmcEvt RmcEvt;
struct RmcEvt
{
    RKH_EVT_T evt;
    Rmc rmc;
};

typedef struct GeoEvt GeoEvt;
struct GeoEvt
{
    RKH_EVT_T evt;
    Geo position;
};

typedef struct DigInChangedEvt DigInChangedEvt;
struct DigInChangedEvt
{
    RKH_EVT_T evt;
    DigIn status;
};

typedef struct DigOutChangedEvt DigOutChangedEvt;
struct DigOutChangedEvt
{
    RKH_EVT_T evt;
    DigOut status;
};

typedef struct GStatusEvt GStatusEvt;
struct GStatusEvt
{
    RKH_EVT_T evt;
    GStatusType status;
};

typedef struct SensorData SensorData;
struct SensorData
{
    RKH_EVT_T evt;
    CBOX_STR cbox;
};

typedef struct TraceEvt TraceEvt;
struct TraceEvt
{
    RKH_EVT_T evt;
    TraceId id;
    TraceArg arg0;
    TraceArg arg1;
};

typedef struct FlowmeterEvt FlowmeterEvt;
struct FlowmeterEvt
{
    RKH_EVT_T evt;
    FlowData flow1;
    FlowData flow2;
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
