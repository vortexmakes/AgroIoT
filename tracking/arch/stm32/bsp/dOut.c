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
#include "cubemx.h"

/* ----------------------------- Local macros ------------------------------ */
#define Output1(b)      HAL_GPIO_WritePin(OUTPUT1_GPIO_Port, OUTPUT1_Pin, b)
#define Output2(b)      HAL_GPIO_WritePin(OUTPUT2_GPIO_Port, OUTPUT2_Pin, b)

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
static
void gpioWrite(DigOutSignalId pin, uint8_t value)
{
    switch(pin)
    {
        case dOut1:
            return Output1(value);

        case dOut2:
            return Output2(value);

        default:
            return;
    }
}

/* ---------------------------- Global functions --------------------------- */
void
dOut_init(void)
{
    memset(dOuts, 0, sizeof(dOuts));
    Output1(0);
    Output2(0);
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
    DigitalTimerOutput *p;
    DigOutSignalId i;

    p = dOuts;

	for(p=dOuts, i=0; p < &dOuts[NUM_DOUT_SIGNALS]; ++p, ++i)
    {
		if(p->timer > 0 && !(--(p->timer)))
        {
			p->val ^= 1;
            gpioWrite(i, p->val);
        }
    }
}

/* ------------------------------ End of file ------------------------------ */
