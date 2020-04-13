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
#include <string.h>
#include <stddef.h>
#include "unity.h"
#include "ffdir.h"
#include "ffdata.h"
#include "Mock_eeprom.h"
#include "Mock_rkhassert.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct Dir Dir;
struct Dir
{
    FFILE_T file[NUM_FLASH_FILES];
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
static DirSector dirSectorRead, dirSectorWrite;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static ffui16_t
calculate_checksum(ffui8_t *address)
{
    ffui16_t check, i;
    ffui8_t *data;

    data = (ffui8_t*)address;
    FFILE_WATCHDOG();

    for (check = 0, i = (sizeof(FFILE_T) * NUM_FLASH_FILES); i; --i)
    {
        check += *data++;
    }

    return ~check;
}

static void
makeDirSector(DirSector *sector)
{
    Dir *dir;

    TEST_ASSERT_NOT_NULL(sector);
    dir = &sector->main;
    memcpy(dir->file,
           (ffui8_t *)defdir,
           sizeof(FFILE_T) * NUM_FLASH_FILES);
    dir->checksum = calculate_checksum((ffui8_t *)dir->file);
    sector->backup = sector->main;
}

static void
crashDirSector(Dir *sector)
{
    TEST_ASSERT_NOT_NULL(sector);
    sector->checksum = ~sector->checksum;
}

static void
MockEepromReadCallback(uint8_t *p, uint16_t addr, uint16_t qty,
                       int cmock_num_calls)
{
    memcpy(p, &dirSectorRead, qty);
}

static void
MockEepromWriteCallback(uint8_t *p, uint16_t addr, uint16_t qty,
                        int cmock_num_calls)
{
    memcpy((uint8_t *)&dirSectorWrite + addr, p, qty);
}

static void
restoreDir(void)
{
    ffui8_t status;
    FFILE_T *dir;

    eeprom_init_Expect();
    eeprom_read_Expect(0, 0, sizeof(DirSector));
    eeprom_read_IgnoreArg_p();
    eeprom_read_StubWithCallback(MockEepromReadCallback);
    eeprom_read_Expect(0, 0, sizeof(Dir));
    eeprom_read_IgnoreArg_p();

    dir = ffdir_restore(&status);

    TEST_ASSERT_NOT_NULL(dir);
    TEST_ASSERT_EQUAL(DIR_OK, status);
}

static void
MockAssertCallback(const char* const file, int line, int cmock_num_calls)
{
    TEST_PASS();
}

static void
setDir(void)
{
    dirSectorRead.main.checksum = 
                    calculate_checksum((ffui8_t *)dirSectorRead.main.file);
    dirSectorRead.backup = dirSectorRead.main;
    restoreDir();
}

/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
    Mock_eeprom_Init();
    makeDirSector(&dirSectorRead);
}

void
tearDown(void)
{
    Mock_eeprom_Verify();
    Mock_eeprom_Destroy();
}

void
test_RestoreDirWhenMainBackupAreEquals(void)
{
    ffui8_t status;
    FFILE_T *dir;

    eeprom_init_Expect();
    eeprom_read_Expect(0, 0, sizeof(DirSector));
    eeprom_read_IgnoreArg_p();
    eeprom_read_StubWithCallback(MockEepromReadCallback);
    eeprom_read_Expect(0, 0, sizeof(Dir));
    eeprom_read_IgnoreArg_p();

    dir = ffdir_restore(&status);

    TEST_ASSERT_NOT_NULL(dir);
    TEST_ASSERT_EQUAL(DIR_OK, status);
    TEST_ASSERT_EQUAL(FFD0, dir->fd);
    TEST_ASSERT_EQUAL(QFILE_TYPE, dir->type);
    ++dir;
    TEST_ASSERT_EQUAL(FFD1, dir->fd);
    TEST_ASSERT_EQUAL(RFILE_TYPE, dir->type);
}

void
test_RestoreDirFromBackup(void)
{
    ffui8_t status;
    FFILE_T *dir;

    crashDirSector(&dirSectorRead.backup);
    eeprom_init_Expect();
    eeprom_read_Expect(0, 0, sizeof(DirSector));
    eeprom_read_IgnoreArg_p();
    eeprom_read_StubWithCallback(MockEepromReadCallback);
    eeprom_write_Expect(0, offsetof(DirSector, backup), sizeof(Dir));
    eeprom_write_IgnoreArg_p();
    eeprom_read_Expect(0, 0, sizeof(Dir));
    eeprom_read_IgnoreArg_p();

    dir = ffdir_restore(&status);

    TEST_ASSERT_NOT_NULL(dir);
    TEST_ASSERT_EQUAL(DIR_BACKUP, status);
    TEST_ASSERT_EQUAL(FFD0, dir->fd);
    TEST_ASSERT_EQUAL(QFILE_TYPE, dir->type);
    ++dir;
    TEST_ASSERT_EQUAL(FFD1, dir->fd);
    TEST_ASSERT_EQUAL(RFILE_TYPE, dir->type);
}

