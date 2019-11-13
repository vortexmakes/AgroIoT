/**
 *  \file       dIn.c
 *  \brief      Implementation of Digital Inputs HAL and change detection.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.02.08  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "cubemx.h"
#include "rkhfwk_pubsub.h"
#include "dIn.h"
#include "ioChg.h"
#include "mTimeCfg.h"
#include "signals.h"
#include "events.h"
#include "topic.h"

/* ----------------------------- Local macros ------------------------------ */
#define DEBOUNCE_NSAMPLE    2
#define DEBOUNCE_MASK       ((uint8_t)((1 << DEBOUNCE_NSAMPLE) - 1))
#define DEBOUNCE_CHG        ((uint8_t)((1 << (DEBOUNCE_NSAMPLE - 1)) - 1))
#define DEBOUNCE_NOT_MASK   ((uint8_t) ~DEBOUNCE_MASK)
#define DEBOUNCE_NOT_CHG    ((uint8_t) ~DEBOUNCE_CHG)

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
ruint inChg;

/* ---------------------------- Local variables ---------------------------- */
static unsigned char dIns[NUM_DIN_SIGNALS];
static uint8_t dInsSt[NUM_DIN_SIGNALS];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
dIn_init(void)
{
    DigInSignalId i;

    bsp_DigInPullSelect(pullSelect1, 0);
    bsp_DigInPullSelect(pullSelect2, 0);

    for (i = 0; i < NUM_DIN_SIGNALS; ++i)
    {
        dIns[i] = 0;
        dInsSt[i] = 0;
    }
}

void
dIn_scan(void)
{
    DigInSignalId i;
    uint8_t din;
    DigInChangedEvt *p;

    for (i = 0; i < NUM_DIN_SIGNALS; ++i)
    {
        din = (dIns[i] << 1) | bsp_getDigIn(i);

        if ((dIns[i] == DEBOUNCE_CHG) && (din == DEBOUNCE_MASK) &&
            (dInsSt[i] == 0))
        {
            dInsSt[i] = 1;

            p = RKH_ALLOC_EVT(DigInChangedEvt, evDigInChanged, &inChg);
            p->din |= 1 << i;

            topic_publish(status, p, &inChg);
        }
        else if ((dIns[i] == DEBOUNCE_NOT_CHG) && (din == DEBOUNCE_NOT_MASK) &&
                 (dInsSt[i] == 1))
        {
            dInsSt[i] = 0;

            p = RKH_ALLOC_EVT(DigInChangedEvt, evDigInChanged, &inChg);
            p->din &= ~(1 << i);

            topic_publish(status, p, &inChg);
        }

        dIns[i] = din;
    }
}

void
dIn_pullSelect(DigInPullSelectId ps, uint8_t val)
{
    bsp_DigInPullSelect(ps, val);
}

/* ------------------------------ End of file ------------------------------ */
