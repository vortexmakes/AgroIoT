/**
 *  \file       dIn.c
 *  \brief      Implementation of Digital Inputs HAL and change detection.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.17  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"
#include "rkhfwk_pubsub.h"
#include "rkhfwk_dynevt.h"
#include "rkhassert.h"
#include "dIn.h"
#include "mTimeCfg.h"
#include "signals.h"
#include "events.h"
#include "topic.h"

RKH_MODULE_NAME(dIn)

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
ruint inChg;    /* Identifies this module */

/* ---------------------------- Local variables ---------------------------- */
static unsigned char dIns[NUM_DIN_SIGNALS];
static unsigned char dInsKb[NUM_DIN_SIGNALS];
static DigIn dInStatus;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static unsigned char
getInStatus(rInt nDigIn)
{
    RKH_ASSERT(nDigIn < NUM_DIN_SIGNALS);
    return dInsKb[nDigIn];
}

/* ---------------------------- Global functions --------------------------- */
void
keyb_dIn_parser(char c)
{
    c = c - '0';

    if ((c >= 0) && (c < NUM_DIN_SIGNALS))
    {
        dInsKb[c] ^= 1;
    }
}

void
dIn_init(void)
{
    memset(dIns, 0, sizeof(dIns));
    memset(dInsKb, 0, sizeof(dIns));
    dInStatus = 0;
}

void
dIn_scan(void)
{
    rInt i;
    InChgEvt *inChgEvt;
    DigIn dInCurrStatus;

    for (dInCurrStatus = dInStatus, i = 0; i < NUM_DIN_SIGNALS; ++i)
    {
        if (dIns[i] != getInStatus(i))
        {
            dIns[i] = getInStatus(i);
            if (dIns[i] == 1)
            {
                RKH_BIT_SET_08(dInCurrStatus, RKH_BIT08(i));
            }
            else
            {
                RKH_BIT_CLR_08(dInCurrStatus, RKH_BIT08(i));
            }
        }
    }
    if (dInCurrStatus != dInStatus)
    {
        inChgEvt = RKH_ALLOC_EVT(InChgEvt, evIoChg, &inChg);
        dInStatus = inChgEvt->din = dInCurrStatus;
        rkh_pubsub_publish(TopicStatus, RKH_UPCAST(RKH_EVT_T, inChgEvt),
                                        RKH_UPCAST(RKH_SMA_T, &inChg));
    }
}

rui32_t
dIn_getAll(void)
{
    return dInStatus;
}

/* ------------------------------ End of file ------------------------------ */
