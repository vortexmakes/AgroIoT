/**
 *  \file       test_ffile.c
 *  \brief      Unit test for ffile module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.04.03  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include "unity.h"
#include "ffile.h"
#include "rfile.h"
#include "devflash.h"
#include "ffport.h"
#include "ffdata.h"
#include "ffdir.h"
#include "GStatus.h"
#include "Config.h"
#include "eeprom.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
crashDirectory(void)
{
    flash_init();
    eeprom_init();
    rfile_getDirtyDir(DirMainId);
    rfile_getDirtyDir(DirBackupId);
}

/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
}

void
tearDown(void)
{
    ffile_close();
}

void
test_RestoreDefaultDirectory(void)
{
    FFILE_T *file;
    FFUInt result;

    crashDirectory();
    ffile_init();
    file = ffile_get_file_info(FFD0);
    result = ffile_is_corrupted(FFD0);

    TEST_ASSERT_EQUAL(FRFILE_OK, result);
    TEST_ASSERT_EQUAL(FFD0, file->fd);
    TEST_ASSERT_EQUAL(QFILE_TYPE, file->type);
    TEST_ASSERT_EQUAL(sizeof(GStatus), file->size_reg);

    file = ffile_get_file_info(FFD1);
    result = ffile_is_corrupted(FFD1);

    TEST_ASSERT_EQUAL(FRFILE_OK, result);
    TEST_ASSERT_EQUAL(FFD1, file->fd);
    TEST_ASSERT_EQUAL(RFILE_TYPE, file->type);
    TEST_ASSERT_EQUAL(sizeof(Config), file->size_reg);
}

void
test_RestoreDirectoryFromBackup(void)
{
    FFILE_T *file;
    int nWritten;
    Config writeData;

    crashDirectory();
    ffile_init();

    nWritten = ffile_random_access(FFD1, WRITE_ACCESS, &writeData, 1);
    file = ffile_get_file_info(FFD1);
    TEST_ASSERT_EQUAL(1, nWritten);
    TEST_ASSERT_EQUAL(1, file->pos);
    ffile_sync();

    rfile_getDirtyDir(DirMainId);
    ffile_init();

    TEST_ASSERT_EQUAL(1, file->pos);
}

void
test_RestoreDirectoryFromMain(void)
{
    FFILE_T *file;
    int nWritten;
    Config writeData;

    crashDirectory();
    ffile_init();

    file = ffile_get_file_info(FFD1);
    nWritten = ffile_random_access(FFD1, WRITE_ACCESS, &writeData, 1);
    file = ffile_get_file_info(FFD1);
    TEST_ASSERT_EQUAL(1, nWritten);
    TEST_ASSERT_EQUAL(1, file->pos);
    ffile_sync();

    rfile_getDirtyDir(DirBackupId);
    ffile_init();

    TEST_ASSERT_EQUAL(1, file->pos);
}

void
test_RestoreDirectoryFromMemory(void)
{
    FFILE_T *file;
    int nWritten;
    Config writeData;

    crashDirectory();
    ffile_init();

    file = ffile_get_file_info(FFD1);
    nWritten = ffile_random_access(FFD1, WRITE_ACCESS, &writeData, 1);
    file = ffile_get_file_info(FFD1);
    TEST_ASSERT_EQUAL(1, nWritten);
    TEST_ASSERT_EQUAL(1, file->pos);
    ffile_sync();

    ffile_init();

    TEST_ASSERT_EQUAL(1, file->pos);
}

void
test_WriteAndReadRandomFile(void)
{
    int nRead, nWritten;
    Config writeData;
    Config readData;

    memset(&writeData, 0xde, sizeof(Config));
    memset(&readData, 0xad, sizeof(Config));

    crashDirectory();
    ffile_init();
    nWritten = ffile_random_access(FFD1, WRITE_ACCESS, &writeData, 1);
    ffile_seek(FFD1, 0);
    nRead = ffile_random_access(FFD1, READ_ACCESS, &readData, 1);

    TEST_ASSERT_EQUAL(1, nWritten);
    TEST_ASSERT_EQUAL(1, nRead);
    TEST_ASSERT_EQUAL_MEMORY(&writeData, &readData, sizeof(Config));
}

/* ------------------------------ End of file ------------------------------ */
