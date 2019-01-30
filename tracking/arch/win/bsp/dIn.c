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
#include "topics.h"

RKH_MODULE_NAME(dIn)

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
ruint inChg;    /* Identifies this module */

/* ---------------------------- Local variables ---------------------------- */
static unsigned char dIns[NUM_DIN_SIGNALS];
static unsigned char dInsKb[NUM_DIN_SIGNALS];
static ruint dInStatus;

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
    ruint dInCurrStatus;

    for (dInCurrStatus = dInStatus, i = 0; i < NUM_DIN_SIGNALS; ++i)
    {
        if (dIns[i] != getInStatus(i))
        {
            dIns[i] = getInStatus(i);
            dInCurrStatus &= ~(1 << i);
            dInCurrStatus |= dIns[i] ? (1 << i) : 0;
        }
    }
    if (dInCurrStatus != dInStatus)
    {
        inChgEvt = RKH_ALLOC_EVT(InChgEvt, evIoChg, &inChg);
        dInStatus = inChgEvt->din = dInCurrStatus;
        tpIoChg_publish(inChgEvt, &inChg);
    }
}

ruint
dIn_get(void)
{
    return dInStatus;
}

/* ------------------------------ End of file ------------------------------ */
