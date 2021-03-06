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
#include "bsp.h"
#include "rkhfwk_pubsub.h"
#include "dIn.h"
#include "ioChg.h"
#include "mTimeCfg.h"
#include "signals.h"
#include "events.h"
#include "topic.h"
#include "Config.h"

/* ----------------------------- Local macros ------------------------------ */
#define DEBOUNCE_NSAMPLE    2
#define DEBOUNCE_MASK       ((uint8_t)((1 << DEBOUNCE_NSAMPLE) - 1))
#define DEBOUNCE_CHG        ((uint8_t)((1 << (DEBOUNCE_NSAMPLE - 1)) - 1))
#define DEBOUNCE_NOT_MASK   ((uint8_t) ~DEBOUNCE_MASK)
#define DEBOUNCE_NOT_CHG    ((uint8_t) ~DEBOUNCE_CHG)
#define DUMMY_MASK          0xFC

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
ruint inChg;

/* ---------------------------- Local variables ---------------------------- */
static unsigned char dIns[NUM_DIN_SIGNALS];
static uint8_t dInsSt[NUM_DIN_SIGNALS];

/* ----------------------- Local function prototypes ----------------------- */
static DigIn assembleDin(void);

/* ---------------------------- Local functions ---------------------------- */
static
DigIn
assembleDin(void)
{
    DigInSignalId i;
    DigIn status;

    status = Config_getDigInPolarity() == ActiveHigh ? 0 : DUMMY_MASK;

    for (i = 0; i < NUM_DIN_SIGNALS; ++i)
    {
        status |= dInsSt[i] << i; 
    }

    return status;
}

/* ---------------------------- Global functions --------------------------- */
void
dIn_init(void)
{
    DigInSignalId i;
    uint8_t pullSelect, initState;
    unsigned char initFilter;

    if( Config_getDigInPolarity() == ActiveHigh)
    {
        pullSelect = 0;
        initState = 0;
        initFilter = 0;
    }
    else
    {
        pullSelect = 1;
        initState = 1;
        initFilter = 0xFF;
    }

    bsp_DigInPullSelect(pullSelect1, pullSelect);
    bsp_DigInPullSelect(pullSelect2, pullSelect);

    for (i = 0; i < NUM_DIN_SIGNALS; ++i)
    {
        dIns[i] = initFilter;
        dInsSt[i] = initState;
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

            p->status = assembleDin();

            topic_publish(Status, RKH_UPCAST(RKH_EVT_T, p),
                                  RKH_UPCAST(RKH_SMA_T, &inChg));
        }
        else if ((dIns[i] == DEBOUNCE_NOT_CHG) && (din == DEBOUNCE_NOT_MASK) &&
                 (dInsSt[i] == 1))
        {
            dInsSt[i] = 0;

            p = RKH_ALLOC_EVT(DigInChangedEvt, evDigInChanged, &inChg);

            p->status = assembleDin();

            topic_publish(Status, RKH_UPCAST(RKH_EVT_T, p),
                                  RKH_UPCAST(RKH_SMA_T, &inChg));
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
