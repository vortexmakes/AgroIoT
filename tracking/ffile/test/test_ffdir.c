/**
 *  \file       test_ffdir.c
 *  \brief      Unit test for ffdir module for eemprom.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.07.10  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "ffdir.h"
#include "Mock_ffdata.h"
#include "Mock_eeprom.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct Dir Dir;
struct Dir
{
    FFILE_T data[NUM_FLASH_FILES];
    ffui16_t checksum;
};

typedef struct DirSector DirSector;
struct DirSector
{
    Dir main;
    Dir backup;
};

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
test_RestoreDirBothMainBackupEqual(void)
{
    ffui8_t status;
    FFILE_T *dir;

    eeprom_init_Expect();
    /* verify main directory -> ok */
    /* verify backup directory -> ok */
    /* return from eeprom */

    dir = ffdir_restore(&status);
    TEST_ASSERT_NOT_NULL(dir);
    TEST_ASSERT_EQUAL(DIR_OK, status);
}

/* ------------------------------ End of file ------------------------------ */
