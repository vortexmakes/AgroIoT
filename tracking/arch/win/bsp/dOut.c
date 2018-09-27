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
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static rui8_t dOuts[NUM_DOUT_SIGNALS];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
dOut_init(void)
{
    memset(dOuts, 0, sizeof(dOuts));
}

void
dOut_set(rui8_t out, rui8_t val)
{
    printf("dOut[%d]:%d", out, val);
    dOuts[out] = val;
}

rui8_t
dOut_get(rui8_t out)
{
    return dOuts[out];
}

/* ------------------------------ End of file ------------------------------ */
