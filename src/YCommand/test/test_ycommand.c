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
#include "IpValid.h"
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
test_InvalidKey(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p= "Im:2,321,10;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdInvalidKey, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);
}

void
test_Unknown(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:16,123,10;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdUnknown, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);

    p = "Im:000";

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdUnknown, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);
}

void
test_ServerIp(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    /* Success */
    p = "Im:0,123,255.255.255.255;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdServerIp, res);
    TEST_ASSERT_EQUAL(YCmdServerIp, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("255.255.255.255", yCmd.data.serverIp);

    /* WrongLen min */
    p = "Im:0,123,255;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongLen, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);

    /* WrongLen max */
    p = "Im:0,123,2552132142421123212132134;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongLen, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);

    /* WrongFormat */
    p = "Im:0,123,255.2552.55.6666;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongFormat, res);
    TEST_ASSERT_EQUAL(YCmdWrongFormat, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);
}

/* ------------------------------ End of file ------------------------------ */
