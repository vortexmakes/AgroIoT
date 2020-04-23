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
test_WrongLen(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    /* WrongLen min */
    p = "Im:1,123,;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongLen, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);

    /* WrongLen max */
    p = "Im:1,123,123456;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongLen, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverPort);
}

void
test_WrongFormat(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;

    p = "Im:0,123,255.2552.55.6666;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongFormat, res);
    TEST_ASSERT_EQUAL(YCmdWrongFormat, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);
}

void
test_ServerIp(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:0,123,255.255.255.255;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdServerIp, res);
    TEST_ASSERT_EQUAL(YCmdServerIp, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("255.255.255.255", yCmd.data.serverIp);

}

void
test_ServerPort(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:1,123,12345;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdServerPort, res);
    TEST_ASSERT_EQUAL(YCmdServerPort, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("12345", yCmd.data.serverIp);

}

void
test_ConnectionTime(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:2,123,123;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdConnectionTime, res);
    TEST_ASSERT_EQUAL(YCmdConnectionTime, yCmd.id);
    TEST_ASSERT_EQUAL(123, yCmd.data.connTime);
}

void
test_GpsTime(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:3,123,123;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdGpsTime, res);
    TEST_ASSERT_EQUAL(YCmdGpsTime, yCmd.id);
    TEST_ASSERT_EQUAL(123, yCmd.data.updateGPSTime);
}

void
test_AccLimit(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:4,123,12;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdAccLimit, res);
    TEST_ASSERT_EQUAL(YCmdAccLimit, yCmd.id);
    TEST_ASSERT_EQUAL(12, yCmd.data.accLimit);
}

void
test_BreakLimit(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:5,123,12;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdBreakLimit, res);
    TEST_ASSERT_EQUAL(YCmdBreakLimit, yCmd.id);
    TEST_ASSERT_EQUAL(12, yCmd.data.brLimit);
}

void
test_Status(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:6,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdStatus, res);
    TEST_ASSERT_EQUAL(YCmdStatus, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.status);
}

void
test_SetOut1(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:7,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut1, res);
    TEST_ASSERT_EQUAL(YCmdSetOut1, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
}

void
test_SetOut2(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:8,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut2, res);
    TEST_ASSERT_EQUAL(YCmdSetOut2, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
}

void
test_SetOut3(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:9,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut3, res);
    TEST_ASSERT_EQUAL(YCmdSetOut3, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
}

void
test_SetOut4(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:10,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut4, res);
    TEST_ASSERT_EQUAL(YCmdSetOut4, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
}

void
test_SetOut5(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:11,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut5, res);
    TEST_ASSERT_EQUAL(YCmdSetOut5, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
}

void
test_SetOut6(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:12,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut6, res);
    TEST_ASSERT_EQUAL(YCmdSetOut6, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
}

void
test_Reset(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:13,123;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdReset, res);
    TEST_ASSERT_EQUAL(YCmdReset, yCmd.id);
}

void
test_SampleTime(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:14,123,123;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSampleTime, res);
    TEST_ASSERT_EQUAL(YCmdSampleTime, yCmd.id);
    TEST_ASSERT_EQUAL(123, yCmd.data.sampleTime);
}

void
test_DataFormat(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:15,123;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdDataFormat, res);
    TEST_ASSERT_EQUAL(YCmdDataFormat, yCmd.id);
}

/* ------------------------------ End of file ------------------------------ */
