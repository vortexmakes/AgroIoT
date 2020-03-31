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
    char *str = "123456789";
    char *pos = (char *)(0xdeadbeaf);
    size_t len;

    len = strlen(str);
    pos = SecString_strchk(str, len + 1);

    TEST_ASSERT_NOT_NULL(pos);
    TEST_ASSERT_EQUAL(pos - str, len);
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

/* ------------------------------ End of file ------------------------------ */
