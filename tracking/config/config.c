/**
 *  \file config.c
 *
 *	This module contains functions for read and write configuration
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.09.06  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "config.h"
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
Config * config_read(void)
{
    /*
     * Wired Configiguration Settings
     * TODO: update / store / recover non volatile configuration
     */
    return &config;
}

/* ------------------------------ End of file ------------------------------ */
