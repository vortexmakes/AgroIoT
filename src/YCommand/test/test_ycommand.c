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

#if 1
void
test_smsInvalidKey(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p= "Im:2,321,10;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdInvalidKey, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsUnknown(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:16,123,10;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdUnknown, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);

    p = "Im:000";

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdUnknown, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsWrongLen(void)
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
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);

    /* WrongLen max */
    p = "Im:1,123,123456;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongLen, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverPort);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsWrongFormat(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;

    p = "Im:0,123,255.2552.55.6666;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongFormat, res);
    TEST_ASSERT_EQUAL(YCmdWrongFormat, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsServerIp(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:0,123,255.255.255.255;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdServerIp, res);
    TEST_ASSERT_EQUAL(YCmdServerIp, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("255.255.255.255", yCmd.data.serverIp);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsServerPort(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:1,123,12345;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdServerPort, res);
    TEST_ASSERT_EQUAL(YCmdServerPort, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("12345", yCmd.data.serverIp);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsConnectionTime(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:2,123,123;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdConnectionTime, res);
    TEST_ASSERT_EQUAL(YCmdConnectionTime, yCmd.id);
    TEST_ASSERT_EQUAL(123, yCmd.data.connTime);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsGpsTime(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:3,123,123;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdGpsTime, res);
    TEST_ASSERT_EQUAL(YCmdGpsTime, yCmd.id);
    TEST_ASSERT_EQUAL(123, yCmd.data.updateGPSTime);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsAccLimit(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:4,123,12;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdAccLimit, res);
    TEST_ASSERT_EQUAL(YCmdAccLimit, yCmd.id);
    TEST_ASSERT_EQUAL(12, yCmd.data.accLimit);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsBreakLimit(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:5,123,12;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdBreakLimit, res);
    TEST_ASSERT_EQUAL(YCmdBreakLimit, yCmd.id);
    TEST_ASSERT_EQUAL(12, yCmd.data.brLimit);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsStatus(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:6,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdStatus, res);
    TEST_ASSERT_EQUAL(YCmdStatus, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.status);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsSetOut1(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:7,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut1, res);
    TEST_ASSERT_EQUAL(YCmdSetOut1, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsSetOut2(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:8,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut2, res);
    TEST_ASSERT_EQUAL(YCmdSetOut2, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsSetOut3(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:9,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut3, res);
    TEST_ASSERT_EQUAL(YCmdSetOut3, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsSetOut4(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:10,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut4, res);
    TEST_ASSERT_EQUAL(YCmdSetOut4, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsSetOut5(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:11,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut5, res);
    TEST_ASSERT_EQUAL(YCmdSetOut5, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsSetOut6(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:12,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut6, res);
    TEST_ASSERT_EQUAL(YCmdSetOut6, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsReset(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:13,123,0;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdReset, res);
    TEST_ASSERT_EQUAL(YCmdReset, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsSampleTime(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:14,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSampleTime, res);
    TEST_ASSERT_EQUAL(YCmdSampleTime, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.sampleTime);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_smsDataFormat(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "Im:15,123,0;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdDataFormat, res);
    TEST_ASSERT_EQUAL(YCmdDataFormat, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_gprsIndex(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p= "!3123456789AB,2,123,10;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdConnectionTime, res);
    TEST_ASSERT_EQUAL(YCmdConnectionTime, yCmd.id);
    TEST_ASSERT_EQUAL(10, yCmd.data.connTime);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);

    p= "!3123456789ABCD,2,123,10;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdConnectionTime, res);
    TEST_ASSERT_EQUAL(YCmdConnectionTime, yCmd.id);
    TEST_ASSERT_EQUAL(10, yCmd.data.connTime);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);

    p= "!31,2,123,10;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdConnectionTime, res);
    TEST_ASSERT_EQUAL(YCmdConnectionTime, yCmd.id);
    TEST_ASSERT_EQUAL(10, yCmd.data.connTime);
    TEST_ASSERT_EQUAL_STRING("1", yCmd.index);
}

void
test_gprsInvalidKey(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p= "!3:123456789AB,2,321,10;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdInvalidKey, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_gprsUnknown(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p= "!3123456789AB,16,123,10;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdUnknown, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);

    p= "!3123456789AB,000;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdUnknown, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_gprsWrongLen(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    /* WrongLen min */
    p = "!3123456789AB,1,123,;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongLen, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);

    /* WrongLen max */
    p = "!312345679AB,1,123,123456;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongLen, res);
    TEST_ASSERT_EQUAL(0, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverPort);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_gprsWrongFormat(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;

    p = "!3123456789AB,0,123,255.2552.55.6666;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongFormat, res);
    TEST_ASSERT_EQUAL(YCmdWrongFormat, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("", yCmd.data.serverIp);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
}

void
test_gprsServerIp(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,0,123,255.255.255.255;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdServerIp, res);
    TEST_ASSERT_EQUAL(YCmdServerIp, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("255.255.255.255", yCmd.data.serverIp);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsServerPort(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,1,123,12345;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdServerPort, res);
    TEST_ASSERT_EQUAL(YCmdServerPort, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("12345", yCmd.data.serverIp);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsConnectionTime(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,2,123,123;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdConnectionTime, res);
    TEST_ASSERT_EQUAL(YCmdConnectionTime, yCmd.id);
    TEST_ASSERT_EQUAL(123, yCmd.data.connTime);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsGpsTime(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,3,123,123;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdGpsTime, res);
    TEST_ASSERT_EQUAL(YCmdGpsTime, yCmd.id);
    TEST_ASSERT_EQUAL(123, yCmd.data.updateGPSTime);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsAccLimit(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,4,123,12;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdAccLimit, res);
    TEST_ASSERT_EQUAL(YCmdAccLimit, yCmd.id);
    TEST_ASSERT_EQUAL(12, yCmd.data.accLimit);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsBreakLimit(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,5,123,12;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdBreakLimit, res);
    TEST_ASSERT_EQUAL(YCmdBreakLimit, yCmd.id);
    TEST_ASSERT_EQUAL(12, yCmd.data.brLimit);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsStatus(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,6,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdStatus, res);
    TEST_ASSERT_EQUAL(YCmdStatus, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.status);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsSetOut1(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,7,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut1, res);
    TEST_ASSERT_EQUAL(YCmdSetOut1, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsSetOut2(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,8,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut2, res);
    TEST_ASSERT_EQUAL(YCmdSetOut2, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsSetOut3(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,9,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut3, res);
    TEST_ASSERT_EQUAL(YCmdSetOut3, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsSetOut4(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,10,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut4, res);
    TEST_ASSERT_EQUAL(YCmdSetOut4, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsSetOut5(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,11,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut5, res);
    TEST_ASSERT_EQUAL(YCmdSetOut5, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsSetOut6(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,12,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSetOut6, res);
    TEST_ASSERT_EQUAL(YCmdSetOut6, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.outValue);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsReset(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,13,123,0;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdReset, res);
    TEST_ASSERT_EQUAL(YCmdReset, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsSampleTime(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,14,123,1;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdSampleTime, res);
    TEST_ASSERT_EQUAL(YCmdSampleTime, yCmd.id);
    TEST_ASSERT_EQUAL(1, yCmd.data.sampleTime);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}

void
test_gprsDataFormat(void)
{
    YCommand yCmd;
    YCmd_t res;
    char *p;
    
    p = "!3123456789AB,15,123,0;"; 

    res = YCommand_parse(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdDataFormat, res);
    TEST_ASSERT_EQUAL(YCmdDataFormat, yCmd.id);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
}
#endif
/* ------------------------------ End of file ------------------------------ */
