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
#include "Mock_GStatus.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct OpenContext OpenContext;
struct OpenContext
{
    char path[24];
    BYTE mode;
    FSIZE_t fileSize;
    FRESULT result;
};

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static OpenContext openCtx[2];
static FRESULT mkdirResult, findFirstResult, findNextResult, writeResult;
static int fileIndex;
static TCHAR files[BACKUP_MAXNUMFILES][16];
static UINT bytesWritten;
static FSIZE_t fileSize;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static FRESULT
f_mkdir_Callback(const TCHAR* path, int cmock_num_calls)
{
    TEST_ASSERT_EQUAL_STRING(BACKUP_DIR_NAME, path);
    return mkdirResult;
}

static FRESULT
f_findfirst_Callback(DIR* dp, FILINFO* fno, const TCHAR* path, 
                     const TCHAR* pattern, int cmock_num_calls)
{
    strcpy(fno->fname, files[fileIndex++]);
    TEST_ASSERT_EQUAL_STRING(BACKUP_DIR_NAME, path);
    TEST_ASSERT_EQUAL_STRING("*.frm", pattern);
    return findFirstResult;
}

static FRESULT
f_findnext_Callback(DIR* dp, FILINFO* fno, int cmock_num_calls)
{
    strcpy(fno->fname, files[fileIndex++]);
    return findNextResult;
}

static FRESULT
f_open_Callback(FIL* fp, const TCHAR* path, BYTE mode, 
                int cmock_num_calls)
{
    fp->obj.objsize = openCtx[cmock_num_calls].fileSize;
    TEST_ASSERT_EQUAL_STRING(openCtx[cmock_num_calls].path, path);
    TEST_ASSERT_EQUAL(openCtx[cmock_num_calls].mode, mode);
    return openCtx[cmock_num_calls].result;
}

static FRESULT
f_write_Callback(FIL* fp, const void* buff, UINT btw, UINT* bw, 
                 int cmock_num_calls)
{
    *bw = bytesWritten;
    fp->obj.objsize = fileSize;
    return writeResult;
}

static int
findFiles(int nFiles)
{
    int i, actualNumFiles;

    if (nFiles > BACKUP_MAXNUMFILES)
    {
        actualNumFiles = BACKUP_MAXNUMFILES;
    }
    else
    {
        actualNumFiles = nFiles;
    }
    if (actualNumFiles > 0)
    {
        for (i = actualNumFiles - 1; i >= 0; --i)
        {
            sprintf(files[i], "%05u.frm", i);
        }
    }
    files[actualNumFiles][0] = 0;

    fileIndex = 0;
    f_findfirst_ExpectAndReturn(0, 0, BACKUP_DIR_NAME, "*.frm", FR_OK);
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

        f_open_ExpectAndReturn(0, 0, 0, FR_OK);
        f_open_IgnoreArg_fp();
        f_open_IgnoreArg_path();
        f_open_IgnoreArg_mode();
    }

    f_closedir_ExpectAndReturn(0, FR_OK);
    f_closedir_IgnoreArg_dp();
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
test_Deinit(void)
{
    Backup info;
    BackupCode initResult;

    initResult = Backup_deinit(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, initResult);
    TEST_ASSERT_EQUAL(Backup_NoInit, info.error);
}

