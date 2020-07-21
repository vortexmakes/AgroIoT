/**
 *  \file       stackMonitor.c
 *  \brief      Stack Usage Level Monitor for STM32
 *
 *  \ingroup    bsp
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.21.07  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */
/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdio.h>
#include "rkh.h"
#include "bsp.h"

RKH_THIS_MODULE

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
extern const volatile unsigned int _estack;
extern const volatile unsigned int _Min_Stack_Size;

/* ---------------------------- Local variables ---------------------------- */
unsigned int *psstack = &_estack;
unsigned int StackSize = &_Min_Stack_Size;

unsigned int *pstart, *pend;

unsigned int usage;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
stackMonitor_init(void)
{
    unsigned int *p;

	for(p = psstack - (StackSize/4); p < &p; ++p) //psstack - (0x50/4); ++p)
	{
		*p = 0xAA55AA55;
	}

    pstart = psstack - (StackSize/4);
    pend = p;
}

void
stackMonitor_check(void)
{
    unsigned int *p;

    for(p = pstart; p < pend; ++p)
    {
        if(*p != 0xAA55AA55)
            break;
    }

    usage = psstack - p;

    RKH_ASSERT(usage < (StackSize - 0x100));
}

unsigned int
stackMonitor_getUsage(void)
{
    return usage;
}

/* ------------------------------ File footer ------------------------------ */
