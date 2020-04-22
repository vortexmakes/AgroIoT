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
static FRESULT mkdirResult, findFirstResult, findNextResult;
static int fileIndex;
static TCHAR files[BACKUP_MAXNUMFRMFILES][16];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static FRESULT
f_mkdir_Callback(const TCHAR* path, int cmock_num_calls)
{
    TEST_ASSERT_EQUAL_STRING(BACKUP_FRMDIR, path);
    return mkdirResult;
}

static FRESULT
f_findfirst_Callback(DIR* dp, FILINFO* fno, const TCHAR* path, 
                     const TCHAR* pattern, int cmock_num_calls)
{
    strcpy(fno->fname, files[fileIndex++]);
    TEST_ASSERT_EQUAL_STRING(BACKUP_FRMDIR, path);
    TEST_ASSERT_EQUAL_STRING("*.frm", pattern);
    return findFirstResult;
}

static FRESULT
f_findnext_Callback(DIR* dp, FILINFO* fno, int cmock_num_calls)
{
    strcpy(fno->fname, files[fileIndex++]);
    return findNextResult;
}

static int
findFiles(int nFiles)
{
    int i, actualNumFiles;

    if (nFiles > BACKUP_MAXNUMFRMFILES)
    {
        actualNumFiles = BACKUP_MAXNUMFRMFILES;
    }
    else
    {
        actualNumFiles = nFiles;
    }
    for (i = 0; i < actualNumFiles; ++i)
    {
        sprintf(files[i], "%d.frm", i);
    }
    files[actualNumFiles][0] = 0;

    fileIndex = 0;
    f_findfirst_ExpectAndReturn(0, 0, BACKUP_FRMDIR, "*.frm", FR_OK);
    f_findfirst_IgnoreArg_dp();
    f_findfirst_IgnoreArg_fno();
    f_findfirst_IgnoreArg_path();
    f_findfirst_IgnoreArg_pattern();
    f_findfirst_StubWithCallback(f_findfirst_Callback);

    if (actualNumFiles > 0)
    {
        for (i = 0; i < (actualNumFiles - 1); ++i)
        {
            f_findnext_ExpectAndReturn(0, 0, FR_OK);
            f_findnext_IgnoreArg_dp();
            f_findnext_IgnoreArg_fno();
            f_findnext_StubWithCallback(f_findnext_Callback);
        }

        f_findnext_ExpectAndReturn(0, 0, FR_OK);
        f_findnext_IgnoreArg_dp();
        f_findnext_IgnoreArg_fno();
        f_findnext_StubWithCallback(f_findnext_Callback);
    }
    return actualNumFiles;
}

/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
    Mock_ff_Init();
    mkdirResult = FR_DISK_ERR;
    findFirstResult = FR_OK;
    findNextResult = FR_OK;
}

void
tearDown(void)
{
    Mock_ff_Verify();
    Mock_ff_Destroy();
}

void
test_InitWithFrmDirWithoutFiles(void)
{
    Backup info;
    int initResult;

    mkdirResult = FR_EXIST;
    f_mkdir_ExpectAndReturn(BACKUP_FRMDIR, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    f_mkdir_StubWithCallback(f_mkdir_Callback);
    findFiles(0);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(0, info.nFiles);
}

void
test_InitWithFrmDirWithOneFile(void)
{
    Backup info;
    int initResult;

    f_mkdir_ExpectAndReturn(BACKUP_FRMDIR, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(1);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(1, info.nFiles);
}

void
test_InitWithFrmDirWithMoreThanOneFile(void)
{
    Backup info;
    int initResult;

    f_mkdir_ExpectAndReturn(BACKUP_FRMDIR, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(3);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(3, info.nFiles);
}

void
test_InitWithFrmDirWithExactlyAllowedFiles(void)
{
    Backup info;
    int initResult;

    f_mkdir_ExpectAndReturn(BACKUP_FRMDIR, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(BACKUP_MAXNUMFRMFILES);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFRMFILES, info.nFiles);
}

void
test_InitWithFrmDirWithMoreThanAllowedFiles(void)
{
    int nFiles;
    Backup info;
    int initResult;

    f_mkdir_ExpectAndReturn(BACKUP_FRMDIR, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    nFiles = findFiles(BACKUP_MAXNUMFRMFILES + 1);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFRMFILES, nFiles);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFRMFILES, info.nFiles);
}

void
test_InitWithoutFrmDir(void)
{
    Backup info;
    int initResult;

    f_mkdir_ExpectAndReturn(BACKUP_FRMDIR, FR_OK);
    f_mkdir_IgnoreArg_path();
    findFiles(0);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(0, info.nFiles);
}

void
test_InitMkDirFail(void)
{
    Backup info;
    int initResult;

    f_mkdir_ExpectAndReturn(BACKUP_FRMDIR, FR_DISK_ERR);
    f_mkdir_IgnoreArg_path();

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(1, initResult);
    TEST_ASSERT_EQUAL(-1, info.nFiles);
}

void
test_InitCalculateOldestFile(void)
{
    Backup info;
    int initResult;

    f_mkdir_ExpectAndReturn(BACKUP_FRMDIR, FR_OK);
    f_mkdir_IgnoreArg_path();
    findFiles(3);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(3, info.nFiles);
    /*TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(2, info.newest);*/
}

/* ------------------------------ End of file ------------------------------ */
