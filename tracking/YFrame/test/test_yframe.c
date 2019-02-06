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
#include "unity.h"
#include "YFrame.h"
#include "Mock_cbox.h"
#include "Mock_Geo.h"
#include "Mock_BatChr.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
static const char imei[] = "355826018345180";
static RawData defStatus =
{
    {
        "185124", "A", "37.8402883", "-", "057.6884350", "-", "0.078",
        "", "310119"
    },
    {1, 1, {0, 0, 1}, {0xdddd, 0xffff, 0xffff}, 3},
    {0xff, 0x3f},
    3
};

static const char pattern[] = "!0|19355826018345180,185124,-37.8402883,-057.6884350,0.078,,310119,3FFF,0000,00,00,DDDD,FFFF,FFFF,3|";

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RawData status;
static char buf[256];
static int size;

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
test_MakeInvalidArgs(void)
{
    size = YFrame_makeFrame(&status, (char *)0);
    TEST_ASSERT_EQUAL(0, size);

    size = YFrame_makeFrame((RawData *)0, buf);
    TEST_ASSERT_EQUAL(0, size);
}

void
test_GetFlagsInvalidArgs(void)
{
    rui8_t flags;
    rInt err;

    flags = 0xad;
    err = YFrame_getFlags((RawData *)0, &flags, 0);
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
    Geo_isValid_ExpectAndReturn(&(defStatus.position), 1);
    cbox_isMoving_ExpectAndReturn(&(defStatus.dev), 0);
    BatChr_getStatus_ExpectAndReturn(LINE_NOBATT);

    err = YFrame_getFlags((RawData *)&defStatus, &flags, YFRAME_MGP_TYPE);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(0x13, flags);
}

void
test_MakeSingle(void)
{
    TEST_IGNORE();
}

void
test_MakeMultiple(void)
{
    TEST_IGNORE();
}

/* ------------------------------ End of file ------------------------------ */