void
test_RestoreDirFromMain(void)
{
    ffui8_t status;
    FFILE_T *dir;

    crashDirSector(&dirSectorRead.main);
    eeprom_init_Expect();
    eeprom_read_Expect(0, 0, sizeof(DirSector));
    eeprom_read_IgnoreArg_p();
    eeprom_read_StubWithCallback(MockEepromReadCallback);
    eeprom_write_Expect(0, 0, sizeof(Dir));
    eeprom_write_IgnoreArg_p();
    eeprom_read_Expect(0, 0, sizeof(Dir));
    eeprom_read_IgnoreArg_p();

    dir = ffdir_restore(&status);

    TEST_ASSERT_NOT_NULL(dir);
    TEST_ASSERT_EQUAL(DIR_RECOVERY, status);
    TEST_ASSERT_EQUAL(FFD0, dir->fd);
    TEST_ASSERT_EQUAL(QFILE_TYPE, dir->type);
    ++dir;
    TEST_ASSERT_EQUAL(FFD1, dir->fd);
    TEST_ASSERT_EQUAL(RFILE_TYPE, dir->type);
}

void
test_RestoreDirFromDefault(void)
{
    ffui8_t status;
    FFILE_T *dir;

    crashDirSector(&dirSectorRead.main);
    crashDirSector(&dirSectorRead.backup);
    eeprom_init_Expect();
    eeprom_read_Expect(0, 0, sizeof(DirSector));
    eeprom_read_IgnoreArg_p();
    eeprom_read_StubWithCallback(MockEepromReadCallback);

    dir = ffdir_restore(&status);

    TEST_ASSERT_NOT_NULL(dir);
    TEST_ASSERT_EQUAL(DIR_BAD, status);
    TEST_ASSERT_EQUAL(FFD0, dir->fd);
    TEST_ASSERT_EQUAL(QFILE_TYPE, dir->type);
    ++dir;
    TEST_ASSERT_EQUAL(FFD1, dir->fd);
    TEST_ASSERT_EQUAL(RFILE_TYPE, dir->type);
}

void
test_RestoreDirFromBackupChecksumNotEqual(void)
{
    ffui8_t status;
    FFILE_T *dir;

    memcpy(&dirSectorRead.main.file[1],
           &dirSectorRead.main.file[0],
           sizeof(FFILE_T));
    dirSectorRead.main.checksum =
        calculate_checksum((ffui8_t *)dirSectorRead.main.file);
    eeprom_init_Expect();
    eeprom_read_Expect(0, 0, sizeof(DirSector));
    eeprom_read_IgnoreArg_p();
    eeprom_read_StubWithCallback(MockEepromReadCallback);
    eeprom_write_Expect(0, offsetof(DirSector, backup), sizeof(Dir));
    eeprom_write_IgnoreArg_p();
    eeprom_read_Expect(0, 0, sizeof(Dir));
    eeprom_read_IgnoreArg_p();

    dir = ffdir_restore(&status);

    TEST_ASSERT_NOT_NULL(dir);
    TEST_ASSERT_EQUAL(DIR_BACKUP, status);
    TEST_ASSERT_EQUAL(FFD0, dir->fd);
    TEST_ASSERT_EQUAL(QFILE_TYPE, dir->type);
    ++dir;
    TEST_ASSERT_EQUAL(FFD0, dir->fd);
    TEST_ASSERT_EQUAL(QFILE_TYPE, dir->type);
}

void
test_GetDirtyDirectory(void)
{
    eeprom_read_Expect(0, 0, sizeof(DirSector));
    eeprom_read_IgnoreArg_p();
    eeprom_read_StubWithCallback(MockEepromReadCallback);
    eeprom_write_Expect(0, offsetof(DirSector, main), sizeof(Dir));
    eeprom_write_IgnoreArg_p();
    eeprom_write_StubWithCallback(MockEepromWriteCallback);

    ffdir_getDirty(DirMainId);

    TEST_ASSERT_EQUAL_HEX16(~dirSectorRead.main.checksum,
                            dirSectorWrite.main.checksum);

    eeprom_read_Expect(0, 0, sizeof(DirSector));
    eeprom_read_IgnoreArg_p();
    eeprom_read_StubWithCallback(MockEepromReadCallback);
    eeprom_write_Expect(0, offsetof(DirSector, backup), sizeof(Dir));
    eeprom_write_IgnoreArg_p();
    eeprom_write_StubWithCallback(MockEepromWriteCallback);

    ffdir_getDirty(DirBackupId);

    TEST_ASSERT_EQUAL_HEX16(~dirSectorRead.backup.checksum,
                            dirSectorWrite.backup.checksum);
}

