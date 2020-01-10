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
#include "rkh.h"
#include "rkhtmr.h"
#include "ConMgr.h"
#include "ConMgrAct.h"
#include "bsp.h"
#include "ConMgrActRequired.h"
#include "ModCmd.h"
#include "signals.h"
#include "events.h"
#include "topic.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define SIZEOF_QDEFER   1

/* ---------------------------- Local data types --------------------------- */
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
static RKH_ROM_STATIC_EVENT(e_SendFail, evSendFail);

static RKH_QUEUE_T qDefer;
static RKH_EVT_T *qDefer_sto[SIZEOF_QDEFER];

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

    rkh_queue_init(&qDefer, (const void **)qDefer_sto, SIZEOF_QDEFER,
                   CV(0));

    Config_getConnectionDomain(me->Domain);
    Config_getConnectionPort(me->Port);
}

void
setupApn(ConMgr *const me)
{
    Apn *apn;

    apn = getAPNbyOper(me->Oper);
    ModCmd_setupAPN(apn->addr, apn->usr, apn->psw);
}

void
sendFail(ConMgr *const me)
{
    topic_publish(TCPConnection, &e_SendFail, me);

    ModCmd_init();
}

void
flushData(ConMgr *const me)
{
    ModCmd_sendData(me->psend->buf, me->psend->size);
}

void
ConMgr_defer(RKH_EVT_T *pe)
{
    if (rkh_queue_is_full(&qDefer) != RKH_TRUE)
    {
        rkh_sma_defer(&qDefer, pe);
    }
}

/* ------------------------------ End of file ------------------------------ */
