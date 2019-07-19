/**
 *  \file       modpwr_sim900.c
 *  \brief      Implementation of modpwr abstraction for SIM900 on STM32.
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
#include "rkh.h"
#include "rkhplat.h"
#include "cubemx.h"
#include "modpwr.h"
#include "mTimeCfg.h"

/* ----------------------------- Local macros ------------------------------ */
#define ModemPowerEnable(b) HAL_GPIO_WritePin(MODEM_PWR_ENABLE_GPIO_Port, \
                                              MODEM_PWR_ENABLE_Pin, b)

#define ModemPwrOn(b)       HAL_GPIO_WritePin(MODEM_PWRON_GPIO_Port, \
                                              MODEM_PWRON_Pin, b)

#define ModemPwrOn_toggle() \
        { \
			RKH_SR_ALLOC(); \
            RKH_ENTER_CRITICAL_(); \
            counter = SIM5320_PWR_OFF_TIME; \
            state = Toggling; \
            RKH_EXIT_CRITICAL_(); \
        }

#define SIM5320_PWR_OFF_TIME     (200/MTIME_MODPWR_SCAN_PERIOD)

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
    ModemPowerEnable(0);
}

void
modPwr_on(void)
{
    ModemPowerEnable(1);

    ModemPwrOn_toggle();
}

#endif

/* ------------------------------ End of file ------------------------------ */
