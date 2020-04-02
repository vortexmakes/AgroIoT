/**
 *  \file       test_Crc32.c
 *  \brief      Unit test for CRC-32 module
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include "unity.h"
#include "Crc32.h"

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
test_CalculateCRC32(void)
{
    uint8_t message[] = "123456789";
    uint32_t result, expected = 0xcbf43926;

    Crc32_init();
    result = Crc32_calc(message, strlen(message), 0xffffffff);

    TEST_ASSERT_EQUAL_HEX(expected, result);
}

/* ------------------------------ End of file ------------------------------ */
