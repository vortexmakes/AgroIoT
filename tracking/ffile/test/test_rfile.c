/**
 *  \file       test_rfile.c
 *  \brief      Unit test for rfile module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.07.01  LeFr  v1.0.00
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "ffdata.h"
#include "rfile.h"
#include "Mock_ffile.h"
#include "Mock_devflash.h"
#include "Mock_ffdir.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void 
setUp(void)
{
}

void 
tearDown(void)
{
}

void
test_RestoreDirectory(void)
{
    PageRes res;
    ffui8_t nFile;
    FFILE_T *pf;

    res.result = PAGE_OK;
    ffdir_restore_ExpectAndReturn(0, (FFILE_T *)defdir);
    ffdir_restore_IgnoreArg_status();
    devflash_verify_page_IgnoreAndReturn(res);
    for (nFile = 0, pf = (FFILE_T *)defdir; 
         nFile < NUM_FLASH_FILES; 
         ++nFile, ++pf)
    {
    }

    rfile_init_directory();
}

/* ------------------------------ End of file ------------------------------ */
