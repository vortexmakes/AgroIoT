/**
 *  \file       trkClient.c
 *  \brief      Yipies Tracking Client.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.06.05  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdio.h>
#include <string.h>
#include "rkh.h"
#include "rkhfwk_pubsub.h"
#include "rkhtmr.h"
#include "bsp.h"
#include "signals.h"
#include "events.h"
#include "topics.h"
#include "trkClient.h"
#include "epoch.h"
#include "date.h"
#include "conMgr.h"

/* ----------------------------- Local macros ------------------------------ */
#define WAIT_TIME    RKH_TIME_MS(2000)
#define TEST_FRAME   "!0|12359094043105600,120000,-38.0050660,-057.5443696," \
                     "000.000,000,050514,00FF,0000,00,00,FFFF,FFFF,FFFF,+0"

#define TEST_FRAME_HEADER   "!0|12"
#define TEST_FRAME_TAIL     "00FF,0000,00,00,FFFF,FFFF,FFFF,+0"

/* ......................... Declares active object ........................ */
typedef struct TrkClient TrkClient;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE Client_Disconnected, Client_Send, Client_Receive,
                     Client_Idle; 
RKH_DCLR_COMP_STATE Client_Connected; 
RKH_DCLR_COND_STATE Client_CheckResp;
                    
/* ........................ Declares initial action ........................ */
static void init(TrkClient *const me, RKH_EVT_T *pe);

/* ........................ Declares effect actions ........................ */
static void sendFrame(TrkClient *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void sendFail(TrkClient *const me);
static void recvEntry(TrkClient *const me);
static void recvFail(TrkClient *const me);

/* ......................... Declares exit actions ......................... */
static void waitExit(TrkClient *const me);

/* ............................ Declares guards ............................ */
rbool_t checkResp(TrkClient *const me, RKH_EVT_T *pe);

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(Client_Disconnected, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(Client_Disconnected)
    RKH_TRREG(evNetConnected, NULL, NULL, &Client_Connected),
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(Client_Connected, NULL, NULL, RKH_ROOT, 
                             &Client_Idle, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(Client_Connected)
    RKH_TRREG(evNetDisconnected, NULL, NULL, &Client_Disconnected),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(Client_Idle, NULL, NULL, &Client_Connected, NULL);
RKH_CREATE_TRANS_TABLE(Client_Idle)
    RKH_TRREG(evGeoStamp,           NULL, sendFrame, &Client_Send),
    RKH_TRREG(evGeoStampInvalid,    NULL, sendFrame, &Client_Send),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(Client_Send, NULL, NULL, &Client_Connected, NULL);
RKH_CREATE_TRANS_TABLE(Client_Send)
    RKH_TRREG(evSent,     NULL, NULL, &Client_Receive),
    RKH_TRREG(evSendFail, NULL, sendFail, &Client_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(Client_Receive, recvEntry, NULL, &Client_Connected, NULL);
RKH_CREATE_TRANS_TABLE(Client_Receive)
    RKH_TRREG(evReceived, NULL, NULL, &Client_CheckResp),
    RKH_TRREG(evRecvFail, NULL, recvFail, &Client_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_COND_STATE(Client_CheckResp);
RKH_CREATE_BRANCH_TABLE(Client_CheckResp)
    RKH_BRANCH(checkResp,   NULL,   &Client_Idle),
    RKH_BRANCH(ELSE,        NULL,   &Client_Receive),
RKH_END_BRANCH_TABLE

/* ............................. Active object ............................. */
struct TrkClient
{
    RKH_SMA_T ao;       /* Base structure */
    RKH_TMR_T timer;    
};

RKH_SMA_CREATE(TrkClient, trkClient, 3, HCAL, &Client_Disconnected, init, NULL);
RKH_SMA_DEF_PTR(trkClient);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_STATIC_EVENT(e_tout, evTimeout);
static RKH_ROM_STATIC_EVENT(evRecvObj, evRecv);
static SendEvt evSendObj;
static char *testFrame = TEST_FRAME;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
static void
init(TrkClient *const me, RKH_EVT_T *pe)
{
	(void)pe;

    tpConnection_subscribe(me);
    tpGeo_subscribe(me);
    tpIoChg_subscribe(me);

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_TIMER(&me->timer);

    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &Client_Disconnected);
    RKH_TR_FWK_STATE(me, &Client_Connected);
    RKH_TR_FWK_STATE(me, &Client_Idle);
    RKH_TR_FWK_STATE(me, &Client_Send);
    RKH_TR_FWK_STATE(me, &Client_Receive);

    RKH_SET_STATIC_EVENT(RKH_UPCAST(RKH_EVT_T, &evSendObj), evSend);
    RKH_TMR_INIT(&me->timer, &e_tout, NULL);
}

/* ............................ Effect actions ............................. */
static void
sendFrame(TrkClient *const me, RKH_EVT_T *pe)
{
    char *p;
    GeoStamp *geo;

    geo = &(((GeoStampEvt *)pe)->gps);

    p = (char *)evSendObj.buf;

    sprintf(p, "%s", TEST_FRAME_HEADER);
    strcat(p, ConMgr_Imei());
    strcat(p, ",");
    strcat(p, geo->utc);
    strcat(p, ",");
	strcat(p, geo->latInd);
	strcat(p, geo->latitude);
    strcat(p, ",");
	strcat(p, geo->longInd);
	strcat(p, geo->longitude);
    strcat(p, ",");
    strcat(p, geo->speed);
    strcat(p, ",");
    strcat(p, geo->course);
    strcat(p, ",");
    strcat(p, geo->date);
    strcat(p, ",");
    strcat(p, TEST_FRAME_TAIL);

    evSendObj.size = strlen((char *)evSendObj.buf);
    tpConnection_publish(&evSendObj, me);
}

/* ............................. Entry actions ............................. */
static void
sendFail(TrkClient *const me)
{
    (void)me;

    bsp_sendFail();
}

static void
recvEntry(TrkClient *const me)
{
    tpConnection_publish(&evRecvObj, me);
}

static void
recvFail(TrkClient *const me)
{
    (void)me;

    bsp_recvFail();
}


/* ............................. Exit actions .............................. */
static void
waitExit(TrkClient *const me)
{
    rkh_tmr_stop(&me->timer);
}


/* ................................ Guards ................................. */
rbool_t
checkResp(TrkClient *const me, RKH_EVT_T *pe)
{
    ReceivedEvt *evt;

    (void)me;
    
    evt = RKH_DOWNCAST(ReceivedEvt, pe);

    return (evt->size != 0) ? RKH_TRUE : RKH_FALSE;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
