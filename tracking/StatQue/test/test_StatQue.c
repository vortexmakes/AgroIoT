/**
 *  \file       test_StatQue.c
 *  \brief      Unit test for status queue's interface.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.18.02  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "StatQue.h"
#include "Mock_ffdata.h"
#include "Mock_ffile.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void 
setUp(void)
{
}

void 
tearDown(void)
{
}

void
test_init(void)
{
	ffile_queue_open_as_random_ExpectAndReturn(FFD0, 
                                               OPEN_FROM_BEGINNING, 0);
    StatQue_init();
}

/* ------------------------------ End of file ------------------------------ */
