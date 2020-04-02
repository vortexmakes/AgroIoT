/**
 *  \file       test_yframe.c
 *  \brief      Unit test for Yipies frame module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.02  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include "unity.h"
#include "YFrame.h"
#include "Mock_cbox.h"
#include "Mock_Geo.h"
#include "Mock_BatChr.h"
#include "Mock_GsmMgr.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
static GStatus status0 =
{
    {
        "185124", "A", "37.8402883", "-", "057.6884350", "-", "0.078",
        "", "310119"
    },
    {1, 1, {0, 0, 1}, {0xdddd, 0xffff, 0xffff}, 0},
    {0xff, 0x3f},
    3
};

static const char singleFrame[] =
    "!0|19355826018345180,185124,-37.8402883,-057.6884350,0.078,,310119,3FFF,0000,00,00,DDDD,FFFF,FFFF,3";

static const char multipleFrame[] =
    "!1|0002|355826018345180|1b,185124,-37.8402883,-057.6884350,0.078,,310119,3FFF,0000,00,00,DDDD,FFFF,FFFF,3|1b,185124,-37.8402883,-057.6884350,0.078,,310119,3FFF,0000,00,00,DDDD,FFFF,FFFF,3#";

static const char corruptSingleFrame[] =
    "!0|19355826018345180,,,,,,,FFFF,FFFF,FF,FF,FFFF,FFFF,FFFF,FF";

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static GStatus status;
static char buf[256];
static int size;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
errorHandler(GeoErrorCode errCode)
{
    TEST_ASSERT_EQUAL(INDEX_OUT_OF_RANGE, errCode);
}

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
test_MakeInvalidArgs(void)
{
    size = YFrame_data(&status, (char *)0, YFRAME_SGP_TYPE);
    TEST_ASSERT_EQUAL(0, size);

    size = YFrame_data((GStatus *)0, buf, YFRAME_SGP_TYPE);
    TEST_ASSERT_EQUAL(0, size);
}

void
test_GetFlagsInvalidArgs(void)
{
    rui8_t flags;
    rInt err;

    flags = 0xad;
    err = YFrame_getFlags((GStatus *)0, &flags, 0);
    TEST_ASSERT_EQUAL(1, err);
    TEST_ASSERT_EQUAL(0xad, flags);

    err = YFrame_getFlags(&status, (rui8_t *)0, 0);
    TEST_ASSERT_EQUAL(1, err);
}

void
test_ValidGetFlags(void)
{
    rui8_t flags;
    rInt err;

    flags = 0;
    Geo_isValid_ExpectAndReturn(&(status0.position), 1);
    cbox_isMoving_ExpectAndReturn(&(status0.devData), 0);
    BatChr_getStatus_ExpectAndReturn(NOLINE_BATT);

    err = YFrame_getFlags((GStatus *)&status0, &flags, YFRAME_SGP_TYPE);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL_HEX(0x19, flags);
}

void
test_MakeSingleHeader(void)
{
    Geo_isValid_ExpectAndReturn(&(status0.position), 1);
    cbox_isMoving_ExpectAndReturn(&(status0.devData), 0);
    BatChr_getStatus_ExpectAndReturn(NOLINE_BATT);
    GsmMgr_getImei_ExpectAndReturn("355826018345180");

    size = YFrame_header(&status0, buf, 0, YFRAME_SGP_TYPE);
    TEST_ASSERT_EQUAL_STRING("!0|19355826018345180,", buf);
}

void
test_MakeSingleFrame(void)
{
    int expLen;

    expLen = strlen(singleFrame);
    Geo_isValid_ExpectAndReturn(&(status0.position), 1);
    cbox_isMoving_ExpectAndReturn(&(status0.devData), 0);
    BatChr_getStatus_ExpectAndReturn(NOLINE_BATT);
    GsmMgr_getImei_ExpectAndReturn("355826018345180");
    Geo_getUtc_ExpectAndReturn(&status0.position, status0.position.utc);
    Geo_getLatInd_ExpectAndReturn(&status0.position, status0.position.latInd);
    Geo_getLatitude_ExpectAndReturn(&status0.position, 
                                    status0.position.latitude);
    Geo_getLongInd_ExpectAndReturn(&status0.position, status0.position.longInd);
    Geo_getLongitude_ExpectAndReturn(&status0.position, 
                                     status0.position.longitude);
    Geo_getSpeed_ExpectAndReturn(&status0.position, status0.position.speed);
    Geo_getCourse_ExpectAndReturn(&status0.position, status0.position.course);
    Geo_getDate_ExpectAndReturn(&status0.position, status0.position.date);

    size = YFrame_header(&status0, buf, 0, YFRAME_SGP_TYPE);
    TEST_ASSERT_EQUAL_STRING("!0|19355826018345180,", buf);
    TEST_ASSERT_TRUE(size != 0);

    size += YFrame_data(&status0, &buf[size], YFRAME_SGP_TYPE);
    TEST_ASSERT_EQUAL_STRING(singleFrame, buf);
    TEST_ASSERT_EQUAL(expLen, size);
}

void
test_MakeMultipleHeader(void)
{
    GsmMgr_getImei_ExpectAndReturn("355826018345180");

    (ruint)YFrame_header(&status0, buf, 2, YFRAME_MGP_TYPE);
    TEST_ASSERT_EQUAL_STRING("!1|0002|355826018345180", buf);
}

void
test_MakeMultipleFrame(void)
{
    int expLen, i;

    expLen = strlen(multipleFrame);
    GsmMgr_getImei_ExpectAndReturn("355826018345180");

    size = YFrame_header(&status0, buf, 2, YFRAME_MGP_TYPE);
    TEST_ASSERT_EQUAL_STRING("!1|0002|355826018345180", buf);
    for (i = 0; i < 2; ++i)
    {
        Geo_isValid_ExpectAndReturn(&(status0.position), 1);
        cbox_isMoving_ExpectAndReturn(&(status0.devData), 0);
        BatChr_getStatus_ExpectAndReturn(NOLINE_BATT);
        Geo_getUtc_ExpectAndReturn(&status0.position, status0.position.utc);
        Geo_getLatInd_ExpectAndReturn(&status0.position, 
                                      status0.position.latInd);
        Geo_getLatitude_ExpectAndReturn(&status0.position, 
                                        status0.position.latitude);
        Geo_getLongInd_ExpectAndReturn(&status0.position, 
                                       status0.position.longInd);
        Geo_getLongitude_ExpectAndReturn(&status0.position, 
                                         status0.position.longitude);
        Geo_getSpeed_ExpectAndReturn(&status0.position, 
                                     status0.position.speed);
        Geo_getCourse_ExpectAndReturn(&status0.position, 
                                      status0.position.course);
        Geo_getDate_ExpectAndReturn(&status0.position, status0.position.date);
        size += YFrame_data(&status0, &buf[size], YFRAME_MGP_TYPE);
    }
    size += YFrame_multipleTail(&buf[size]);
    TEST_ASSERT_EQUAL_STRING(multipleFrame, buf);
    TEST_ASSERT_EQUAL(expLen, size);
}

void
test_CheckResponseAck(void)
{
    TypeOfResp res;

    strcpy(buf, "!2|");
    res = YFrame_parse(buf);
    TEST_ASSERT_EQUAL(TypeOfRespAck, res);

    strcpy(buf, "!3|");
    res = YFrame_parse(buf);
    TEST_ASSERT_EQUAL(TypeOfRespUnknown, res);

    strcpy(buf, "!2,");
    res = YFrame_parse(buf);
    TEST_ASSERT_EQUAL(TypeOfRespUnknown, res);

    strcpy(buf, "!2||");
    res = YFrame_parse(buf);
    TEST_ASSERT_EQUAL(TypeOfRespUnknown, res);

    strcpy(buf, "!2|!2|");
    res = YFrame_parse(buf);
    TEST_ASSERT_EQUAL(TypeOfRespAck, res);

    strcpy(buf, "!3|!2|");
    res = YFrame_parse(buf);
    TEST_ASSERT_EQUAL(TypeOfRespAck, res);

    strcpy(buf, "!2|!3|");
    res = YFrame_parse(buf);
    TEST_ASSERT_EQUAL(TypeOfRespUnknown, res);

    strcpy(buf, "cmd:04040404!2|");
    res = YFrame_parse(buf);
    TEST_ASSERT_EQUAL(TypeOfRespAck, res);

    strcpy(buf, "cmd:0408!2|cmd:0408");
    res = YFrame_parse(buf);
    TEST_ASSERT_EQUAL(TypeOfRespUnknown, res);

    res = YFrame_parse((char *)0);
    TEST_ASSERT_EQUAL(TypeOfRespUnknown, res);
}

void
test_CheckEmptyFrame(void)
{
    TypeOfResp res;

    strcpy(buf, "");
    res = YFrame_parse(buf);
    TEST_ASSERT_EQUAL(TypeOfRespEmpty, res);
}

void
test_AttemptToMakeACorruptFrame(void)
{
    int expLen;
    GStatus status;

    memset(&status, 0xff, sizeof(GStatus));

    expLen = strlen(corruptSingleFrame);
    Geo_isValid_ExpectAndReturn(&(status.position), 1);
    cbox_isMoving_ExpectAndReturn(&(status.devData), 0);
    BatChr_getStatus_ExpectAndReturn(NOLINE_BATT);
    GsmMgr_getImei_ExpectAndReturn("355826018345180");
    Geo_getUtc_ExpectAndReturn(&status.position, 0);
    Geo_getLatInd_ExpectAndReturn(&status.position, 0);
    Geo_getLatitude_ExpectAndReturn(&status.position, 0);
    Geo_getLongInd_ExpectAndReturn(&status.position, 0);
    Geo_getLongitude_ExpectAndReturn(&status.position, 0);
    Geo_getSpeed_ExpectAndReturn(&status.position, 0);
    Geo_getCourse_ExpectAndReturn(&status.position, 0);
    Geo_getDate_ExpectAndReturn(&status.position, 0);

    size = YFrame_header(&status, buf, 0, YFRAME_SGP_TYPE);
    TEST_ASSERT_EQUAL_STRING("!0|19355826018345180,", buf);
    TEST_ASSERT_TRUE(size != 0);

    size += YFrame_data(&status, &buf[size], YFRAME_SGP_TYPE);
    TEST_ASSERT_EQUAL_STRING(corruptSingleFrame, buf);
    TEST_ASSERT_EQUAL(expLen, size);
}

/* ------------------------------ End of file ------------------------------ */
