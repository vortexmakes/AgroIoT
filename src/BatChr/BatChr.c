/**
 *  \file       BatChr.c
 *  \brief      Implementation of battery charger module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.04.02  LeFr  v1.0.00  ---
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "bsp.h"
#include "rkh.h"
#include "Collector.h"
#include "signals.h"
#include "topic.h"
#include "BatChr.h"

/* ----------------------------- Local macros ------------------------------ */
#ifdef DEBUG
#define reset_now()		__asm volatile	("	bkpt 0x00FF\n" )
#else
#define reset_now()		NVIC_SystemReset()
#endif

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static ruint BatChr;
static BatChrStatus status = -1;
static BatChrEvt e_BatChr;  

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
BatChr_init(void)
{
	bsp_get_ExtPower();
   	bsp_set_battery(Ready);
    status = LINE_BATT;

    RKH_SET_STATIC_EVENT(&e_BatChr, evBatChrStatus);

    RKH_TR_FWK_OBJ(&BatChr);
    RKH_TR_FWK_SIG(evBatChrStatus);
}

void
BatChr_powerFailCheck(void)
{
    switch(status)
    {
        case LINE_BATT:
            if(bsp_get_ExtPower() != ExtPowerOk)
            {
                status = NOLINE_BATT;
                e_BatChr.status = status; 
                topic_publish(Status, RKH_UPCAST(RKH_EVT_T, &e_BatChr), 
                                      RKH_UPCAST(RKH_SMA_T, &BatChr));
            }
            break;

        case NOLINE_BATT:
            if(bsp_get_ExtPower() == ExtPowerOk)
            {
                status = LINE_BATT;
                e_BatChr.status = status; 
                topic_publish(Status, RKH_UPCAST(RKH_EVT_T, &e_BatChr), 
                                      RKH_UPCAST(RKH_SMA_T, &BatChr));
            }
            break;

        default:
            break;
    }
}

void
BatChr_shutDown(void)
{
	if(bsp_get_ExtPower() == ExtPowerOk)
	{
		reset_now();
	}
	else
	{
		bsp_set_battery(Release);
		while(1);   // while until shutdown
	}
}

BatChrStatus
BatChr_getStatus(void)
{
    return status;
}

/* ------------------------------ End of file ------------------------------ */
