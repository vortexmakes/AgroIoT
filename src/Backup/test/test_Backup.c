/**
 *  \file       test_Backup.c
 *  \brief      Unit test for system status backup
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
#include <stdlib.h>
#include "Backup.h"
#include "Mock_ff.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static FRESULT result;
static int fileIndex;
static TCHAR files[4][16] =
{
    "0.frm", "1.frm", "2.frm", "3.frm"
};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static FRESULT
f_mkdir_Callback(const TCHAR* path, int cmock_num_calls)
{
    TEST_ASSERT_EQUAL_STRING(BACKUP_FRMDIR, path);
    return result;
}

static FRESULT
f_findfirst_Callback(DIR* dp, FILINFO* fno, const TCHAR* path, 
                     const TCHAR* pattern, int cmock_num_calls)
{
    strcpy(fno->fname, files[fileIndex++]);
    TEST_ASSERT_EQUAL_STRING(BACKUP_FRMDIR, path);
    TEST_ASSERT_EQUAL_STRING("*.frm", pattern);
    return result;
}

static FRESULT
f_findnext_Callback(DIR* dp, FILINFO* fno, int cmock_num_calls)
{
    strcpy(fno->fname, files[fileIndex++]);
    return result;
}

/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
    Mock_ff_Init();
}

void
tearDown(void)
{
    Mock_ff_Verify();
    Mock_ff_Destroy();
}

void
test_InitFrmDirExistsWithOneFile(void)
{
    fileIndex = 0;
    result = FR_EXIST;
    f_mkdir_ExpectAndReturn(BACKUP_FRMDIR, result);
    f_mkdir_IgnoreArg_path();
    f_mkdir_StubWithCallback(f_mkdir_Callback);
    result = FR_OK;

    f_findfirst_ExpectAndReturn(0, 0, BACKUP_FRMDIR, "*.frm", result);
    f_findfirst_IgnoreArg_dp();
    f_findfirst_IgnoreArg_fno();
    f_findfirst_IgnoreArg_path();
    f_findfirst_IgnoreArg_pattern();
    f_findfirst_StubWithCallback(f_findfirst_Callback);

    f_findnext_StubWithCallback(f_findnext_Callback);
    f_findnext_ExpectAndReturn(0, 0, FR_OK);
    f_findnext_IgnoreArg_dp();
    f_findnext_IgnoreArg_fno();

    f_findnext_ExpectAndReturn(0, 0, FR_OK);
    f_findnext_IgnoreArg_dp();
    f_findnext_IgnoreArg_fno();

    files[2][0] = 0;
    Backup_init();
}

/*
void
test_InitFrmDirExistsWithThreeFiles(void)
{
    int i;

    result = FR_EXIST;
    f_mkdir_ExpectAndReturn(BACKUP_FRMDIR, result);
    f_mkdir_IgnoreArg_path();
    f_mkdir_StubWithCallback(f_mkdir_Callback);
    result = FR_OK;
    fileIndex = 0;
    f_findfirst_ExpectAndReturn(0, 0, BACKUP_FRMDIR, "*.frm", result);
    f_findfirst_IgnoreArg_dp();
    f_findfirst_IgnoreArg_fno();
    f_findfirst_IgnoreArg_path();
    f_findfirst_IgnoreArg_pattern();
    f_findfirst_StubWithCallback(f_findfirst_Callback);

    fileIndex = 1;
    f_findnext_ExpectAndReturn(0, 0, FR_OK);
    f_findnext_IgnoreArg_dp();
    f_findnext_IgnoreArg_fno();
    f_findnext_StubWithCallback(f_findnext_Callback);

    fileIndex = 1;
    f_findnext_ExpectAndReturn(0, 0, FR_OK);
    f_findnext_IgnoreArg_dp();
    f_findnext_IgnoreArg_fno();
    f_findnext_StubWithCallback(f_findnext_Callback);

    result = 1;
    f_findnext_ExpectAndReturn(0, 0, FR_OK);
    f_findnext_IgnoreArg_dp();
    f_findnext_IgnoreArg_fno();
    f_findnext_StubWithCallback(f_findnext_CallbackAtTheEnd);

    Backup_init();
}*/

void
test_InitFrmDirDoesNotExist(void)
{
    TEST_IGNORE();
}

void
test_InitFailOnMkdir(void)
{
    TEST_IGNORE();
}

/* ------------------------------ End of file ------------------------------ */
