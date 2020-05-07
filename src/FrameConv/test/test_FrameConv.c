/**
 *  \file       test_FrameConv.c
 *  \brief      Unit test for FrameConv module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.06.05  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include "unity.h"
#include "FrameConv.h"
#include "Mock_GStatus.h"
#if 0
#include "YFrame.h"
#include "Geo.h"
#include "cbox.h"
#include "GsmMgr.h"
#else
#include "Mock_YFrame.h"
#endif

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
extern char imei[16];

/* ---------------------------- Local variables ---------------------------- */
static uint8_t buf[128] =
{
  0x31, 0x34, 0x31, 0x33, 0x30, 0x35, 0x00, 0x41, 0x00, 0x33, 0x38, 0x2e,
  0x30, 0x31, 0x33, 0x33, 0x37, 0x33, 0x00, 0x00, 0x2d, 0x00, 0x30, 0x35,
  0x37, 0x2e, 0x35, 0x37, 0x34, 0x30, 0x39, 0x34, 0x00, 0x00, 0x2d, 0x00,
  0x30, 0x2e, 0x30, 0x32, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x30, 0x36, 0x30, 0x35, 0x32, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x9b, 0xd5, 0x24
};
static uint8_t frame[128];
static GStatus expStatus =
{
    {
        {
            "141305", 
            "A", 
            "38.013373", 
            "-", 
            "057.574094", 
            "-", 
            "0.026",
            "", 
            "060520"
        },
        {0, 0, {0, 0, 0}, {0xffff, 0x0000, 0x0000, 0}, 0},
        {0xfc, 0x00},
        0
    },
    0xc09bd524
};

static const char singleFrame[] =
   /*------ header ------*/
    "!0|01355826018345180,185124,-37.8402883,-057.6884350,0.078,,310119,3FFF,0000,00,00,DDDD,FFFF,FFFF,3";

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
test_ConvertGStatusFromSTM32ToX86(void)
{
    int result;
    GStatus status;
    Geo *pos, *expPos;
    CBOX_STR *devData, *expDevData;
    IOStatus *io, *expIo;
    BatChrStatus *bat, *expBat;

    expPos = &expStatus.data.position;
    pos = &status.data.position;
    expDevData = &expStatus.data.devData;
    devData = &status.data.devData;
    expIo= &expStatus.data.ioStatus;
    io = &status.data.ioStatus;
    bat = &status.data.batChrStatus;
    expBat = &status.data.batChrStatus;

    result = FrameConv_STM32ToX86(&status, buf, SIZEOF_GSTATUS_STM32);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_STRING(expPos->utc, pos->utc);
    TEST_ASSERT_EQUAL_STRING(expPos->status, pos->status);
    TEST_ASSERT_EQUAL_STRING(expPos->latitude, pos->latitude);
    TEST_ASSERT_EQUAL_STRING(expPos->latInd, pos->latInd);
    TEST_ASSERT_EQUAL_STRING(expPos->longitude, pos->longitude);
    TEST_ASSERT_EQUAL_STRING(expPos->longInd, pos->longInd);
    TEST_ASSERT_EQUAL_STRING(expPos->speed, pos->speed);
    TEST_ASSERT_EQUAL_STRING(expPos->course, pos->course);
    TEST_ASSERT_EQUAL_STRING(expPos->date, pos->date);
    TEST_ASSERT_EQUAL(expDevData->cmd, devData->cmd);
    TEST_ASSERT_EQUAL(expDevData->m, devData->m);
    TEST_ASSERT_EQUAL(expDevData->h.hoard, devData->h.hoard);
    TEST_ASSERT_EQUAL(expDevData->h.pqty, devData->h.pqty);
    TEST_ASSERT_EQUAL(expDevData->h.flow, devData->h.flow);
    TEST_ASSERT_EQUAL(expDevData->a.x, devData->a.x);
    TEST_ASSERT_EQUAL(expDevData->a.y, devData->a.y);
    TEST_ASSERT_EQUAL(expDevData->a.z, devData->a.z);
    TEST_ASSERT_EQUAL(expDevData->hum, devData->hum);
    TEST_ASSERT_EQUAL(expIo->digIn, io->digIn);
    TEST_ASSERT_EQUAL(expBat, bat);
}

void
test_ConvertGStatusToFrame(void)
{
    int result;
    GStatus status;
    size_t size;

    YFrame_header_ExpectAndReturn(&status.data, frame, 0, YFRAME_SGP_TYPE, 8);
    YFrame_data_ExpectAndReturn(&status.data, frame + 8, YFRAME_SGP_TYPE, 8);

    result = FrameConv_GStatusToFrame(frame, &status, &size);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(16, size);
}

/* ------------------------------ End of file ------------------------------ */
