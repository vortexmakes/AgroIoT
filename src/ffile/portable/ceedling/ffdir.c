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
#include "devflash.h"
#include "Trace.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef ffui8_t (*RecProc)(void);

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static FFILE_T dir[NUM_FLASH_FILES];
static ffui16_t main_check, back_check;

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
    devflash_copy_page(RF_DIR_MAIN_PAGE, RF_DIR_BACK_PAGE);
    return DIR_RECOVERY;
}

static ffui8_t
proc_page_backup(void)
{
    devflash_copy_page(RF_DIR_BACK_PAGE, RF_DIR_MAIN_PAGE);
    return DIR_BACKUP;
}

static ffui8_t
proc_page_cmp(void)
{
    if (main_check != back_check)
    {
        return proc_page_backup();
    }

    if (devflash_cmp_pages(main_page_addr, back_page_addr))
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
    ffui8_t r;
    PageRes mainPage, backPage;

    devflash_setInvalidPage();
    mainPage = devflash_verify_page(RF_DIR_MAIN_PAGE);
#if FF_DIR_BACKUP == 1
    backPage = devflash_verify_page(RF_DIR_BACK_PAGE);
    r = 0;
    r = (mainPage.result << 1) | backPage.result;
    *status = (*recovery[r])();
#else
    *status = (mainPage.result == CHECK_OK) ? DIR_OK : DIR_BAD;
#endif
    if (*status != DIR_BAD)
    {
        devflash_read_data((SA_T)0,
                           RF_DIR_MAIN_PAGE,
                           (ffui8_t *)dir,
                           sizeof(FFILE_T) * NUM_FLASH_FILES);
    }
    else
    {
        memcpy(dir,
               (ffui8_t *)defdir,
               sizeof(FFILE_T) * NUM_FLASH_FILES);
    }

    Trace_put(TraceId_Restore, (TraceArg)*status, 0);
    FFDBG_RESTORE_DIR(r);
    return dir;
}

void
ffdir_update(FFILE_T *pf)
{
    SA_T da;
    ffui8_t *sa;
    ffui8_t nfiles;

    if (devflash_is_ready_to_save_in_flash())
    {
        if (pf != (FFILE_T *)0)
        {
            da = (SA_T)(pf->fd * sizeof(FFILE_T));
            sa = (ffui8_t *)pf;
            nfiles = 1;
        }
        else
        {
            da = (SA_T)0;
            sa = (ffui8_t *)dir;
            nfiles = NUM_FLASH_FILES;
            FFDBG_SYNC();
        }

        devflash_write_data(da, RF_DIR_MAIN_PAGE, sa,
                            sizeof(FFILE_T) * nfiles);
#if FF_DIR_BACKUP == 1
        devflash_copy_page_from_buff(RF_DIR_BACK_FB_ADDRESS);
#endif
    }
}

FFILE_T *
ffdir_getFile(FFD_T fd)
{
    return &dir[fd];
}

void 
ffdir_set(FFILE_T *file, ffui8_t nFiles)
{
    memcpy(dir, file != (FFILE_T*)0 ?  file : defdir,
           sizeof(FFILE_T) * nFiles);
    devflash_write_data((SA_T)0, RF_DIR_MAIN_PAGE, (ffui8_t*)dir,
                        sizeof(FFILE_T) * nFiles);
#if FF_DIR_BACKUP == 1
    devflash_copy_page_from_buff(RF_DIR_BACK_FB_ADDRESS);
#endif
}

void 
ffdir_getDirty(DirId dir)
{
    if ((dir == DirMainId) || (dir == DirBackupId))
    {
        devflash_page_dirty((SPG_T)dir);
    }
}

FFILE_T *
ffdir_clean(void)
{
    devflash_setInvalidPage();
    ffdir_getDirty(DirMainId);
    ffdir_getDirty(DirBackupId);
    return ffdir_restore((ffui8_t *)0);
}

/* ------------------------------ End of file ------------------------------ */
