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
#include "ffile.h"
#include "ffdir.h"
#include "ffdata.h"
#include "eeprom.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef ffui8_t (*RecProc)(void);
typedef struct Dir Dir;
struct Dir
{
    FFILE_T data[NUM_FLASH_FILES];
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
static Dir dir;

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
#if FF_DIR_BACKUP == 1
static ffui8_t
proc_page_in_error(void)
{
    return DIR_BAD;
}

static ffui8_t
proc_page_recovery(void)
{
    /*devflash_copy_page(RF_DIR_MAIN_PAGE, RF_DIR_BACK_PAGE);*/
    return DIR_RECOVERY;
}

static ffui8_t
proc_page_backup(void)
{
    /*devflash_copy_page(RF_DIR_BACK_PAGE, RF_DIR_MAIN_PAGE);*/
    return DIR_BACKUP;
}

static ffui8_t
proc_page_cmp(void)
{
#if 0
    if (main_check != back_check)
    {
        return proc_page_backup();
    }

    if (devflash_cmp_pages(main_page_addr, back_page_addr))
    {
        return DIR_OK;
    }
#endif

    return proc_page_backup();
}
#endif

/* ---------------------------- Global functions --------------------------- */
FFILE_T *
ffdir_restore(ffui8_t *status)
{
    eeprom_init();
    if (status != (ffui8_t *)0)
    {
        *status = DIR_OK;
    }

    return dir.data;
}

void
ffdir_update(FFILE_T *pf)
{
}

FFILE_T *
ffdir_getFile(FFD_T fd)
{
    return &dir.data[fd];
}

void 
ffdir_set(FFILE_T *file, ffui8_t nFiles)
{
}

/* ------------------------------ End of file ------------------------------ */
