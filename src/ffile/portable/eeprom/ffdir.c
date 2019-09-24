/**
 *  \file       ffdir.c
 *  \brief
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
#include "ffile.h"
#include "ffdir.h"
#include "ffdata.h"
#include "eeprom.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define EEPROM_DIRSECTOR_ADDR       0
#define EEPROM_DIRSECTOR_ADDR_END   (EEPROM_DIRSECTOR_ADDR + \
                                     sizeof(DirSector))

/* ---------------------------- Local data types --------------------------- */
typedef ffui8_t (*RecProc)(void);
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

typedef struct DirSectorStatus DirSectorStatus;
struct DirSectorStatus
{
    ffui8_t result;
    ffui16_t checksum;
};

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static Dir dir;
static DirSector sector;
static DirSectorStatus mainDir, backupDir;

/*
 *  Recovery true table:
 *
 *  rmain: result of checksum from main page.
 *  rback: result of checksum from backup page.
 *
 *  rmain		|	rback		|	Process
 *  ---------------------------------------------------
 *  CHECK_BAD	|	CHECK_BAD	|	DIR_BAD
 *  CHECK_BAD	|	CHECK_OK	|	DIR_RECOVERY
 *  CHECK_OK	|	CHECK_BAD	|	DIR_BACKUP
 *  CHECK_OK	|	CHECK_OK	|	-> next true table
 *
 *  Checksum compare true table:
 *  ---------------------------------------------------
 *  NOT_MATCH	|	DIR_BACKUP
 *  MATCH		|	-> next true table
 *
 *  Content compare true table:
 *  ---------------------------------------------------
 *	NOT_MATCH	|	DIR_BACKUP
 *  MATCH		|	DIR_OK
 */
#if FF_DIR_BACKUP == 1
static ffui8_t proc_page_in_error(void);
static ffui8_t proc_page_recovery(void);
static ffui8_t proc_page_backup(void);
static ffui8_t proc_page_cmp(void);

static const RecProc recovery[] =
{
    proc_page_in_error, proc_page_recovery, proc_page_backup, proc_page_cmp
};
#endif

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

static ffui8_t 
cmpDirSector(void)
{
    return 1;
}

#if FF_DIR_BACKUP == 1
static ffui8_t
proc_page_in_error(void)
{
    return DIR_BAD;
}

static ffui8_t
proc_page_recovery(void)
{
    sector.main = sector.backup;
    eeprom_write((uint8_t *)&sector.main, EEPROM_DIRSECTOR_ADDR, sizeof(Dir));
    return DIR_RECOVERY;
}

static ffui8_t
proc_page_backup(void)
{
    sector.backup = sector.main;
    eeprom_write((uint8_t *)&sector.main, 
                 EEPROM_DIRSECTOR_ADDR + offsetof(DirSector, backup), 
                 sizeof(Dir));
    return DIR_BACKUP;
}

static ffui8_t
proc_page_cmp(void)
{
    if (mainDir.checksum != backupDir.checksum)
    {
        return proc_page_backup();
    }

    if (cmpDirSector())
    {
        return DIR_OK;
    }

    return proc_page_backup();
}
#endif

/* ---------------------------- Global functions --------------------------- */
FFILE_T *
ffdir_restore(ffui8_t *status)
{
    ffui8_t dirStatus;

    eeprom_init();
    eeprom_read((uint8_t *)&sector, EEPROM_DIRSECTOR_ADDR, sizeof(DirSector));

    mainDir.checksum = calculate_checksum((ffui8_t *)sector.main.file);
    mainDir.result = (sector.main.checksum == mainDir.checksum) ? 1 : 0;
#if FF_DIR_BACKUP == 1
    backupDir.checksum = calculate_checksum((ffui8_t *)sector.backup.file);
    backupDir.result = (sector.backup.checksum == backupDir.checksum) ? 1 : 0;
    dirStatus = 0;
    dirStatus = (mainDir.result << 1) | backupDir.result;
    dirStatus = (*recovery[dirStatus])();
#else
    dirStatus = (mainDir.result == CHECK_OK) ? DIR_OK : DIR_BAD;
#endif

    if (dirStatus != DIR_BAD)
    {
        eeprom_read((uint8_t *)&dir, EEPROM_DIRSECTOR_ADDR, sizeof(Dir));
    }
    else
    {
        memcpy(dir.file,
               (ffui8_t *)defdir,
               sizeof(FFILE_T) * NUM_FLASH_FILES);
    }

    FFDBG_RESTORE_DIR(dirStatus);
    if (status != (ffui8_t *)0)
    {
        *status = dirStatus;
    }
     
    return dir.file;
}

void
ffdir_update(FFILE_T *pf)
{
    if (pf == (FFILE_T *)0)
    {
        dir.checksum = calculate_checksum((ffui8_t *)dir.file);
        sector.backup = sector.main = dir;
    }
    else
    {
        dir.file[pf->fd] = *pf;
        dir.checksum = calculate_checksum((ffui8_t *)dir.file);
        sector.backup = sector.main = dir;
    }
    eeprom_write((uint8_t *)&sector, EEPROM_DIRSECTOR_ADDR, sizeof(DirSector));
}

FFILE_T *
ffdir_getFile(FFD_T fd)
{
    return &dir.file[fd];
}

void 
ffdir_getDirty(DirId dir)
{
    Dir *pDir;
    ffui16_t addr;

    if (dir == DirMainId)
    {
        pDir = &sector.main;
        addr = EEPROM_DIRSECTOR_ADDR;
    }
    else
    {
        pDir = &sector.backup;
        addr = EEPROM_DIRSECTOR_ADDR + offsetof(DirSector, backup);
    }

    eeprom_read((uint8_t *)&sector, EEPROM_DIRSECTOR_ADDR, sizeof(DirSector));
    pDir->checksum = ~pDir->checksum;
    eeprom_write((uint8_t *)pDir, addr, sizeof(Dir));
}

/* ------------------------------ End of file ------------------------------ */
