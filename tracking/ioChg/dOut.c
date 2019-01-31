/**
 *  \file       dOut.c
 *  \brief      Implementation of Digital Outputs Control HAL.
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
#include "rkh.h"
#include "dOut.h"
#include "bsp.h"

RKH_MODULE_NAME(dOut)

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct
{
    ruint val;
    rui16_t timer;
} DigitalTimerOutput;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static DigitalTimerOutput dOuts[NUM_DOUT_SIGNALS];
static rui32_t dOutStatus;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
void
setStatus(DigOutSignalId out, ruint val)
{
    RKH_ASSERT(out < NUM_DOUT_SIGNALS);

    if (val != 0)
    {
        RKH_BIT_SET_32(dOutStatus, RKH_BIT32(out));
        dOuts[out].val = 1;
    }
    else
    {
        RKH_BIT_CLR_32(dOutStatus, RKH_BIT32(out));
        dOuts[out].val = 0;
    }
    bsp_setDigOut(out, val);
}

/* ---------------------------- Global functions --------------------------- */
void
dOut_init(void)
{
    memset(dOuts, 0, sizeof(dOuts));    /* default values */
    dOutStatus = 0;
}

void
dOut_set(DigOutSignalId out, ruint val, rui16_t tmr)
{
    RKH_SR_ALLOC();

    RKH_ENTER_CRITICAL_();
    setStatus(out, val);
    dOuts[out].timer = tmr;
    RKH_EXIT_CRITICAL_();
}

rui32_t
dOut_get(DigOutSignalId out)
{
    return dOuts[out].val;
}

void
dOut_process(void)
{
    DigitalTimerOutput *out;
    rInt i;

    for (out = dOuts, i = 0; out < &dOuts[NUM_DOUT_SIGNALS]; ++out, ++i)
    {
        if ((out->timer > 0) && !(--(out->timer)))
        {
            setStatus(i, out->val ^ 1);
        }
    }
}

/* ------------------------------ End of file ------------------------------ */
