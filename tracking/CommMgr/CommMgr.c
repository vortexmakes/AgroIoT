/**
 *  \file       CommMgr.c
 *  \brief      Communication Manager active object implementation.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.06.05  DaBa  v1.0.00   Initial version
 *  2019.16.01  LeFr  v2.0.00   
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Bali�a db@vortexmakes.com
 *  LeFr  Leandro Francucci lf@vortexmakes.com
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
#include "CommMgr.h"
#include "epoch.h"
#include "date.h"
#include "conMgr.h"

/* ----------------------------- Local macros ------------------------------ */
#define WAIT_TIME    RKH_TIME_MS(2000)
#define TEST_FRAME   "!0|12359094043105600,120000,-38.0050660,-057.5443696," \
                     "000.000,000,050514,00FF,0000,00,00,FFFF,FFFF,FFFF,+0"

#define TEST_FRAME_HEADER   "!0|12"
#define TEST_FRAME_TAIL     "+0"

//#define TEST_FRAME_TAIL     "00FF,0000,00,00,FFFF,FFFF,FFFF,+0"

/* ......................... Declares active object ........................ */
typedef struct CommMgr CommMgr;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE Idle, WaitSync, SendingStatus, ReceivingAck, EndCycle,
                     SendingHist, SendingEndOfHist;
RKH_DCLR_COMP_STATE Active; 
RKH_DCLR_CHOICE_STATE ChkRecv, ChkPendStatus, ChkHist, ChkEndOfBlock;
                    
/* ........................ Declares initial action ........................ */
static void init(CommMgr *const me, RKH_EVT_T *pe);

