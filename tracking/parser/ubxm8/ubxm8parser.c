/**
 *  \file       ubxm8parser.c
 *  \brief      ...
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.09.07  Daba  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "ubxm8parser.h"
#include "geoMgr.h"
#include "signals.h"
#include "events.h"
#include <stdlib.h>

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static rui8_t ubxm8parser;
static RMC_t rmcCurrent, rmcLast;
static RmcEvt rmcEvt;

/* ----------------------- Local function prototypes ----------------------- */
static void rmcUpdate(unsigned char pos);

/* ---------------------------- Local functions ---------------------------- */

SSP_CREATE_NORMAL_NODE(rootGpsParser);
SSP_CREATE_BR_TABLE(rootGpsParser)
	SSPBR("$GNRMC,",      rmcUpdate,     &rootGpsParser),
SSP_END_BR_TABLE

static void
rmcUpdate(unsigned char pos)
{
    (void)pos;

    rmcLast = rmcCurrent;

    rmcEvt.p = &rmcLast;

    RKH_SET_STATIC_EVENT(RKH_UPCAST(RKH_EVT_T, &rmcEvt), evRMC);

    RKH_SMA_POST_FIFO(geoMgr, RKH_UPCAST(RKH_EVT_T, &rmcEvt), &ubxm8parser);
}


/* ---------------------------- Global functions --------------------------- */
void
ubx8mparser_init(void)
{
    RKH_TR_FWK_ACTOR(&ubxm8parser, "ubxm8parser");
}

/* ------------------------------ End of file ------------------------------ */
