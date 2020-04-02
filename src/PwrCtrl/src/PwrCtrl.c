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
PwrCtrl_init(PwrCtrl_t state)
{
    bsp_set_PowerOff(state);
    enable = 1;
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
    bsp_set_PowerOff(PowerOFF);
    while(1);   // while until shutdown
}

/* ------------------------------ End of file ------------------------------ */
