/**
 *  \file       modpwr_sim900.c
 *  \brief      Implementation of modpwr abstraction for SIM900 on WINx86.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.06.05  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Bali�a       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "wserial.h"

#include "rkh.h"
#include "rkhplat.h"
#include "bsp.h"
#include "modpwr.h"
#include "mTimeCfg.h"

/* ----------------------------- Local macros ------------------------------ */
#define ModemPwrOn_toggle(t) \
        { \
			RKH_SR_ALLOC(); \
            RKH_ENTER_CRITICAL_(); \
            counter = (t); \
            state = Toggling; \
            RKH_EXIT_CRITICAL_(); \
        }

/* ------------------------------- Constants ------------------------------- */
#define SIM53200_PWR_ON_TIME     (200/(100/MTIME_TIME_TICK))
#define SIM53200_PWR_OFF_TIME    (1000/(100/MTIME_TIME_TICK))

/* ---------------------------- Local data types --------------------------- */
typedef enum ModPwrStates
{
    OnOff,
    Toggling
}ModPwrStates;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static ruint state, counter;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static
void
ModemPwrOn(ruint b)
{
    if(b)
        reset_dtr(GSM_PORT);
    else
        set_dtr(GSM_PORT);
}

/* ---------------------------- Global functions --------------------------- */
#ifdef MODPWR_CTRL_ENABLE

void
modPwr_init(void)
{
    state = OnOff;
}

void
modPwr_ctrl(void)
{
    switch(state)
    {
        case OnOff:
            ModemPwrOn(1);
            break;

        case Toggling:
            ModemPwrOn(0);
            if(counter && (--counter == 0))
            {
                state = OnOff;
            }

            break;
    }
}

void
modPwr_off(void)
{
    ModemPwrOn_toggle(SIM53200_PWR_OFF_TIME);
}

void
modPwr_on(void)
{
    ModemPwrOn_toggle(SIM53200_PWR_ON_TIME);
}

#endif

/* ------------------------------ End of file ------------------------------ */
