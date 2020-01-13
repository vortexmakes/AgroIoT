/**
 *  \file       ConMgrActRequired.c
 *  \brief      Active object's action required interface implementations.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>

#include "rkh.h"
#include "rkhtmr.h"
#include "Config.h"
#include "ConMgr.h"
#include "ConMgrAct.h"
#include "bsp.h"
#include "ConMgrActRequired.h"
#include "ModMgr.h"
#include "ModCmd.h"
#include "signals.h"
#include "events.h"
#include "topic.h"
#include "modpwr.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct ConMgrInternal
{
    ConMgr *pConMgr;
    
    char Imei[IMEI_BUF_SIZE];
    char Oper[OPER_BUF_SIZE];
    char Domain[DOMAIN_BUF_SIZE];
    char Port[PORT_BUF_SIZE];

    RKH_QUEUE_T qDefer;
    RKH_EVT_T *qDefer_sto[SIZEOF_QDEFER];

    SendEvt *psend;
    RKHTmEvt tmEvtRegStatus;
} ConMgrInternal;

typedef struct Apn
{
    char *addr;
    char *usr;
    char *psw;
}Apn;

typedef struct Operator
{
    char *netCode;
    Apn apn;
}Operator;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_ROM_STATIC_EVENT(e_NetConnected, evNetConnected);
static RKH_ROM_STATIC_EVENT(e_NetDisconnected, evNetDisconnected);
static RKH_ROM_STATIC_EVENT(e_Sent,     evSent);
static RKH_ROM_STATIC_EVENT(e_SendFail, evSendFail);
static RKH_ROM_STATIC_EVENT(e_RecvFail, evRecvFail);

ReceivedEvt e_Received;

static Operator operTable[] =
{
    {MOVISTAR_OPERATOR,
     {MOVISTAR_APN_ADDR, MOVISTAR_APN_USER, MOVISTAR_APN_PASS}},
    {CLARO_OPERATOR,
     {CLARO_APN_ADDR, CLARO_APN_USER, CLARO_APN_PASS}},
    {PERSONAL_OPERATOR,
     {PERSONAL_APN_ADDR, PERSONAL_APN_USER, PERSONAL_APN_PASS}},
    {NULL}
};

static Apn *defaultAPN = &(operTable[0].apn);


static ConMgrInternal ConMgrInt;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static Apn *
getAPNbyOper(char *oper)
{
    Operator *pOper;

    for (pOper = &operTable[0]; pOper->netCode != NULL; ++pOper)
    {
        if (strcmp(oper, pOper->netCode) == 0)
        {
            return &(pOper->apn);
        }
    }
    return defaultAPN;
}

/* ---------------------------- Global functions --------------------------- */
void
init(ConMgr *const me)
{
    topic_subscribe(TCPConnection, me);
    topic_subscribe(ModURC, me);

    rkh_queue_init(&ConMgrInt.qDefer, (const void **)ConMgrInt.qDefer_sto, 
                    SIZEOF_QDEFER, CV(0));

    ConMgrInt.pConMgr = me;

    me->imei = ConMgrInt.Imei;
    me->oper = ConMgrInt.Oper;
    me->protocol = CONNECTION_PROT;
    me->domain = ConMgrInt.Domain;
    me->port = ConMgrInt.Port;


    bsp_SIMSelect(MainSIM);
}

void
storeImei(ConMgr *const me, RKH_EVT_T *pe)
{
    ImeiEvt *p;

    (void)me;

    p = RKH_UPCAST(ImeiEvt, pe);
    strcpy(me->imei, p->buf);
}

void
storeOper(ConMgr *const me, RKH_EVT_T *pe)
{
    OperEvt *p;

    p = RKH_UPCAST(OperEvt, pe);
    strcpy(me->oper, p->buf);
}

void
powerOn(void)
{
    modPwr_on();
    ModCmd_init();
}

void
powerOff(void)
{
    modPwr_off();
    bsp_SIMChange();
}

void
modemFound(void)
{
    bsp_GSMModemFound();
}

void
setupAPN(ConMgr *const me)
{
    Apn *apn;

    apn = getAPNbyOper(me->oper);
    ModCmd_setupAPN(apn->addr, apn->usr, apn->psw);
}


void
startRegStatusTimer(ConMgr *const me)
{
	RKH_SET_STATIC_EVENT(&ConMgrInt.tmEvtRegStatus, evRegStatusTout);
	RKH_TMR_INIT(&ConMgrInt.tmEvtRegStatus.tmr, 
                    RKH_UPCAST(RKH_EVT_T, &ConMgrInt.tmEvtRegStatus), NULL);
	RKH_TMR_ONESHOT(&ConMgrInt.tmEvtRegStatus.tmr, RKH_UPCAST(RKH_SMA_T, me), 
                        CHECK_REG_STATUS_PERIOD);
}

void
sendRequest(RKH_EVT_T *pe)
{
    ConMgrInt.psend = RKH_UPCAST(SendEvt, pe);

    ModCmd_sendDataRequest((rui16_t)(ConMgrInt.psend->size));
}

void
sendOk(ConMgr *const me)
{
    topic_publish(TCPConnection, &e_Sent, me);
}

void
sendFail(ConMgr *const me)
{
    topic_publish(TCPConnection, &e_SendFail, me);

    ModCmd_init();
}

void
flushData(void)
{
    ModCmd_sendData(ConMgrInt.psend->buf, ConMgrInt.psend->size);
}

void
readData(void)
{
    RKH_SET_STATIC_EVENT(RKH_UPCAST(RKH_EVT_T, &e_Received), evReceived);
    ModCmd_readData();
}

void
recvOk(ConMgr *const me)
{
    topic_publish(TCPConnection, &e_Received, me);
    bsp_recvOk();
}

void
recvFail(ConMgr *const me)
{
    topic_publish(TCPConnection, &e_RecvFail, me);

    ModCmd_init();
}

void
setSigLevel(ConMgr *const me, RKH_EVT_T *pe)
{
    SigLevelEvt *p;

    p = RKH_UPCAST(SigLevelEvt, pe);
    me->sigLevel = p->value;
}

void
registered(void)
{
    bsp_regStatus(RegisteredSt);
}

void
unregistered(void)
{
    bsp_regStatus(UnregisteredSt);
}

void
rtimeSync(RKH_EVT_T *pe)
{
    rtime_set(&(RKH_UPCAST(LocalTimeEvt, pe)->time));
}

void
reqDefer(RKH_EVT_T *pe)
{
    if (rkh_queue_is_full(&ConMgrInt.qDefer) != RKH_TRUE)
    {
        rkh_sma_defer(&ConMgrInt.qDefer, pe);
    }
}

void
reqRecall(ConMgr *const me)
{
    rkh_sma_recall((RKH_SMA_T *)me, &ConMgrInt.qDefer);
}

void
socketConnected(ConMgr *const me)
{
    topic_publish(TCPConnection, &e_NetConnected, me);

    bsp_netStatus(ConnectedSt);
}

void
socketDisconnected(ConMgr *const me)
{
    topic_publish(TCPConnection, &e_NetDisconnected, me);

    bsp_netStatus(DisconnectedSt);
}

void
stopSMS(void)
{

}

ReceivedEvt *
ConMgr_ReceiveDataGetRef(void)
{
    return &e_Received;
}


char *
ConMgr_Imei(void)
{
    return ConMgrInt.Imei;
}


/* ------------------------------ End of file ------------------------------ */
