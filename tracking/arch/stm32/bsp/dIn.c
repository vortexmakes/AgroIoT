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
 *  DaBa  Dario Baliña       db@vortexmakes.com
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
#include "topics.h"

/* ----------------------------- Local macros ------------------------------ */
#define DEBOUNCE_NSAMPLE    2
#define DEBOUNCE_MASK       ((uint8_t)((1 << DEBOUNCE_NSAMPLE) - 1))
#define DEBOUNCE_CHG        ((uint8_t)((1 << (DEBOUNCE_NSAMPLE - 1)) - 1))
#define DEBOUNCE_NOT_MASK   ((uint8_t)~DEBOUNCE_MASK)
#define DEBOUNCE_NOT_CHG    ((uint8_t)~DEBOUNCE_CHG)

#define PullSelect1(b)      HAL_GPIO_WritePin(PULL_SELECT1_GPIO_Port, \
                                              PULL_SELECT1_Pin, b)

#define PullSelect2(b)      HAL_GPIO_WritePin(PULL_SELECT2_GPIO_Port, \
                                              PULL_SELECT2_Pin, b)

#define Input1()            HAL_GPIO_ReadPin(INPUT1_GPIO_Port, INPUT1_Pin)
#define Input2()            HAL_GPIO_ReadPin(INPUT2_GPIO_Port, INPUT2_Pin)

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
ruint ioChg;

/* ---------------------------- Local variables ---------------------------- */
static unsigned char dIns[NUM_DIN_SIGNALS];
static uint8_t dInsSt[NUM_DIN_SIGNALS];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static
uint8_t gpioRead(dInSignalId pin)
{
    switch(pin)
    {
        case dIn1:
            return Input1();

        case dIn2:
            return Input2();

        default:
            return 0;
    }
}

/* ---------------------------- Global functions --------------------------- */
void
dIn_init(void)
{
    dInSignalId i;

    for(i=0; i < NUM_DIN_SIGNALS; ++i)
    {
        PullSelect1(0);
        PullSelect2(1);

        dIns[i] = 0;
        dInsSt[i] = 0;
    }
}

void
dIn_scan(void)
{
    dInSignalId i;
    uint8_t din;
    IoChgEvt *p;

    for(i=0; i < NUM_DIN_SIGNALS; ++i)
    {
        din = (dIns[i] << 1) | gpioRead(i);
        
        if((dIns[i] == DEBOUNCE_CHG) && (din == DEBOUNCE_MASK) &&
           (dInsSt[i] == 0))
        {
            dInsSt[i] = 1;

            p = RKH_ALLOC_EVT(IoChgEvt, evIoChg, &ioChg);
            p->din |= 1 << i;

            tpIoChg_publish(p, &ioChg);
        }
        else if((dIns[i] == DEBOUNCE_NOT_CHG) && (din == DEBOUNCE_NOT_MASK) &&
                (dInsSt[i] == 1))
        {
            dInsSt[i] = 0;

            p = RKH_ALLOC_EVT(IoChgEvt, evIoChg, &ioChg);
            p->din &= ~(1 << i);

            tpIoChg_publish(p, &ioChg);
        }

        dIns[i] = din;
    }
}

void
dIn_pullSelect(pullSelectId ps, uint8_t value)
{
    switch(ps)
    {
        case pullSelect1:
            PullSelect1(value);
            break;

        case pullSelect2:
            PullSelect2(value);
            break;

        default:
            break;
    }
}

/* ------------------------------ End of file ------------------------------ */