void
test_InitWithFrmDirWithoutFiles(void)
{
    Backup info;
    BackupCode initResult;

    mkdirResult = FR_EXIST;
    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    f_mkdir_StubWithCallback(f_mkdir_Callback);
    findFiles(0);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, initResult);
    TEST_ASSERT_EQUAL(0, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(0, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
    TEST_ASSERT_EQUAL(Backup_Ok, info.error);
}

void
test_InitWithFrmDirWithOneFile(void)
{
    Backup info;
    BackupCode initResult;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    strcpy(name, "00000.frm");
    strcpy(openCtx[0].path, "frames/");
    strcat(openCtx[0].path, name);
    openCtx[0].mode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
    openCtx[0].fileSize = 0;
    openCtx[0].result = FR_OK;
    f_open_StubWithCallback(f_open_Callback);
    findFiles(1);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, initResult);
    TEST_ASSERT_EQUAL(1, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(0, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
    TEST_ASSERT_EQUAL(Backup_Ok, info.error);
}

void
test_InitWithFrmDirWithOneFileButFailsToOpenIt(void)
{
    Backup info;
    BackupCode initResult;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    strcpy(name, "00000.frm");
    strcpy(openCtx[0].path, "frames/");
    strcat(openCtx[0].path, name);
    openCtx[0].mode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
    openCtx[0].fileSize = 0;
    openCtx[0].result = FR_NO_FILE;
    f_open_StubWithCallback(f_open_Callback);
    findFiles(1);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_OpenFileError, initResult);
    TEST_ASSERT_EQUAL(Backup_OpenFileError, info.error);
}

void
test_InitWithFrmDirWithMoreThanOneFile(void)
{
    Backup info;
    BackupCode initResult;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(3);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, initResult);
    TEST_ASSERT_EQUAL(3, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(2, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
    TEST_ASSERT_EQUAL(Backup_Ok, info.error);
}

void
test_InitWithFrmDirWithExactlyAllowedFiles(void)
{
    Backup info;
    BackupCode initResult;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(BACKUP_MAXNUMFILES);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, initResult);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES - 1, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
    TEST_ASSERT_EQUAL(Backup_Ok, info.error);
}

void
test_InitWithFrmDirWithMoreThanAllowedFiles(void)
{
    int nFiles;
    Backup info;
    BackupCode initResult;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    nFiles = findFiles(BACKUP_MAXNUMFILES + 1);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES, nFiles);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, initResult);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES - 1, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
    TEST_ASSERT_EQUAL(Backup_Ok, info.error);
}

void
test_InitWithoutFrmDir(void)
{
    Backup info;
    BackupCode initResult;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_OK);
    f_mkdir_IgnoreArg_path();
    findFiles(0);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, initResult);
    TEST_ASSERT_EQUAL(0, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(0, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
    TEST_ASSERT_EQUAL(Backup_Ok, info.error);
}

void
test_InitMkDirFail(void)
{
    Backup info;
    BackupCode initResult;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_DISK_ERR);
    f_mkdir_IgnoreArg_path();

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_DirError, initResult);
    TEST_ASSERT_EQUAL(0, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(0, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
    TEST_ASSERT_EQUAL(Backup_DirError, info.error);
}

void
test_InitGetNumberOfScannedFiles(void)
{
    Backup info;
    BackupCode initResult;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_OK);
    f_mkdir_IgnoreArg_path();
    findFiles(3);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, initResult);
    TEST_ASSERT_EQUAL(3, info.nFiles);
}

void
test_InitGetOldestAndNewestFiles(void)
{
    Backup info;
    BackupCode initResult;
    int nFilesExpected;

    nFilesExpected = 3;
    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_OK);
    f_mkdir_IgnoreArg_path();
    findFiles(nFilesExpected);

    initResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, initResult);
    TEST_ASSERT_EQUAL(nFilesExpected, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(nFilesExpected - 1, info.newest);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_InitGetCurrentFile(void)
{
    Backup info;
    BackupCode initResult;
    int nFilesExpected;
    char name[12];

    nFilesExpected = 3;
    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_OK);
    f_mkdir_IgnoreArg_path();
    findFiles(nFilesExpected);

    initResult = Backup_init(&info);

    TEST_ASSERT_EQUAL(Backup_Ok, initResult);
    TEST_ASSERT_EQUAL(nFilesExpected, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(nFilesExpected - 1, info.newest);
    sprintf(name, "%05u.frm", nFilesExpected - 1);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_GetInfo(void)
{
    Backup info, retInfo;
    BackupCode initResult;
    int nFilesExpected;
    char name[12];

    nFilesExpected = 3;
    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_OK);
    f_mkdir_IgnoreArg_path();
    findFiles(nFilesExpected);

    initResult = Backup_init(&info);
    Backup_getInfo(&retInfo);

    TEST_ASSERT_EQUAL(Backup_Ok, initResult);
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
    BackupCode backupResult;

    backupResult = Backup_store((GStatus *)0);
    TEST_ASSERT_EQUAL(Backup_WrongArgsInitError, backupResult);
}

void
test_StoreWithInitError(void)
{
    Backup info;
    GStatus status;
    BackupCode backupResult;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_DISK_ERR);
    f_mkdir_IgnoreArg_path();

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_DirError, backupResult);
    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(Backup_WrongArgsInitError, backupResult);
}

