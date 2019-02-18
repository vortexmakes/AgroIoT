/*
 *  rfile.c
 */

#include "ffile.h"
#include "devflash.h"
#include "ffdata.h"
#include "trktrc.h"
#include "genled.h"
#include "newslog.h"
#include <string.h>

static FFILE_T dir[NUM_FLASH_FILES];
static SA_T reg_addr;
static SPG_T page_num;

void
rfile_file_format(FFILE_T *pf)
{
    FFDBG_FILE_FORMAT(pf);
    if (is_ready_to_save_in_flash())
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
    ffui8_t *pdir;
    SPG_T page, page_error;
    FFILE_T *pf;

    pdir = devflash_restore_directory(&r);
    memcpy(dir, r != DIR_BAD ? pdir : (ffui8_t*)defdir,
           sizeof(FFILE_T) * NUM_FLASH_FILES);
    FFDBG_RESTORE_DIR(r);

    for (file = 0, pf = dir; file < NUM_FLASH_FILES; ++file, ++pf)
    {
        for (page = page_error = 0; page < pf->num_pages; ++page)
            if (devflash_verify_page(page + pf->begin_page) == PAGE_BAD)
            {
                ++page_error;
/*				set_led( LED_BATT, LSTAGE1 ); */
            }
            else
            {
                break;
            }

        pf->page_error = page_error;
        #if FF_AUTO_FILE_FORMAT == 1
        {
            if (pf->page_error == pf->num_pages)
            {
                rfile_file_format(pf);
                trace_evt(file + TRC_FFD0_FORMAT);
            }
        }
        #endif
        rfile_update_directory(pf);
    }

    if (r == DIR_BAD)
    {
        trace_evt(TRC_DFLSH_DIR_BAD);
        set_led(LED_SMS, LSTAGE4);
    }
}

void
rfile_update_directory(FFILE_T *pf)
{
    SA_T da;
    ffui8_t *sa;
    ffui8_t nfiles;
    if (is_ready_to_save_in_flash())
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
/* ------------------------------ End of file ------------------------------ */
