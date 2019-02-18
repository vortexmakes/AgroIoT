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
#include "rkhtypes.h"
#include "Config.h"
#include "settings.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static Config config =
{
    ACLIMIT_DFT,
    BRLIMIT_DFT
};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
Config *
config_read(void)
{
    /*
     * Wired Configiguration Settings
     * TODO: update / store / recover non volatile configuration
     */
    return &config;
}

/* ------------------------------ End of file ------------------------------ */
