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
 *  DaBa  Dario Baliña db@vortexmakes.com
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
#include "topic.h"
#include "CommMgr.h"
#include "epoch.h"
#include "date.h"
#include "conMgr.h"
#include "GStatus.h"
#include "cbox.h"

/* ----------------------------- Local macros ------------------------------ */
#define WAIT_TIME    RKH_TIME_MS(2000)
#define TEST_FRAME   "!0|12359094043105600,120000,-38.0050660,-057.5443696," \
                     "000.000,000,050514,00FF,0000,00,00,FFFF,FFFF,FFFF,+0"

#define YFRAME_SEPARATOR        ","
#define YFRAME_MARK             "|"

#define YFRAME_ID               "!"
#define YFRAME_SGP_TYPE         "0"
#define YFRAME_SEPARATOR        ","
#define YFRAME_GPS_VALID        1
#define YFRAME_HISTORY          2
#define YFRAME_MOVING           4

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
static void enWaitSync(CommMgr *const me);

/* ......................... Declares exit actions ......................... */
static void exWaitSync(CommMgr *const me);

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
    RKH_TRINT(evGStatus,         NULL, updateStatus),
    RKH_TRREG(evNetDisconnected, NULL, NULL, &Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(WaitSync, enWaitSync, exWaitSync, &Active, NULL);
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
    RKH_TMR_T syncTmr;    
    GStatus currStatus;
};

RKH_SMA_CREATE(CommMgr, commMgr, 4, HCAL, &Idle, init, NULL);
RKH_SMA_DEF_PTR(commMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_STATIC_EVENT(evSyncToutObj, evSyncTout);
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

    rkh_pubsub_subscribe(TopicConnection, RKH_UPCAST(RKH_SMA_T, me));

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_TIMER(&me->syncTmr);
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
    RKH_TMR_INIT(&me->syncTmr, &evSyncToutObj, NULL);
}

static unsigned char
yframe_getFlags(GStatus *const status)
{
    unsigned char flags;

    flags = 0;
    flags |= Geo_isValid(&status->position) ? YFRAME_GPS_VALID : 0;
    flags |= cbox_isMoving(&status->dev) ? YFRAME_MOVING : 0;
    flags |= BatChr_getStatus() << 3;

    return flags;
}

static void
get_frame(GStatus *currStatus, char *buf)
{
    char *frame, temp[16];
    Geo *position;
    IOStatus *io;
    CBOX_STR *dev;

    frame = buf;
    position = &currStatus->position;
    io = &currStatus->io;
    dev = &currStatus->dev;

    strcat(frame, YFRAME_ID);
    strcat(frame, YFRAME_SGP_TYPE);
    strcat(frame, YFRAME_MARK);
    sprintf(temp, "%02x", yframe_getFlags(currStatus));
    strcat(frame, temp);
    strcat(frame, ConMgr_Imei());
    strcat(frame, YFRAME_SEPARATOR);
    strcat(frame, position->utc);
    strcat(frame, YFRAME_SEPARATOR);
    strcat(frame, position->latInd);
    strcat(frame, position->latitude);
    strcat(frame, YFRAME_SEPARATOR);
    strcat(frame, position->longInd);
    strcat(frame, position->longitude);
    strcat(frame, YFRAME_SEPARATOR);
    strcat(frame, position->speed);
    strcat(frame, YFRAME_SEPARATOR);
    strcat(frame, position->course);
    strcat(frame, YFRAME_SEPARATOR);
    strcat(frame, position->date);
    strcat(frame, YFRAME_SEPARATOR);
    sprintf(temp, "%02x%02x,", io->digOut, io->digIn);
    strcat(frame, temp);
    sprintf(temp, "%04x,", dev->h.hoard );
    strcat(frame, temp);
    sprintf(temp, "%02x,", dev->h.pqty );
    strcat(frame, temp);
    sprintf(temp, "%02x,", dev->hum );
    strcat(frame, temp);
    sprintf(temp, "%04x,", dev->a.x );
    strcat(frame, temp);
    sprintf(temp, "%04x,", dev->a.y );
    strcat(frame, temp);
    sprintf(temp, "%04x,", dev->a.z );
    strcat(frame, temp);
    sprintf(temp, "%02,", BatChr_getStatus());
    strcat(frame, temp);

    evSendObj.size = strlen((char *)evSendObj.buf);
}

/* ............................ Effect actions ............................. */
static void 
activateSync(CommMgr *const me, RKH_EVT_T *pe)
{
}

static void
updateStatus(CommMgr *const me, RKH_EVT_T *pe)
{
    me->currStatus = ((GStatusEvt *)pe)->status;
    get_frame(&(me->currStatus), (char *)evSendObj.buf);
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
#if 0
    rkh_pubsub_publish(TopicConnection, RKH_UPCAST(RKH_EVT_T, &evRecvObj),
                                        RKH_UPCAST(RKH_SMA_T, me));
#endif
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

static void 
enWaitSync(CommMgr *const me)
{
#if 0
    RKH_TMR_ONESHOT(&me->syncTmr, RKH_UPCAST(RKH_SMA_T, me), RKH_TIME_SEC(1));
#endif
}

/* ............................. Exit actions .............................. */
static void 
exWaitSync(CommMgr *const me)
{
#if 0
    rkh_tmr_stop(&me->syncTmr);
#endif
}

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

//#/* ------------------------------ End of file ------------------------------ */
