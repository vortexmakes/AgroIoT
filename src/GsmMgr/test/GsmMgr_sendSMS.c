/**
 *  \file       GsmMgr_sendSMS.c
 *  \brief      Example: Sending Test SMS using GsmMgr interface.
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
#include "GsmMgr_sendSMS.h"
#include "signals.h"
#include "events.h"
#include "topic.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define DESTINATION_PHONE_NUM   "2235493862"

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
static SendSMSEvt SMSEvtobj;
static ruint smsCount;

void
GsmMgr_sendTestSMS(RKH_SMA_T *ao)
{
    ++smsCount;
    RKH_SET_STATIC_EVENT(&SMSEvtobj, evSendSMS);

    strcpy(SMSEvtobj.dest, DESTINATION_PHONE_NUM);

    sprintf(SMSEvtobj.buf, "SMS Test Count: %d", smsCount);
    SMSEvtobj.size = strlen(SMSEvtobj.buf);

    topic_publish(TCPConnection,
                  RKH_UPCAST(RKH_EVT_T, &SMSEvtobj), ao);
}

/* ------------------------------ End of file ------------------------------ */
