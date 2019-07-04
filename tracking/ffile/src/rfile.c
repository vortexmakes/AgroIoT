/**
 *  \file       rfile.c
 *  \brief
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
#include <string.h>
#include "ffile.h"
#include "devflash.h"
#include "ffdata.h"
#include "ffdir.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef ffui8_t (*RECPROC_T)(void);

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static FFILE_T dir[NUM_FLASH_FILES];
static SA_T reg_addr;
static SPG_T page_num;
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

static const RECPROC_T recovery[] =
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
void
rfile_file_format(FFILE_T *pf)
{
    FFDBG_FILE_FORMAT(pf);
    if (devflash_is_ready_to_save_in_flash())
    {
        devflash_format_pages(pf->begin_page, pf->num_pages);
        pf->pos_qty = pf->pos = pf->in = pf->out = pf->qty = 0;
        pf->page_error = 0;
        rfile_update_directory(pf);
    }
}

void
rfile_init_directory(void)
{
    ffui8_t file, r;
    SPG_T page, page_error;
    FFILE_T *pf;
    PageRes res;

    ffdir_restore(&r);
    for (file = 0, pf = dir; file < NUM_FLASH_FILES; ++file, ++pf)
    {
        for (page = page_error = 0; page < pf->num_pages; ++page)
        {
            res = devflash_verify_page(page + pf->begin_page);
            if (res.result == PAGE_BAD)
            {
                ++page_error;
            }
            else
            {
                break;
            }
        }
        pf->page_error = page_error;
#if FF_AUTO_FILE_FORMAT == 1
        if (pf->page_error == pf->num_pages)
        {
            rfile_file_format(pf);
        }
#endif
        rfile_update_directory(pf);
    }
}

void
rfile_update_directory(FFILE_T *pf)
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

void
rfile_access_register(RACC_T *pra)
{
    NR_T numregs,       /* number of register to access */
         remregs,       /* number of remainded register in current page */
         posreg,        /* register position in page */
         nrpp;          /* number of registers per page */
    ffui8_t *pdata;     /* pointer to data to write or read */

    nrpp = pra->pf->num_regs_per_page;
    posreg = pra->currpos % nrpp;
    page_num = (SPG_T)(pra->currpos / nrpp + pra->pf->begin_page);

    numregs = pra->rqty;
    remregs = numregs > (nrpp - posreg) ? (nrpp - posreg) : numregs;
    pdata = pra->ptofrom;

    while (numregs > 0)
    {
        reg_addr = (SA_T)(posreg * pra->pf->size_reg);
        if (pra->op == READ_ACCESS)
        {
            devflash_read_data(reg_addr,
                               page_num,
                               pdata,
                               pra->pf->size_reg * remregs);
        }
        else
        {
            devflash_write_data(reg_addr,
                                page_num,
                                pdata,
                                pra->pf->size_reg * remregs);
        }

        numregs -= remregs;
        pdata += (remregs * pra->pf->size_reg);
        ++page_num;
        remregs = numregs > nrpp ? nrpp : numregs;
        posreg = 0;
    }
}

FFILE_T *
rfile_get_file(FFD_T fd)
{
    return &dir[fd];
}

#if RF_PAGE_DIRTY == 1
void
rfile_page_dirty(SPG_T page)
{
    devflash_page_dirty(page);
}
#endif

#if RF_SET_DIR == 1
void
rfile_set_directory(FFILE_T *pdir, ffui8_t nfiles)
{
    memcpy(dir, pdir != (FFILE_T*)0 ?  pdir : defdir,
           sizeof(FFILE_T) * nfiles);
    devflash_write_data((SA_T)0, RF_DIR_MAIN_PAGE, (ffui8_t*)dir,
                        sizeof(FFILE_T) * nfiles);
#if FF_DIR_BACKUP == 1
    devflash_copy_page_from_buff(RF_DIR_BACK_FB_ADDRESS);
#endif
}
#endif

FFILE_T *
rfile_restore_directory(ffui8_t *status)
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

    FFDBG_RESTORE_DIR(r);
    return dir;
}

/* ------------------------------ End of file ------------------------------ */
