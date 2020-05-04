/**
 *  \file       test_complete.c
 *  \brief      Unit test for system status backup on any platform
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
#include <stdio.h>
#include "Backup.h"
#include "ff.h"
#include "FindFile.h"
#include "FileMgr.h"

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
    /* In order to execute these test cases the disk have to be mounted */
    /* and BACKUP_DIR_NAME directory has to be recursively deleted */
    FileMgr_cd(BACKUP_DIR_NAME);
    FileMgr_rmrf();
}

void
tearDown(void)
{
}

void
test_InitWithoutBackupDir(void)
{
    Backup info;
    int result;

    result = Backup_init(&info);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(0, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(0, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_InitWithBackupDirWithOneFile(void)
{
    Backup info;
    int result;
    int nFilesExpected;
    char name[12];

    nFilesExpected = 1;
    FileMgr_createFiles(nFilesExpected);

    result = Backup_init(&info);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(nFilesExpected, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(nFilesExpected - 1, info.newest);
    sprintf(name, "%05u.frm", nFilesExpected - 1);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_InitWithBackupDirWithMoreThanOneFile(void)
{
    Backup info;
    int result;
    int nFilesExpected;
    char name[12];

    nFilesExpected = 3;
    FileMgr_createFiles(nFilesExpected);

    result = Backup_init(&info);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(nFilesExpected, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(nFilesExpected - 1, info.newest);
    sprintf(name, "%05u.frm", nFilesExpected - 1);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_InitWithBackupDirWithExactlyAllowedFiles(void)
{
    Backup info;
    int result;
    int nFilesExpected;
    char name[12];

    nFilesExpected = BACKUP_MAXNUMFILES;
    FileMgr_createFiles(nFilesExpected);

    result = Backup_init(&info);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(nFilesExpected, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(nFilesExpected - 1, info.newest);
    sprintf(name, "%05u.frm", nFilesExpected - 1);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_InitWithoutBackupFiles(void)
{
    Backup info;
    int result;

    FileMgr_createFiles(0);

    result = Backup_init(&info);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(0, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(0, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_GetInfo(void)
{
    Backup info, retInfo;
    int result;
    int nFilesExpected;
    char name[12];

    nFilesExpected = 3;
    FileMgr_createFiles(nFilesExpected);

    result = Backup_init(&info);
    Backup_getInfo(&retInfo);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(retInfo.nFiles, info.nFiles);
    TEST_ASSERT_EQUAL(retInfo.oldest, info.oldest);
    TEST_ASSERT_EQUAL(retInfo.newest, info.newest);
    TEST_ASSERT_EQUAL(retInfo.nWrites, info.nWrites);
    TEST_ASSERT_EQUAL(retInfo.state, info.state);
    sprintf(name, "%05u.frm", nFilesExpected - 1);
    TEST_ASSERT_EQUAL_STRING(name, retInfo.current);
}

void
test_StoreWrongArg(void)
{
    int result;

    result = Backup_store((GStatus *)0);
    TEST_ASSERT_EQUAL(1, result);
}

void
test_StoresAndCreatesTheFirstFile(void)
{
    Backup info;
    int result;
    GStatus status;
    char name[12];

    strcpy(name, "00000.frm");
    result = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, result);

    result = Backup_store(&status);
    TEST_ASSERT_EQUAL(0, result);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(1, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(0, info.newest);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(1, info.nWrites);
}

void
test_StoresInCurrentFile(void)
{
    Backup info;
    int result;
    GStatus status;

    FileMgr_createFiles(1);

    result = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, result);

    result = Backup_store(&status);
    TEST_ASSERT_EQUAL(0, result);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(1, info.nWrites);
}

void
test_ThereIsNoRoomToStoreCreatesNewFileAndStores(void)
{
    Backup info;
    int result;
    GStatus status;
    char name[12];

    FileMgr_createFiles(1);
    FileMgr_fillFile("00000.frm");

    result = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, result);

    result = Backup_store(&status);
    TEST_ASSERT_EQUAL(0, result);

    Backup_getInfo(&info);

    TEST_ASSERT_EQUAL(2, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(1, info.newest);
    sprintf(name, "%05u.frm", info.newest);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(1, info.nWrites);
}

void
test_ThereIsNoRoomToStoreRecyclesOldestFileAndStores(void)
{
    Backup info;
    int i, result;
    GStatus status;
    char name[12];

    FileMgr_createFiles(BACKUP_MAXNUMFILES);
    FileMgr_fillFile("00019.frm");

    result = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, result);

    result = Backup_store(&status);
    TEST_ASSERT_EQUAL(0, result);

    Backup_getInfo(&info);

    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES, info.nFiles);
    TEST_ASSERT_EQUAL(1, info.oldest);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES, info.newest);
    sprintf(name, "%05u.frm", info.newest);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(1, info.nWrites);

    for (i = 0; i < BACKUP_MAXNUMREGPERFILE; ++i)
    {
        result = Backup_store(&status);
        TEST_ASSERT_EQUAL(0, result);
    }

    Backup_getInfo(&info);

    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES, info.nFiles);
    TEST_ASSERT_EQUAL(2, info.oldest);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES + 1, info.newest);
    sprintf(name, "%05u.frm", info.newest);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(1, info.nWrites);
}

/* ------------------------------ End of file ------------------------------ */
