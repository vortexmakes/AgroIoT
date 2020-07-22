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
#define STACK_USAGE_WATERMARK   80  // Percent of total stack size
#define STACK_FILL_PATTERN      0xA5A5A5A5

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
extern const volatile unsigned int _estack;
extern const volatile unsigned int _Min_Stack_Size;

/* ---------------------------- Local variables ---------------------------- */
unsigned int *psstack = (unsigned int *)(&_estack);
unsigned int StackSize = (unsigned int)(&_Min_Stack_Size);

unsigned int *pstart, *pend;

unsigned int usage;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
stackMonitor_init(void)
{
    unsigned int *p;

    pstart = psstack - (StackSize/sizeof(int));

    /* 'p' is allocated at most lower stack used address  */
    /* then unused stack memory is filled with pattern */
    /* until this address the address of 'p' &p */
	for(p = pstart; p < (unsigned int *)(&p); ++p)
	{                             
		*p = STACK_FILL_PATTERN;
	}

    pend = p;
}

void
stackMonitor_check(void)
{
	unsigned int *p;
	RKH_SR_ALLOC();

    for(p = pstart; p < pend; ++p)
    {
        if(*p != STACK_FILL_PATTERN)
            break;
    }

    RKH_ENTER_CRITICAL_();
    usage = (psstack - p) * sizeof(int);
    usage = usage * 100 / StackSize;
    RKH_EXIT_CRITICAL_();

    RKH_ASSERT(usage < STACK_USAGE_WATERMARK);
}

unsigned int
stackMonitor_getUsage(void)
{
    return usage;
}

/* ------------------------------ File footer ------------------------------ */
