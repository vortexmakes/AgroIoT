/**
 *  \file       test_SecString.c
 *  \brief      Unit test for secure string module
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
#include "unity.h"
#include <string.h>
#include "SecString.h"

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
test_CheckNullTerminatedString(void)
{
    char str[16];
    char *pos = (char *)(0xdeadbeaf);

    strcpy(str, "123456789");
    pos = SecString_strchk(str, sizeof(str));
    TEST_ASSERT_NOT_NULL(pos);
}

void
test_CheckNonNullTerminatedString(void)
{
    char str[16];
    char *pos = (char *)(0xdeadbeaf);

    memset(str, '@', sizeof(str));
    pos = SecString_strchk(str, sizeof(str));
    TEST_ASSERT_NULL(pos);
    
    str[15] = '\0';
    pos = SecString_strchk(str, sizeof(str) - 1);
    TEST_ASSERT_NULL(pos);
}

void
test_CheckEmptyString(void)
{
    char *str = "123456789";
    char *pos = (char *)(0xdeadbeaf);

    pos = SecString_strchk(str, 0);

    TEST_ASSERT_NULL(pos);
}

void
test_GetLengthOfANullTerminatedString(void)
{
    char str[16];
    size_t len;

    strcpy(str, "123456789");
    len = SecString_strnlen(str, sizeof(str));
    TEST_ASSERT_EQUAL(len, 9);
}

void
test_GetLengthOfANonNullTerminatedString(void)
{
    char str[16];
    size_t len;

    memset(str, '@', sizeof(str));
    len = SecString_strnlen(str, sizeof(str));
    TEST_ASSERT_EQUAL(len, sizeof(str));
}

void
test_GetLengthOfAnEmptyString(void)
{
    char str[16];
    size_t len;

    str[0] = '\0';
    len = SecString_strnlen(str, sizeof(str));
    TEST_ASSERT_EQUAL(len, 0);
}

/* ------------------------------ End of file ------------------------------ */
