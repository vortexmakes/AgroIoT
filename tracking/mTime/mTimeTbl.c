/**
 *  \file       mTimeTble.c
 * 	\bried      mTime timers Table.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.17  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdio.h>
#include "mTime.h"
#include "mTimeCfg.h"
#include "epoch.h"
#include "modpwr.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static void(* const actions_100[])( void ) =
{
#ifdef MODPWR_CTRL_ENABLE
	modPwr_ctrl, 
#endif
    epoch_updateByStep,
    NULL
};

static void(* const actions_1000[])( void ) =
{
	NULL
};

static void(* const actions_10000[])( void ) =
{
	NULL
};

const timerChain_t timerChain[] =
{
	{ MTIME_EPOCH_SCAN_PERIOD, actions_100 },
	{ MTIME_1SEC_SCAN_PERIOD, actions_1000 },
	{ MTIME_10SEC_SCAN_PERIOD, actions_10000 }
};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */





