/**
 *  \file       FileMgr.c
 *  \brief      Implementation for fatfs on stm32
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include <stdlib.h>
#include "Backup.h"
#include "GStatus.h"
#include "ff.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static GStatus status = 
{
    {
        {
            "000000", {'V'}, {"38.0030396"}, {"-"}, {"057.3266218"}, {"-"},
            {"000.000"}, {"000"}, "000000"
        },
        {0, 0, {0, 0, 0}, {0, 0, 0}, 0},
        {0, 0},
        0
    },
    0
};
static char filePath[32];
static char dirPath[32];
static FIL file;
static DIR dir;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
int 
FileMgr_rmrf(void)
{
    FRESULT result;
    FILINFO f;


    result = f_findfirst(&dir, &f, BACKUP_DIR_NAME, "*.frm");

    while ((result == FR_OK) && (f.fname[0] != 0))
    {
        sprintf(filePath, "%s/%s", BACKUP_DIR_NAME, f.fname);
        f_unlink(filePath); /* Remove *.frm file */
        result = f_findnext(&dir, &f);
    }
    f_closedir(&dir); /* Close BACKUP_DIR_NAME if it exists */
    result = f_unlink(BACKUP_DIR_NAME); /* Remove BACKUP_DIR_NAME */
                                        /* if it exists */
    return (int)result;
}

void
FileMgr_createFiles(int nFiles, uint32_t from)
{
    int i;
    FRESULT result;

    result = f_mkdir(dirPath);
    if ((result == FR_OK) || (result == FR_EXIST))
    {
        for (i = 0; i < nFiles; ++i)
        {
            sprintf(filePath, "%s/%05d.frm", dirPath, i + from);
            result = f_open(&file, filePath, 
                            FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
            if ((result == FR_OK) || (result == FR_EXIST))
            {
                f_close(&file);
            }
        }
    }
}

void 
FileMgr_cd(char *path)
{
    strcpy(dirPath, path);
}

void
FileMgr_fillFile(char *path)
{
    int i;
    FRESULT result;
    UINT nBytesWritten;

    sprintf(filePath, "%s/%s", BACKUP_DIR_NAME, path);
    result = f_open(&file, filePath, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
    if ((result == FR_OK) || (result == FR_EXIST))
    {
        for (i = 0; i < BACKUP_MAXNUMREGPERFILE; ++i)
        {
            f_write(&file, &status, BACKUP_SIZEOF_REG, &nBytesWritten);
        }
        f_sync(&file);
        f_close(&file);
    }
}

/* ------------------------------ End of file ------------------------------ */
