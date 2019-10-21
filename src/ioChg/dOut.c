/**
 *  \file       dout.c
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
#include <stdio.h>
#include "rkh.h"
#include "rkhfwk_pubsub.h"
#include "dOut.h"
#include "cubemx.h"
#include "bsp.h"
#include "events.h"
#include "signals.h"
#include "topic.h"

RKH_MODULE_NAME(dOut)

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
enum
{
    OnInit, OnSet
};

/* ---------------------------- Local data types --------------------------- */
typedef struct
{
    ruint val;
    rui16_t timer;
}DigitalTimerOutput;

/* ---------------------------- Global variables --------------------------- */
ruint outChg;    /* Identifies this module */

/* ---------------------------- Local variables ---------------------------- */
static DigitalTimerOutput dOuts[NUM_DOUT_SIGNALS];
static DigOut dOutStatus;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
setStatus(DigOutSignalId out, ruint val, int context)
{
    RKH_ASSERT(out < NUM_DOUT_SIGNALS);

    if (val != 0)
    {
        RKH_BIT_SET_08(dOutStatus, RKH_BIT08(1 << out));
        dOuts[out].val = 1;
    }
    else
    {
        RKH_BIT_CLR_08(dOutStatus, RKH_BIT08(1 << out));
        dOuts[out].val = 0;
    }

    bsp_setDigOut(out, val);
    
    if (context == OnSet)
    {
        OutChgEvt *outChgObj = RKH_ALLOC_EVT(OutChgEvt, evOutChg, &outChg);
        outChgObj->dout = dOutStatus;
        rkh_pubsub_publish(deviceStatus, RKH_UPCAST(RKH_EVT_T, outChgObj),
                                        RKH_UPCAST(RKH_SMA_T, &outChg));
    }
}

/* ---------------------------- Global functions --------------------------- */
void
dOut_init(void)
{
    DigitalTimerOutput *out;
    rInt i;

    for (out = dOuts, i = 0; out < &dOuts[NUM_DOUT_SIGNALS]; ++out, ++i)
    {
        setStatus(i, 0, OnInit);
        out->timer = 0;
    }
}

void
dOut_set(DigOutSignalId out, ruint val, rui16_t tmr)
{
    RKH_SR_ALLOC();

    if(out >= NUM_DOUT_SIGNALS)
        return;

    RKH_ENTER_CRITICAL_();    
    dOuts[out].val = val != 0 ? 1 : 0;
    dOuts[out].timer = tmr;
    RKH_EXIT_CRITICAL_();    
}

ruint
dOut_get(DigOutSignalId out)
{
    return dOuts[out].val;
}

void
dOut_process(void)
{
    DigitalTimerOutput *out;
    DigOutSignalId i;

	for(out = dOuts, i=0; out < &dOuts[NUM_DOUT_SIGNALS]; ++out, ++i)
    {
		if(out->timer > 0 && !(--(out->timer)))
        {
            setStatus(i, out->val ^ 1, OnSet);
        }
    }
}

/* ------------------------------ End of file ------------------------------ */