/* ........................ Declares effect actions ........................ */
static void activateSync(CommMgr *const me, RKH_EVT_T *pe);
static void updateStatus(CommMgr *const me, RKH_EVT_T *pe);
static void sendMsgFail(CommMgr *const me, RKH_EVT_T *pe);
static void recvFail(CommMgr *const me, RKH_EVT_T *pe);
static void parseRecv(CommMgr *const me, RKH_EVT_T *pe);
static void parseError(CommMgr *const me, RKH_EVT_T *pe);
static void initSendBlock(CommMgr *const me, RKH_EVT_T *pe);
static void nextSend(CommMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void receive(CommMgr *const me);
static void sendStatus(CommMgr *const me);
static void sendOneMsg(CommMgr *const me);
static void sendEndOfHist(CommMgr *const me);

/* ......................... Declares exit actions ......................... */
/* ............................ Declares guards ............................ */
rbool_t isAck(CommMgr *const me, RKH_EVT_T *pe);
rbool_t isPending(CommMgr *const me, RKH_EVT_T *pe);
rbool_t isThereMsg(CommMgr *const me, RKH_EVT_T *pe);
rbool_t isEndOfBlock(CommMgr *const me, RKH_EVT_T *pe);

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(Idle, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(Idle)
    RKH_TRREG(evNetConnected, NULL, NULL, &Active),
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(Active, NULL, NULL, RKH_ROOT, 
                             &WaitSync, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(Active)
    RKH_TRINT(evRawData,         NULL, updateStatus),
    RKH_TRREG(evNetDisconnected, NULL, NULL, &Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(WaitSync, NULL, NULL, &Active, NULL);
RKH_CREATE_TRANS_TABLE(WaitSync)
    RKH_TRREG(evSyncTout, NULL, NULL, &SendingStatus),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(SendingStatus, sendStatus, NULL, &Active, NULL);
RKH_CREATE_TRANS_TABLE(SendingStatus)
    RKH_TRREG(evSent,     NULL, NULL, &ReceivingAck),
    RKH_TRREG(evSendFail, NULL, sendMsgFail, &EndCycle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ReceivingAck, receive, NULL, &Active, NULL);
RKH_CREATE_TRANS_TABLE(ReceivingAck)
    RKH_TRREG(evReceived, NULL, parseRecv, &ChkRecv),
    RKH_TRREG(evRecvFail, NULL, recvFail, &EndCycle),
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(ChkRecv);
RKH_CREATE_BRANCH_TABLE(ChkRecv)
    RKH_BRANCH(isAck, NULL, &ChkPendStatus),
    RKH_BRANCH(ELSE,  parseError, &EndCycle),
RKH_END_BRANCH_TABLE

RKH_CREATE_CHOICE_STATE(ChkPendStatus);
RKH_CREATE_BRANCH_TABLE(ChkPendStatus)
    RKH_BRANCH(isPending, NULL, &SendingStatus),
    RKH_BRANCH(ELSE,      NULL, &ChkHist),
RKH_END_BRANCH_TABLE

RKH_CREATE_CHOICE_STATE(ChkHist);
RKH_CREATE_BRANCH_TABLE(ChkHist)
    RKH_BRANCH(isThereMsg, initSendBlock, &SendingHist),
    RKH_BRANCH(ELSE,       NULL, &EndCycle),
RKH_END_BRANCH_TABLE

RKH_CREATE_BASIC_STATE(SendingHist, sendOneMsg, NULL, &Active, NULL);
RKH_CREATE_TRANS_TABLE(SendingHist)
    RKH_TRREG(evSent,     NULL, nextSend, &ChkEndOfBlock),
    RKH_TRREG(evSendFail, NULL, sendMsgFail, &EndCycle),
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(ChkEndOfBlock);
RKH_CREATE_BRANCH_TABLE(ChkEndOfBlock)
    RKH_BRANCH(isEndOfBlock, NULL, &SendingHist),
    RKH_BRANCH(ELSE,         NULL, &SendingEndOfHist),
RKH_END_BRANCH_TABLE

RKH_CREATE_BASIC_STATE(SendingEndOfHist, sendEndOfHist, NULL, &Active, NULL);
RKH_CREATE_TRANS_TABLE(SendingEndOfHist)
    RKH_TRREG(evSent,     NULL, NULL, &ReceivingAck),
    RKH_TRREG(evSendFail, NULL, sendMsgFail, &EndCycle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(EndCycle, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(EndCycle)
    RKH_TRCOMPLETION(NULL, NULL, &WaitSync),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct CommMgr
{
    RKH_SMA_T ao;
    RKH_TMR_T timer;    
    Geo geo;
    CBOX_STR rawData;   /* Current raw data (GPS+IO, etc) */
};

RKH_SMA_CREATE(CommMgr, commMgr, 4, HCAL, &Idle, init, NULL);
RKH_SMA_DEF_PTR(commMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_STATIC_EVENT(e_tout, evTimeout);
static RKH_ROM_STATIC_EVENT(evRecvObj, evRecv);
static SendEvt evSendObj;
static char *testFrame = TEST_FRAME;
static ruint din;
static ruint dout;
static CBOX_STR cbox;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
static void
init(CommMgr *const me, RKH_EVT_T *pe)
{
	(void)pe;

    rkh_pubsub_subscribe(ConnectionTopic, RKH_UPCAST(RKH_SMA_T, me));

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_TIMER(&me->timer);

    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &Idle);
    RKH_TR_FWK_STATE(me, &WaitSync);
    RKH_TR_FWK_STATE(me, &SendingStatus);
    RKH_TR_FWK_STATE(me, &ReceivingAck);
    RKH_TR_FWK_STATE(me, &EndCycle);
    RKH_TR_FWK_STATE(me, &SendingHist);
    RKH_TR_FWK_STATE(me, &SendingEndOfHist);
    RKH_TR_FWK_STATE(me, &Active);

    RKH_SET_STATIC_EVENT(RKH_UPCAST(RKH_EVT_T, &evSendObj), evSend);
    RKH_TMR_INIT(&me->timer, &e_tout, NULL);
}

/* ............................ Effect actions ............................. */
static void 
activateSync(CommMgr *const me, RKH_EVT_T *pe)
{
}

static void
updateStatus(CommMgr *const me, RKH_EVT_T *pe)
{
    me->rawData = ((RawDataEvt *)pe)->rawData;
}

static void 
sendMsgFail(CommMgr *const me, RKH_EVT_T *pe)
{
}

static void 
recvFail(CommMgr *const me, RKH_EVT_T *pe)
{
}

static void 
parseRecv(CommMgr *const me, RKH_EVT_T *pe)
{
}

static void 
parseError(CommMgr *const me, RKH_EVT_T *pe)
{
}

static void 
initSendBlock(CommMgr *const me, RKH_EVT_T *pe)
{
}

static void 
nextSend(CommMgr *const me, RKH_EVT_T *pe)
{
}

/* ............................. Entry actions ............................. */
static void
receive(CommMgr *const me)
{
    rkh_pubsub_publish(ConnectionTopic, RKH_UPCAST(RKH_EVT_T, &evRecvObj),
                                        RKH_UPCAST(RKH_SMA_T, me));
}

static void 
sendStatus(CommMgr *const me)
{
    (void)me;
}

static void 
sendOneMsg(CommMgr *const me)
{
    (void)me;
}

static void 
sendEndOfHist(CommMgr *const me)
{
    (void)me;
}

/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
rbool_t
isAck(CommMgr *const me, RKH_EVT_T *pe)
{
    ReceivedEvt *evt;

    (void)me;
    evt = RKH_DOWNCAST(ReceivedEvt, pe);
    return (evt->size != 0) ? RKH_TRUE : RKH_FALSE;
}

rbool_t isPending(CommMgr *const me, RKH_EVT_T *pe)
{
    return RKH_FALSE;
}

rbool_t isThereMsg(CommMgr *const me, RKH_EVT_T *pe)
{
    return RKH_FALSE;
}

rbool_t 
isEndOfBlock(CommMgr *const me, RKH_EVT_T *pe)
{
    return RKH_FALSE;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
