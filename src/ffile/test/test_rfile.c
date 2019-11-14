/**
 *  \file       test_rfile.c
 *  \brief      Unit test for rfile module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.07.01  LeFr  v1.0.00
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include "unity.h"
#include "ffdata.h"
#include "rfile.h"
#include "Mock_ffile.h"
#include "Mock_devflash.h"
#include "Mock_ffdir.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static FFILE_T dir[NUM_FLASH_FILES];

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
test_RestoreDirectory(void)
{
    PageRes res;
    ffui8_t nFile;
    FFILE_T *file;

    res.result = PAGE_OK;
    memcpy(dir, (FFILE_T *)defdir, sizeof(FFILE_T) * NUM_FLASH_FILES);
    ffdir_restore_ExpectAndReturn(0, dir);
    ffdir_restore_IgnoreArg_status();
    devflash_verify_page_IgnoreAndReturn(res);
    for (nFile = 0, file = dir; nFile < NUM_FLASH_FILES; ++nFile, ++file)
    {
        ffdir_update_Expect(file);
        ffdir_update_IgnoreArg_file();
    }

    rfile_init_directory();
    TEST_ASSERT_EQUAL(0, dir[0].page_error);
    TEST_ASSERT_EQUAL(0, dir[1].page_error);
}

void
test_GetDirectory(void)
{
    ffdir_getFile_ExpectAndReturn(FFD0, (FFILE_T *)&defdir[FFD0]);
    rfile_get_file(FFD0);
}

void
test_FormatFile(void)
{
    FFILE_T *file;

    memcpy(dir, (FFILE_T *)defdir, sizeof(FFILE_T) * NUM_FLASH_FILES);
    file = (FFILE_T *)&dir[FFD0];
    devflash_is_ready_to_save_in_flash_ExpectAndReturn(1);
    devflash_format_pages_Expect(file->begin_page, file->num_pages);
    ffdir_update_Expect(file);

    rfile_file_format(file);
}

/* ------------------------------ End of file ------------------------------ */
