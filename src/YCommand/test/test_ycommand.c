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
#include "YCommandFormat.h"
#include "YCommandExec.h"
#include "IpValid.h"
#include "ssp.h"
#include "Mock_Config.h"
#include "Mock_dOut.h"

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
    YCmdRes res;
    char *p;
    
    p= "Im:2,321,10;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdInvalidKey, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_smsUnknown(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "Im:16,123,10;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdUnknown, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);

    p = "Im:000";

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdUnknown, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_smsWrongLen(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    /* WrongLen min */
    p = "Im:1,123,;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongLen, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);

    /* WrongLen max */
    p = "Im:1,123,123456;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongLen, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_smsWrongFormat(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;

    p = "Im:0,123,255.2552.55.6666;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongFormat, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_smsServerIp(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "Im:0,123,255.255.255.255;"; 

    Config_setConnectionDomain_Expect("255.255.255.255");
           
    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_smsServerPort(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "Im:1,123,12345;"; 

    Config_setConnectionPort_Expect("12345");

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_smsConnectionTime(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "Im:2,123,123;"; 

    Config_setConnTime_Expect(123);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_smsGpsTime(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "Im:3,123,123;"; 

    Config_setUpdateGPSTime_Expect(123);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_smsAccLimit(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "Im:4,123,12;"; 

    Config_setAccLimit_Expect(12);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_smsBreakLimit(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "Im:5,123,12;"; 

    Config_setBrLimit_Expect(12);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_smsStatus(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "Im:6,123,1;"; 
    
    Config_setDefault_Expect(1);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(1, yCmd.reset);
}

void
test_smsSetOut1(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "Im:7,123,1;"; 

    Config_setDftDigOut_Expect(1);
    dOut_set_Expect(dOut1, 1, 1);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_smsSetOut2(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "Im:8,123,1;"; 

    Config_setDftDigOut_Expect(1);
    dOut_set_Expect(dOut2, 1, 1);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_smsReset(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "Im:13,123,0;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(1, yCmd.reset);
}

void
test_smsSampleTime(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "Im:14,123,1;"; 
    
    Config_setMapTimeOnRunning_Expect(1);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_smsDataFormat(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "Im:15,123,0;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(1, yCmd.reset);
}

void
test_ack(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p= "!2|"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YAck, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsIndex(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p= "!3123456789AB,2,123,10;"; 

    Config_setConnTime_Expect(10);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);

    p= "!3123456789ABCD,2,123,10;"; 

    Config_setConnTime_Expect(10);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);

    p= "!31,2,123,10;"; 

    Config_setConnTime_Expect(10);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("1", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsInvalidKey(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p= "!3:123456789AB,2,321,10;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdInvalidKey, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsUnknown(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p= "!3123456789AB,16,123,10;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdUnknown, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);

    p= "!3123456789AB,000;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdUnknown, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsWrongLen(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    /* WrongLen min */
    p = "!3123456789AB,1,123,;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongLen, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);

    /* WrongLen max */
    p = "!312345679AB,1,123,123456;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongLen, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsWrongFormat(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;

    p = "!3123456789AB,0,123,255.2552.55.6666;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdWrongFormat, res);
    TEST_ASSERT_EQUAL_STRING("", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsServerIp(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "!3123456789AB,0,123,255.255.255.255;"; 

    Config_setConnectionDomain_Expect("255.255.255.255");

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsServerPort(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "!3123456789AB,1,123,12345;"; 

    Config_setConnectionPort_Expect("12345");

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsConnectionTime(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "!3123456789AB,2,123,123;"; 

    Config_setConnTime_Expect(123);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsGpsTime(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "!3123456789AB,3,123,123;"; 

    Config_setUpdateGPSTime_Expect(123);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsAccLimit(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "!3123456789AB,4,123,12;"; 

    Config_setAccLimit_Expect(12);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsBreakLimit(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "!3123456789AB,5,123,12;"; 

    Config_setBrLimit_Expect(12);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsStatus(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "!3123456789AB,6,123,1;"; 

    Config_setDefault_Expect(1);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(1, yCmd.reset);
}

void
test_gprsSetOut1(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "!3123456789AB,7,123,1;"; 

    Config_setDftDigOut_Expect(1);
    dOut_set_Expect(dOut1, 1, 1);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsSetOut2(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "!3123456789AB,8,123,1;"; 

    Config_setDftDigOut_Expect(1);
    dOut_set_Expect(dOut2, 1, 1);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsReset(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "!3123456789AB,13,123,0;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(1, yCmd.reset);
}

void
test_gprsSampleTime(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "!3123456789AB,14,123,1;"; 

    Config_setMapTimeOnRunning_Expect(1);

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(0, yCmd.reset);
}

void
test_gprsDataFormat(void)
{
    YCommand yCmd;
    YCmdRes res;
    char *p;
    
    p = "!3123456789AB,15,123,0;"; 

    res = YCommand_parseAndExec(&yCmd, p, strlen(p));

    TEST_ASSERT_EQUAL(YCmdOk, res);
    TEST_ASSERT_EQUAL_STRING("123456789AB", yCmd.index);
    TEST_ASSERT_EQUAL(1, yCmd.reset);
}
#endif
/* ------------------------------ End of file ------------------------------ */
