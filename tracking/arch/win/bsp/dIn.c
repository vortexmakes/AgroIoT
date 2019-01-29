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
#include "dIn.h"
#include "mTimeCfg.h"
#include "signals.h"
#include "events.h"
#include "topics.h"

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
/* ---------------------------- Global functions --------------------------- */
void
keyb_dIn_parser(char c)
{
    c = c - '0';

    if (c > NUM_DIN_SIGNALS)
    {
        return;
    }

    dInsKb[c] ^= 1;
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
    unsigned char i;
    InChgEvt *inChgEvt;
    
    inChgEvt = RKH_ALLOC_EVT(InChgEvt, evIoChg, &inChg);
    for (i=0; i < NUM_DIN_SIGNALS; ++i)
    {
        if (dIns[i] != dInsKb[i])
        {
            dIns[i] = dInsKb[i];
            inChgEvt->din &= ~(1 << i);
            inChgEvt->din |= dIns[i] ? (1 << i) : 0;
        }
    }
    dInStatus = inChgEvt->din;
    tpIoChg_publish(inChgEvt, &inChg);
}

ruint
dIn_get(void)
{
    return dInStatus;
}

/* ------------------------------ End of file ------------------------------ */
