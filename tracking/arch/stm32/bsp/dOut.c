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
#include "dOut.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct
{
    ruint val;
    rui16_t timer;
}DigitalTimerOutput;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static DigitalTimerOutput dOuts[NUM_DOUT_SIGNALS];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
dOut_init(void)
{
    memset(dOuts, 0, sizeof(dOuts));
}

void
dOut_set(ruint out, ruint val, rui16_t tmr)
{
    RKH_SR_ALLOC();

    if(out >= NUM_DOUT_SIGNALS)
        return;

    //printf("dOut[%d]:%d\r\n", out, val);

    RKH_ENTER_CRITICAL_();    
    dOuts[out].val = val != 0 ? 1 : 0;
    dOuts[out].timer = tmr;
    RKH_EXIT_CRITICAL_();    
}

ruint
dOut_get(ruint out)
{
    return dOuts[out].val;
}

void
dOut_process(void)
{
    DigitalTimerOutput *p;
    ruint i;

    p = dOuts;

	for(p=dOuts, i=0; p < &dOuts[NUM_DOUT_SIGNALS]; ++p, ++i)
    {
		if(p->timer > 0 && !(--(p->timer)))
        {
			p->val ^= 1;
            //printf("dOut[%d]:%d\r\n", i, p->val);
        }
    }
}

/* ------------------------------ End of file ------------------------------ */
