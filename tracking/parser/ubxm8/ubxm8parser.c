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
#include "signals.h"
#include <stdlib.h>

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static rui8_t ubxm8parser;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */

SSP_CREATE_NORMAL_NODE(rootGpsParser);
SSP_CREATE_BR_TABLE(rootGpsParser)
SSP_END_BR_TABLE


/* ---------------------------- Global functions --------------------------- */
void
ubx8mparser_init(void)
{
    RKH_TR_FWK_ACTOR(&ubxm8parser, "ubxm8parser");
}

/* ------------------------------ End of file ------------------------------ */
