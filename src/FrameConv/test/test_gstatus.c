/**
 *  \file       test_gtatus.c
 *  \brief      Unit test for general status module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.08.02  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include "unity.h"
#include "GStatus.h"
#include "Mock_Crc32.h"

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
test_SetChecksum(void)
{
    GStatus me;
    Crc32 result;

    result = 0xdeadbeaf;
    Crc32_init_Expect();
    Crc32_calc_ExpectAndReturn((const uint8_t *)&me.data, 
                               sizeof(GStatusType), 
                               0xffffffff,
                               result);
    GStatus_setChecksum(&me);
    TEST_ASSERT_EQUAL_HEX(result, me.checksum);
}

void
test_CheckAValidData(void)
{
    GStatus me;
    bool result;

    me.checksum = result = 0xdeadbeaf;
    Crc32_init_Expect();
    Crc32_calc_ExpectAndReturn((const uint8_t *)&me.data, 
                               sizeof(GStatusType), 
                               0xffffffff,
                               result);
    result = GStatus_checkValidity(&me);
    TEST_ASSERT_EQUAL_HEX(true, result);
}

void
test_CheckACorruptData(void)
{
    GStatus me;
    bool result;

    me.checksum = 0xdeadbeaf;
    result = 0;
    Crc32_init_Expect();
    Crc32_calc_ExpectAndReturn((const uint8_t *)&me.data, 
                               sizeof(GStatusType), 
                               0xffffffff,
                               result);
    result = GStatus_checkValidity(&me);
    TEST_ASSERT_EQUAL_HEX(false, result);
}

void
test_CheckAnInValidData(void)
{
    bool result;

    result = GStatus_checkValidity((GStatus *)0);
    TEST_ASSERT_EQUAL_HEX(false, result);
}


/* ------------------------------ End of file ------------------------------ */