void
test_StoreCreatesTheFirstFile(void)
{
    Backup info;
    BackupCode backupResult;
    GStatus status;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(0);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);

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
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);

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
    BackupCode backupResult;
    GStatus status;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(0);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);

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
    TEST_ASSERT_EQUAL(Backup_WriteError, backupResult);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_StoreCreatesTheFirstFileButFailToWriteLessBytesWritten(void)
{
    Backup info;
    BackupCode backupResult;
    GStatus status;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(0);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);

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
    TEST_ASSERT_EQUAL(Backup_WriteError, backupResult);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}
void
test_StoreFailToCreateTheFirstFile(void)
{
    Backup info;
    BackupCode backupResult;
    GStatus status;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(0);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);

    f_open_ExpectAndReturn(0, 0, 0, FR_DISK_ERR);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();
    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(Backup_FailToCreateFirstFile, backupResult);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(0, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(0, info.newest);
}

void
test_StoreInCurrentFile(void)
{
    Backup info;
    BackupCode backupResult;
    GStatus status;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    findFiles(1);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);

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
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(1, info.nWrites);
}

void
test_ThereIsNoRoomToStoreCreatesNewFileAndStores(void)
{
    Backup info;
    BackupCode backupResult;
    GStatus status;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    sprintf(openCtx[0].path, "%s/%05u.frm", BACKUP_DIR_NAME, 0);
    openCtx[0].mode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
    openCtx[0].fileSize = BACKUP_SIZEOF_REG * BACKUP_MAXNUMREGPERFILE;
    openCtx[0].result = FR_OK;
    strcpy(openCtx[1].path, "frames/00001.frm");
    openCtx[1].mode = FA_CREATE_ALWAYS | FA_WRITE | FA_READ;
    openCtx[1].fileSize = 0;
    openCtx[1].result = FR_OK;
    f_open_StubWithCallback(f_open_Callback);
    findFiles(1);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);

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
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(2, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(1, info.newest);
    sprintf(name, "%05u.frm", info.newest);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(1, info.nWrites);
}

void
test_ThereIsNoRoomToStoreButFailsToCreateANewFile(void)
{
    Backup info;
    BackupCode backupResult;
    GStatus status;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    sprintf(openCtx[0].path, "%s/%05u.frm", BACKUP_DIR_NAME, 0);
    openCtx[0].mode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
    openCtx[0].fileSize = BACKUP_SIZEOF_REG * BACKUP_MAXNUMREGPERFILE;
    openCtx[0].result = FR_OK;
    strcpy(openCtx[1].path, "frames/00001.frm");
    openCtx[1].mode = FA_CREATE_ALWAYS | FA_WRITE | FA_READ;
    openCtx[1].fileSize = 0;
    openCtx[1].result = FR_DISK_ERR;
    f_open_StubWithCallback(f_open_Callback);
    findFiles(1);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);

    f_close_ExpectAndReturn(0, FR_OK);
    f_close_IgnoreArg_fp();
    f_open_ExpectAndReturn(0, 0, 0, FR_DISK_ERR);
    f_open_IgnoreArg_fp();
    f_open_IgnoreArg_path();
    f_open_IgnoreArg_mode();

    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(Backup_FailToCreateNewFile, backupResult);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(1, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(0, info.newest);
    sprintf(name, "%05u.frm", info.newest);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_ThereIsNoRoomToStoreRecyclesOldestFileAndStores(void)
{
    Backup info;
    BackupCode backupResult;
    GStatus status;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    sprintf(openCtx[0].path, "%s/%05u.frm", BACKUP_DIR_NAME, 
            BACKUP_MAXNUMFILES - 1);
    openCtx[0].mode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
    openCtx[0].fileSize = BACKUP_SIZEOF_REG * BACKUP_MAXNUMREGPERFILE;
    openCtx[0].result = FR_OK;
    sprintf(openCtx[1].path, "%s/%05u.frm", BACKUP_DIR_NAME, 
            BACKUP_MAXNUMFILES);
    openCtx[1].mode = FA_CREATE_ALWAYS | FA_WRITE | FA_READ;
    openCtx[1].fileSize = 0;
    openCtx[1].result = FR_OK;
    f_open_StubWithCallback(f_open_Callback);
    findFiles(BACKUP_MAXNUMFILES);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);

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
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES, info.nFiles);
    TEST_ASSERT_EQUAL(1, info.oldest);
    TEST_ASSERT_EQUAL(BACKUP_MAXNUMFILES, info.newest);
    sprintf(name, "%05u.frm", info.newest);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(1, info.nWrites);
}

