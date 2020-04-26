/**
 *  \file       test_complete.c
 *  \brief      Unit test for system status backup on Linux
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
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <ftw.h>
#include <errno.h>
#include "Backup.h"
#include "ff.h"
#include "GStatus.h"
#include "findfile.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define FRAME_DIR_PATH          "./test/support/ff"

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static char dirPath[64];
static char filePath[128];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static int 
unlink_cb(const char *fpath, const struct stat *sb, int typeflag, 
          struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv)
    {
        perror(fpath);
    }

    return rv;
}

static int 
rmrf(char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

void static
createFiles(int nFiles)
{
    int i;
    FILE *file;

    sprintf(dirPath, "%s/%s", FRAME_DIR_PATH, BACKUP_DIR_NAME);
    mkdir(dirPath, 0777);
    for (i = 0; i < nFiles; ++i)
    {
        sprintf(filePath, "%s/%05d.frm", dirPath, i);
        file = fopen(filePath, "w+");
        if (file == NULL)
        {
            printf("%s\n", strerror(errno));
            break;
        }
    }
}

/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
    int result;
    char path[64];

    /* In order to execute these test cases the disk have to be mounted */
    /* and BACKUP_DIR_NAME directory has to be recursively deleted */
    sprintf(path, "%s/%s", FRAME_DIR_PATH, BACKUP_DIR_NAME);
    result = rmrf(path);
    if (result == 0)
    {
        printf("Remove %s directory\n", path);
    }
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

    createFiles(1);

    result = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(1, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(0, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_InitWithBackupDirWithMoreThanOneFile(void)
{
    Backup info;
    int result;
    char dirPath[64];

    createFiles(3);
#if 0
    sprintf(dirPath, "%s/%s", FRAME_DIR_PATH, BACKUP_DIR_NAME);
    printf("%s\n", findFirstFile(dirPath));
    printf("%s\n", findNextFile());
    printf("%s\n", findNextFile());
    printf("%s\n", findNextFile());
    printf("%s", findNextFile());
#endif

    result = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(3, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(2, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

#if 0
void
test_InitWithFrmDirWithMoreThanOneFile(void)
{
    Backup info;
    int initResult;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(3);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(3, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(2, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_InitWithFrmDirWithExactlyAllowedFiles(void)
{
    Backup info;
    int initResult;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(BACKUP_MAXNUMFILES);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES - 1, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_InitWithFrmDirWithMoreThanAllowedFiles(void)
{
    int nFiles;
    Backup info;
    int initResult;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    nFiles = findFiles(BACKUP_MAXNUMFILES + 1);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES, nFiles);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES - 1, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_InitWithoutFrmDir(void)
{
    Backup info;
    int initResult;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_OK);
    f_mkdir_IgnoreArg_path();
    findFiles(0);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(0, info.nFiles);
    TEST_ASSERT_EQUAL(-1, info.oldest);
    TEST_ASSERT_EQUAL(-1, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_InitMkDirFail(void)
{
    Backup info;
    int initResult;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_DISK_ERR);
    f_mkdir_IgnoreArg_path();

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(1, initResult);
    TEST_ASSERT_EQUAL(-1, info.nFiles);
    TEST_ASSERT_EQUAL(-1, info.oldest);
    TEST_ASSERT_EQUAL(-1, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_InitGetNumberOfScannedFiles(void)
{
    Backup info;
    int initResult;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_OK);
    f_mkdir_IgnoreArg_path();
    findFiles(3);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(3, info.nFiles);
}

void
test_InitGetOldestAndNewestFiles(void)
{
    Backup info;
    int initResult;
    int nFilesExpected;

    nFilesExpected = 3;
    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_OK);
    f_mkdir_IgnoreArg_path();
    findFiles(nFilesExpected);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(nFilesExpected, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(nFilesExpected - 1, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_InitGetCurrentFile(void)
{
    Backup info;
    int initResult;
    int nFilesExpected;
    char name[12];

    nFilesExpected = 3;
    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_OK);
    f_mkdir_IgnoreArg_path();
    findFiles(nFilesExpected);

    initResult = Backup_init(&info);

    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(nFilesExpected, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(nFilesExpected - 1, info.newest);
    sprintf(name, "%05d.frm", nFilesExpected - 1);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_GetInfo(void)
{
    Backup info, retInfo;
    int initResult;
    int nFilesExpected;
    char name[12];

    nFilesExpected = 3;
    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_OK);
    f_mkdir_IgnoreArg_path();
    findFiles(nFilesExpected);

    initResult = Backup_init(&info);
    Backup_getInfo(&retInfo);

    TEST_ASSERT_EQUAL(0, initResult);
    TEST_ASSERT_EQUAL(retInfo.nFiles, info.nFiles);
    TEST_ASSERT_EQUAL(retInfo.oldest, info.oldest);
    TEST_ASSERT_EQUAL(retInfo.newest, info.newest);
    TEST_ASSERT_EQUAL(retInfo.nWrites, info.nWrites);
    TEST_ASSERT_EQUAL(retInfo.state, info.state);
    sprintf(name, "%05d.frm", nFilesExpected - 1);
    TEST_ASSERT_EQUAL_STRING(name, retInfo.current);
}

void
test_StoreWrongArg(void)
{
    int backupResult;

    backupResult = Backup_store((GStatus *)0);
    TEST_ASSERT_EQUAL(1, backupResult);
}

void
test_StoreCreatesTheFirstFile(void)
{
    Backup info;
    int backupResult;
    GStatus status;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(0);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, backupResult);

    strcpy(name, "00000.frm");
    strcpy(openCtx[0].path, "frames/");
    strcat(openCtx[0].path, name);
    openCtx[0].mode = FA_CREATE_ALWAYS | FA_WRITE | FA_READ;
    openCtx[0].fileSize = 0;
    openCtx[0].result = FR_OK;
    f_open_ExpectAndReturn(0, 0, 0, FR_OK);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();
    f_open_StubWithCallback(f_open_Callback);
    writeResult = FR_OK;
    bytesWritten = BACKUP_SIZEOF_REG;
    fileSize = 0;
    f_write_ExpectAndReturn(0, 0, 0, 0, FR_OK);
    f_write_IgnoreArg_fp();
    f_write_IgnoreArg_buff();
    f_write_IgnoreArg_btw();
    f_write_IgnoreArg_bw();
    f_write_StubWithCallback(f_write_Callback);

    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(0, backupResult);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(1, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(0, info.newest);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(1, info.nWrites);
}

void
test_StoreCreatesTheFirstFileButFailToWriteDiskErr(void)
{
    Backup info;
    int backupResult;
    GStatus status;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(0);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, backupResult);

    strcpy(name, "00000.frm");
    strcpy(openCtx[0].path, "frames/");
    strcat(openCtx[0].path, name);
    openCtx[0].mode = FA_CREATE_ALWAYS | FA_WRITE | FA_READ;
    openCtx[0].fileSize = 0;
    openCtx[0].result = FR_OK;
    f_open_ExpectAndReturn(0, 0, 0, FR_OK);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();
    f_open_StubWithCallback(f_open_Callback);
    writeResult = FR_DISK_ERR;
    fileSize = 0;
    f_write_ExpectAndReturn(0, 0, 0, 0, FR_DISK_ERR);
    f_write_IgnoreArg_fp();
    f_write_IgnoreArg_buff();
    f_write_IgnoreArg_btw();
    f_write_IgnoreArg_bw();
    f_write_StubWithCallback(f_write_Callback);

    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(1, backupResult);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_StoreCreatesTheFirstFileButFailToWriteLessBytesWritten(void)
{
    Backup info;
    int backupResult;
    GStatus status;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(0);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, backupResult);

    strcpy(name, "00000.frm");
    strcpy(openCtx[0].path, "frames/");
    strcat(openCtx[0].path, name);
    openCtx[0].mode = FA_CREATE_ALWAYS | FA_WRITE | FA_READ;
    openCtx[0].fileSize = 0;
    openCtx[0].result = FR_OK;
    f_open_ExpectAndReturn(0, 0, 0, FR_OK);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();
    f_open_StubWithCallback(f_open_Callback);
    writeResult = FR_OK;
    bytesWritten = 1;
    fileSize = 0;
    f_write_ExpectAndReturn(0, 0, 0, 0, FR_OK);
    f_write_IgnoreArg_fp();
    f_write_IgnoreArg_buff();
    f_write_IgnoreArg_btw();
    f_write_IgnoreArg_bw();
    f_write_StubWithCallback(f_write_Callback);

    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(1, backupResult);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}
void
test_StoreFailToCreateTheFirstFile(void)
{
    Backup info;
    int backupResult;
    GStatus status;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(0);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, backupResult);

    f_open_ExpectAndReturn(0, 0, 0, FR_DISK_ERR);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();
    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(1, backupResult);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(0, info.nFiles);
    TEST_ASSERT_EQUAL(-1, info.oldest);
    TEST_ASSERT_EQUAL(-1, info.newest);
}

void
test_StoreInCurrentFile(void)
{
    Backup info;
    int backupResult;
    GStatus status;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(1);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, backupResult);

    strcpy(name, "00000.frm");
    strcpy(openCtx[0].path, "frames/");
    strcat(openCtx[0].path, name);
    openCtx[0].mode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
    openCtx[0].fileSize = BACKUP_SIZEOF_REG * 1;
    openCtx[0].result = FR_OK;
    f_open_ExpectAndReturn(0, 0, 0, FR_OK);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();
    f_open_StubWithCallback(f_open_Callback);
    writeResult = FR_OK;
    bytesWritten = BACKUP_SIZEOF_REG;
    fileSize = 0;
    f_write_ExpectAndReturn(0, 0, 0, 0, FR_OK);
    f_write_IgnoreArg_fp();
    f_write_IgnoreArg_buff();
    f_write_IgnoreArg_btw();
    f_write_IgnoreArg_bw();
    f_write_StubWithCallback(f_write_Callback);

    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(0, backupResult);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(1, info.nWrites);
}

void
test_StoreFailsToOpenCurrentFile(void)
{
    Backup info;
    int backupResult;
    GStatus status;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(1);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, backupResult);

    f_open_ExpectAndReturn(0, 0, 0, FR_DISK_ERR);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();

    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(1, backupResult);
}

void
test_ThereIsNoRoomToStoreCreatesNewFileAndStores(void)
{
    Backup info;
    int backupResult;
    GStatus status;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(1);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, backupResult);

    sprintf(openCtx[0].path, "%s/%s", BACKUP_DIR_NAME, info.current);
    openCtx[0].mode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
    openCtx[0].fileSize = BACKUP_SIZEOF_REG * BACKUP_MAXNUMREGPERFILE;
    openCtx[0].result = FR_OK;
    openCtx[1].mode = FA_CREATE_ALWAYS | FA_WRITE | FA_READ;
    openCtx[1].result = FR_OK;
    strcpy(openCtx[1].path, "frames/00001.frm");
    openCtx[1].fileSize = 0;
    f_open_ExpectAndReturn(0, 0, 0, FR_OK);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();
    f_open_StubWithCallback(f_open_Callback);
    f_close_ExpectAndReturn(0, FR_OK);
    f_close_IgnoreArg_fp();
    f_open_ExpectAndReturn(0, 0, 0, FR_OK);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();
    f_open_StubWithCallback(f_open_Callback);
    writeResult = FR_OK;
    bytesWritten = BACKUP_SIZEOF_REG;
    fileSize = 0;
    f_write_ExpectAndReturn(0, 0, 0, 0, FR_OK);
    f_write_IgnoreArg_fp();
    f_write_IgnoreArg_buff();
    f_write_IgnoreArg_btw();
    f_write_IgnoreArg_bw();
    f_write_StubWithCallback(f_write_Callback);

    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(0, backupResult);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(2, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(1, info.newest);
    sprintf(name, "%05d.frm", info.newest);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(1, info.nWrites);
}

void
test_ThereIsNoRoomToStoreButFailsToCreateANewFile(void)
{
    Backup info;
    int backupResult;
    GStatus status;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(1);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, backupResult);

    sprintf(openCtx[0].path, "%s/%s", BACKUP_DIR_NAME, info.current);
    openCtx[0].mode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
    openCtx[0].fileSize = BACKUP_SIZEOF_REG * BACKUP_MAXNUMREGPERFILE;
    openCtx[0].result = FR_OK;
    openCtx[1].mode = FA_CREATE_ALWAYS | FA_WRITE | FA_READ;
    openCtx[1].result = FR_DISK_ERR;
    strcpy(openCtx[1].path, "frames/00001.frm");
    openCtx[1].fileSize = 0;
    f_open_ExpectAndReturn(0, 0, 0, FR_OK);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();
    f_open_StubWithCallback(f_open_Callback);
    f_close_ExpectAndReturn(0, FR_OK);
    f_close_IgnoreArg_fp();
    f_open_ExpectAndReturn(0, 0, 0, FR_DISK_ERR);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();

    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(1, backupResult);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(1, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(0, info.newest);
    sprintf(name, "%05d.frm", info.newest);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_ThereIsNoRoomToStoreRecyclesOldestFileAndStores(void)
{
    Backup info;
    int backupResult;
    GStatus status;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(BACKUP_MAXNUMFILES);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, backupResult);

    sprintf(openCtx[0].path, "%s/%s", BACKUP_DIR_NAME, info.current);
    openCtx[0].mode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
    openCtx[0].fileSize = BACKUP_SIZEOF_REG * BACKUP_MAXNUMREGPERFILE;
    openCtx[0].result = FR_OK;
    openCtx[1].mode = FA_CREATE_ALWAYS | FA_WRITE | FA_READ;
    openCtx[1].result = FR_OK;
    strcpy(openCtx[1].path, "frames/00020.frm");
    openCtx[1].fileSize = 0;
    f_open_ExpectAndReturn(0, 0, 0, FR_OK);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();
    f_open_StubWithCallback(f_open_Callback);
    f_close_ExpectAndReturn(0, FR_OK);
    f_close_IgnoreArg_fp();
    f_unlink_ExpectAndReturn(0, FR_OK);
    f_unlink_IgnoreArg_path();
    f_open_ExpectAndReturn(0, 0, 0, FR_OK);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();
    f_open_StubWithCallback(f_open_Callback);
    writeResult = FR_OK;
    bytesWritten = BACKUP_SIZEOF_REG;
    fileSize = 0;
    f_write_ExpectAndReturn(0, 0, 0, 0, FR_OK);
    f_write_IgnoreArg_fp();
    f_write_IgnoreArg_buff();
    f_write_IgnoreArg_btw();
    f_write_IgnoreArg_bw();
    f_write_StubWithCallback(f_write_Callback);

    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(0, backupResult);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES, info.nFiles);
    TEST_ASSERT_EQUAL(1, info.oldest);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES, info.newest);
    sprintf(name, "%05d.frm", info.newest);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(1, info.nWrites);
}

void
test_SyncFile(void)
{
    Backup info;
    int backupResult;
    GStatus status;
    char name[12];
    int i;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(1);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(0, backupResult);

    strcpy(name, "00000.frm");
    strcpy(openCtx[0].path, "frames/");
    strcat(openCtx[0].path, name);
    openCtx[0].mode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
    openCtx[0].fileSize = 0;
    openCtx[0].result = FR_OK;
    writeResult = FR_OK;
    bytesWritten = BACKUP_SIZEOF_REG;

    for (i = 0; i < (BACKUP_NUMWRITES - 1); ++i)
    {
        f_open_ExpectAndReturn(0, 0, 0, FR_OK);
        f_open_IgnoreArg_fp();
        f_open_IgnoreArg_path();
        f_open_IgnoreArg_mode();
        f_write_ExpectAndReturn(0, 0, 0, 0, FR_OK);
        f_write_IgnoreArg_fp();
        f_write_IgnoreArg_buff();
        f_write_IgnoreArg_btw();
        f_write_IgnoreArg_bw();
        f_write_StubWithCallback(f_write_Callback);

        backupResult = Backup_store(&status);
        TEST_ASSERT_EQUAL(0, backupResult);
        Mock_ff_Verify();
        Mock_ff_Destroy();
    }

    f_open_ExpectAndReturn(0, 0, 0, FR_OK);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();
    f_write_ExpectAndReturn(0, 0, 0, 0, FR_OK);
    f_write_IgnoreArg_fp();
    f_write_IgnoreArg_buff();
    f_write_IgnoreArg_btw();
    f_write_IgnoreArg_bw();
    f_write_StubWithCallback(f_write_Callback);
    f_sync_ExpectAndReturn(0, FR_OK);
    f_sync_IgnoreArg_fp();

    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(0, backupResult);
    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}
#endif

/* ------------------------------ End of file ------------------------------ */
