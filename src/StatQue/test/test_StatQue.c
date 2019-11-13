/**
 *  \file       test_StatQue.c
 *  \brief      Unit test for status queue's adapter.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.18.02  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "StatQue.h"
#include "Mock_ffdata.h"
#include "Mock_ffile.h"

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
    Mock_ffile_Init();
}

void
tearDown(void)
{
    Mock_ffile_Verify();
}

void
test_InitOpenFile(void)
{
    ffile_queue_open_as_random_ExpectAndReturn(FFD0,
                                               OPEN_FROM_BEGINNING, 0);
    StatQue_init();
}

void
test_GetNumElems(void)
{
    rui16_t nElem, nElemExp;

    nElemExp = 128;
    ffile_tell_ExpectAndReturn(FFD0, nElemExp);
    nElem = StatQue_getNumElem();

    TEST_ASSERT_EQUAL(nElemExp, nElem);
}

void
test_IsEmpty(void)
{
    rbool_t res;
    rui16_t nElemExp;

    nElemExp = 0;
    ffile_tell_ExpectAndReturn(FFD0, nElemExp);

    res = StatQue_isEmpty();
    TEST_ASSERT_EQUAL(1, res);
}

void
test_RemoveOneElemSuccessfully(void)
{
    rInt res;
    GPS_STR *elem;

    ffile_queue_remove_ExpectAndReturn(FFD0, elem, FQFILE_OK);

    res = StatQue_remove(elem);
    TEST_ASSERT_EQUAL(0, res);
}

void
test_RemoveOneElemWrongly(void)
{
    rInt res;
    GPS_STR *elem;

    ffile_queue_remove_ExpectAndReturn(FFD0, elem, FQFILE_EMPTY);

    res = StatQue_remove(elem);
    TEST_ASSERT_EQUAL(1, res);
}

void
test_Delete(void)
{
    rInt res;
    rui16_t nElem;

    nElem = 8;
    ffile_queue_delete_ExpectAndReturn(FFD0, (NR_T *)&nElem, FQFILE_OK);

    res = StatQue_delete(&nElem);
    TEST_ASSERT_EQUAL(0, res);
}

void
test_Read(void)
{
    rInt res;
    GPS_STR *elem;

    ffile_queue_random_read_ExpectAndReturn(FFD0, READ_FORWARD,
                                            elem, FQFILE_OK);

    res = StatQue_read(elem);
    TEST_ASSERT_EQUAL(0, res);
}

void
test_Put(void)
{
    rInt res;
    GPS_STR *elem;

    ffile_queue_insert_ExpectAndReturn(FFD0, elem, FQFILE_OK);

    res = StatQue_put(elem);
    TEST_ASSERT_EQUAL(0, res);
}

/* ------------------------------ End of file ------------------------------ */
