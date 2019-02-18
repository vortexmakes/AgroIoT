/**
 *  \file       test_StatQue.c
 *  \brief      Unit test for status queue's adapter.
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
#include "Config.h"
#include "settings.h"

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
test_Get(void)
{
    Config *config;

    config = (Config *)0;
    config = Config_get();

    TEST_ASSERT_NOT_NULL(config);
}

void
test_Init(void)
{
    Config *cfg;

    cfg = (Config *)0;
    Config_init();
    cfg = Config_get();

    TEST_ASSERT_NOT_NULL(cfg);
    TEST_ASSERT_EQUAL(0, cfg->status);
    TEST_ASSERT_EQUAL(ACLIMIT_DFT, cfg->aclimit);
    TEST_ASSERT_EQUAL(BRLIMIT_DFT, cfg->brlimit);
}

/* ------------------------------ End of file ------------------------------ */
