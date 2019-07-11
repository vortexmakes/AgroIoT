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
#include "unity.h"
#include "ffdir.h"
#include "ffdata.h"
#include "Mock_eeprom.h"

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
static DirSector dirSector;

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

    dir = &sector->backup;
    memcpy(dir->file, 
           (ffui8_t *)defdir, 
           sizeof(FFILE_T) * NUM_FLASH_FILES);
    dir->checksum = calculate_checksum((ffui8_t *)dir->file);
}

static void 
MockEepromReadCallback(uint8_t *p, uint16_t addr, uint16_t qty, 
                       int cmock_num_calls)
{
    memcpy(p, &dirSector, qty);
}

/* ---------------------------- Global functions --------------------------- */
void 
setUp(void)
{
    makeDirSector(&dirSector);
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

/* ------------------------------ End of file ------------------------------ */
