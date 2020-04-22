/**
 *  \file       test_ycommandParser.c
 *  \brief      Unit test for Yipies command module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.22.04  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Darío Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include "unity.h"
#include "YCommand.h"
#include "YCommandParser.h"
#include "ssp.h"

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
test_ServerIp(void)
{
    YCommand yCmd;
    YCmdPResult res;
    char *p = "Im:0,123,255.255.255.255;";

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdServerIp, res);
    TEST_ASSERT_EQUAL(YCmdServerIp, yCmd.id);
//    TEST_ASSERT_EQUAL_STRING("255.255.255.255", yCmd.data.serverIp);
}

/* ------------------------------ End of file ------------------------------ */
