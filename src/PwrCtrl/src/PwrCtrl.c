/**
 *  \file       PwrCtrl.c
 *  \brief      Implements Power Fail and Shutdown control module
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.03.30  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Darío Baliña  db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "bsp.h"
#include "rkh.h"
#include "Collector.h"
#include "signals.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#ifdef DEBUG
#define reset_now()		__asm volatile	("	bkpt 0x00FF\n" )
#else
#define reset_now()		NVIC_SystemReset()
#endif

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static ruint PwrCtrl;
static enable = 0;
static RKH_ROM_STATIC_EVENT(e_PwrFail, evExtPowerFail);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
PwrCtrl_init(void)
{
    if(bsp_get_ExtPower() == ExtPowerOk)
    {
    	bsp_set_PowerOff(PowerON);
        enable = 1;
    }
    else
    {
    	bsp_set_PowerOff(PowerOFF);
    	enable = 0;
    }
}

void
PwrCtrl_powerFailCheck(void)
{
    if(!enable)
        return;

    if( bsp_get_ExtPower() != ExtPowerOk)
    {
        RKH_SMA_POST_FIFO(collector, &e_PwrFail, &PwrCtrl);
    }
}

void
PwrCtrl_toPowerOff(void)
{
	if(bsp_get_ExtPower() == ExtPowerOk)
	{
		reset_now();
	}
	else
	{
		bsp_set_PowerOff(PowerOFF);
		while(1);   // while until shutdown
	}
}

/* ------------------------------ End of file ------------------------------ */
