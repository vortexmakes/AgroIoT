/**
 *  \file       Backup.c
 *  \brief      Implementation of system status backup 
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
#include "Backup.h"
#include "ff.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static DIR dir;
static FILINFO file;
static Backup backInfo;
static const Backup defBackInfo = {-1, -1, -1};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
int
Backup_init(Backup *info)
{
    FRESULT fsResult;
    int initResult = 0;

    fsResult = f_mkdir(BACKUP_FRMDIR);
    if ((fsResult == FR_OK) || (fsResult == FR_EXIST))
    {
        backInfo.nFiles = 0;
        fsResult = f_findfirst(&dir, &file, BACKUP_FRMDIR, "*.frm");
        while ((fsResult == FR_OK) && (file.fname[0] != 0))
        {
            ++backInfo.nFiles;
            fsResult = f_findnext(&dir, &file);
        }
    }
    else
    {
        initResult = 1;
        backInfo = defBackInfo;
    }

    if (info != (Backup *)0)
    {
        *info = backInfo;
    }
    return initResult;
}

/* ------------------------------ End of file ------------------------------ */
