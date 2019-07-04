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
#include "unity.h"
#include "ffile.h"
#include "rfile.h"
#include "devflash.h"
#include "ffport.h"
#include "ffdata.h"
#include "GStatus.h"
#include "Config.h"

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
    devflash_page_dirty(RF_DIR_MAIN_PAGE);
    devflash_page_dirty(RF_DIR_BACK_PAGE);
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

    TEST_ASSERT_EQUAL(FFD0, file->fd);
    TEST_ASSERT_EQUAL(QFILE_TYPE, file->type);
    TEST_ASSERT_EQUAL(sizeof(GPS_STR), file->size_reg);
    TEST_ASSERT_EQUAL(FRFILE_OK, result);

    file = ffile_get_file_info(FFD1);
    result = ffile_is_corrupted(FFD1);

    TEST_ASSERT_EQUAL(FFD1, file->fd);
    TEST_ASSERT_EQUAL(RFILE_TYPE, file->type);
    TEST_ASSERT_EQUAL(sizeof(Config), file->size_reg);
    TEST_ASSERT_EQUAL(FRFILE_OK, result);
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

    devflash_page_dirty(RF_DIR_MAIN_PAGE);
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

    devflash_page_dirty(RF_DIR_BACK_PAGE);
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

    writeData.aclimit = 8;
    writeData.brlimit = 4;
    writeData.status = 2;
    readData.aclimit = 1;
    readData.brlimit = 1;
    readData.status = 1;

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
