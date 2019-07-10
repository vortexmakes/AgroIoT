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
static SA_T reg_addr;
static SPG_T page_num;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
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
        ffdir_update(pf);
    }
}

void
rfile_init_directory(void)
{
    ffui8_t file, r;
    SPG_T page, page_error;
    FFILE_T *pf;
    PageRes res;

    pf = ffdir_restore(&r);
    for (file = 0; file < NUM_FLASH_FILES; ++file, ++pf)
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
        ffdir_update(pf);
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
    return ffdir_getFile(fd);
}

#if RF_PAGE_DIRTY == 1
void
rfile_page_dirty(SPG_T page)
{
    devflash_page_dirty(page);
}
#endif

void
rfile_set_directory(FFILE_T *pdir, ffui8_t nfiles)
{
    ffdir_set(pdir, nfiles);
}

/* ------------------------------ End of file ------------------------------ */
