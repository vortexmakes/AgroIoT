/**
 *  \file       Config.c
 *  \brief      Implements the configurtion module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.09.06  DaBa  v1.0.00   Initial version
 *  2019.18.02  LeFr  v1.1.00   Reviewed to test it
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina db@vortexmakes.com
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"
#include "Config.h"
#include "settings.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
static const Config cfgDefault =
{
    ACLIMIT_DFT,
    BRLIMIT_DFT,
    1,
    "\"216.75.55.101\"",
    "33499",
    60,
    110,
    8,
    0,
    3
};

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static Config cfgObj;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
Config *
Config_get(void)
{
    return (Config *)&cfgObj;
}

Config *
Config_init(void)
{
    return (Config *)&cfgObj;
}

Config *
config_read(void)
{
    /*
     * Wired Configiguration Settings
     * TODO: update / store / recover non volatile configuration
     */
    return (Config *)&cfgObj;
}

/* ------------------------------ End of file ------------------------------ */