void
test_StoreWholeDirectoryInMemory(void)
{
    ffui8_t status;

    eeprom_init_Expect();
    eeprom_read_Expect(0, 0, sizeof(DirSector));
    eeprom_read_IgnoreArg_p();
    eeprom_read_StubWithCallback(MockEepromReadCallback);
    eeprom_read_Expect(0, 0, sizeof(Dir));
    eeprom_read_IgnoreArg_p();

    ffdir_restore(&status);

    eeprom_write_Expect(0, 0, sizeof(DirSector));
    eeprom_write_IgnoreArg_p();
    eeprom_write_StubWithCallback(MockEepromWriteCallback);

    ffdir_update((FFILE_T *)0);

    TEST_ASSERT_EQUAL_MEMORY(&dirSectorRead,
                             &dirSectorWrite,
                             sizeof(DirSector));
}

void
test_StoreOneFileInMemory(void)
{
    ffui8_t status;

    eeprom_init_Expect();
    eeprom_read_Expect(0, 0, sizeof(DirSector));
    eeprom_read_IgnoreArg_p();
    eeprom_read_StubWithCallback(MockEepromReadCallback);
    eeprom_read_Expect(0, 0, sizeof(Dir));
    eeprom_read_IgnoreArg_p();

    ffdir_restore(&status);

    dirSectorRead.main.file[FFD1].type = QFILE_TYPE;
    dirSectorRead.main.checksum =
        calculate_checksum((ffui8_t *)dirSectorRead.main.file);
    dirSectorRead.backup = dirSectorRead.main;

    eeprom_write_Expect(0, 0, sizeof(DirSector));
    eeprom_write_IgnoreArg_p();
    eeprom_write_StubWithCallback(MockEepromWriteCallback);

    ffdir_update(&dirSectorRead.main.file[FFD1]);

    TEST_ASSERT_EQUAL_MEMORY(&dirSectorRead,
                             &dirSectorWrite,
                             sizeof(DirSector));
}

void
test_GetValidFile(void)
{
    FFD_T fd = FFD0;
    FFILE_T *file = (FFILE_T *)0;

    restoreDir();
    file = ffdir_getFile(fd);
    TEST_ASSERT_NOT_NULL(file);
}

void
test_RequestAnInvalidFile(void)
{
    FFD_T fd = NUM_FLASH_FILES;

    restoreDir();
    rkh_assert_Expect("", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    ffdir_getFile(fd);
}

void
test_RequestAFileWithCurruptedFd(void)
{
    FFD_T fd = FFD0;

    dirSectorRead.main.file[fd].fd = NUM_FLASH_FILES;
    setDir();
    rkh_assert_Expect("", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    ffdir_getFile(fd);
}

void
test_RequestAFileWithCurruptedType(void)
{
    FFD_T fd = FFD0;

    dirSectorRead.main.file[fd].type = RFILE_TYPE + 1;
    setDir();
    rkh_assert_Expect("", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    ffdir_getFile(fd);
}

void
test_RequestAFileWithCurruptedNumPages(void)
{
    FFD_T fd = FFD0;

    dirSectorRead.main.file[fd].num_pages = 0xffff;
    setDir();
    rkh_assert_Expect("", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    ffdir_getFile(fd);
}

void
test_RequestAFileWithCurruptedBeginPage(void)
{
    FFD_T fd = FFD0;

    dirSectorRead.main.file[fd].begin_page = 0xffff;
    setDir();
    rkh_assert_Expect("", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    ffdir_getFile(fd);
}

void
test_RequestAFileWithCurruptedSizeReg(void)
{
    FFD_T fd = FFD0;

    dirSectorRead.main.file[fd].size_reg = 0xffff;
    setDir();
    rkh_assert_Expect("", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    ffdir_getFile(fd);
}

void
test_RequestAFileWithCurruptedIn(void)
{
    TEST_IGNORE();
}

void
test_RequestAFileWithCurruptedOut(void)
{
    TEST_IGNORE();
}

void
test_RequestAFileWithCurruptedQty(void)
{
    TEST_IGNORE();
}

void
test_RequestAFileWithCurruptedPos(void)
{
    TEST_IGNORE();
}

void
test_RequestAFileWithCurruptedPosQty(void)
{
    TEST_IGNORE();
}

/* ------------------------------ End of file ------------------------------ */