void
test_AutomaticallySyncCurrentFile(void)
{
    Backup info;
    BackupCode backupResult;
    GStatus status;
    int i;

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    sprintf(openCtx[0].path, "%s/%05u.frm", BACKUP_DIR_NAME, 0);
    openCtx[0].mode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
    openCtx[0].fileSize = 0;
    openCtx[0].result = FR_OK;
    f_open_StubWithCallback(f_open_Callback);
    findFiles(1);

    backupResult = Backup_init(&info);
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);

    writeResult = FR_OK;
    bytesWritten = BACKUP_SIZEOF_REG;

    for (i = 0; i < (BACKUP_NUMWRITES - 1); ++i)
    {
        f_write_ExpectAndReturn(0, 0, 0, 0, FR_OK);
        f_write_IgnoreArg_fp();
        f_write_IgnoreArg_buff();
        f_write_IgnoreArg_btw();
        f_write_IgnoreArg_bw();
        f_write_StubWithCallback(f_write_Callback);

        backupResult = Backup_store(&status);
        TEST_ASSERT_EQUAL(Backup_Ok, backupResult);
        Mock_ff_Verify();
        Mock_ff_Destroy();
    }

    f_write_ExpectAndReturn(0, 0, 0, 0, FR_OK);
    f_write_IgnoreArg_fp();
    f_write_IgnoreArg_buff();
    f_write_IgnoreArg_btw();
    f_write_IgnoreArg_bw();
    f_write_StubWithCallback(f_write_Callback);
    f_sync_ExpectAndReturn(0, FR_OK);
    f_sync_IgnoreArg_fp();

    backupResult = Backup_store(&status);
    TEST_ASSERT_EQUAL(Backup_Ok, backupResult);
    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_ForceSyncBackupFile(void)
{
    BackupCode result;
    Backup info;
    char name[12];

    f_mkdir_ExpectAndReturn(BACKUP_DIR_NAME, FR_EXIST);
    f_mkdir_IgnoreArg_path();
    strcpy(name, "00000.frm");
    strcpy(openCtx[0].path, "frames/");
    strcat(openCtx[0].path, name);
    openCtx[0].mode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
    openCtx[0].fileSize = 0;
    openCtx[0].result = FR_OK;
    f_open_StubWithCallback(f_open_Callback);
    findFiles(1);

    Backup_init(&info);
    TEST_ASSERT_EQUAL(1, info.nFiles);
    TEST_ASSERT_EQUAL(Backup_Ok, info.error);

    f_sync_ExpectAndReturn(0, FR_OK);
    f_sync_IgnoreArg_fp();
    result = Backup_sync();

    TEST_ASSERT_EQUAL(Backup_Ok, result);

    Backup_getInfo(&info);
    TEST_ASSERT_EQUAL(1, info.nFiles);
    TEST_ASSERT_EQUAL(0, info.oldest);
    TEST_ASSERT_EQUAL(0, info.newest);
    sprintf(name, "%05u.frm", info.newest);
    TEST_ASSERT_EQUAL_STRING(name, info.current);
    TEST_ASSERT_EQUAL(0, info.nWrites);
}

void
test_FailsToForceSyncBackupFile(void)
{
    BackupCode result;

    /* Try to sync before initializing Backup */
    Backup_deinit(0);
    result = Backup_sync();
    TEST_ASSERT_EQUAL(Backup_NoInit, result);

    /* Without backup files */
    f_mkdir_IgnoreAndReturn(FR_OK);
    f_findfirst_IgnoreAndReturn(FR_NO_FILE);
    f_closedir_IgnoreAndReturn(FR_NO_FILE);

    Backup_init(0);
    result = Backup_sync();

    TEST_ASSERT_EQUAL(Backup_NoInit, result);
}

/* ------------------------------ End of file ------------------------------ */
